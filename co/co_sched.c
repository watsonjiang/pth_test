#include "co.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <ucontext.h>

pthread_key_t co_sched_key = 0;
pthread_once_t co_sched_once = PTHREAD_ONCE_INIT;

struct sched_st {
    int id;
    co_pqueue_t RQ;     /* queue of coroutines ready to run       */
    pthread_mutex_t ev_lock;    /*lock to protect event queue     */
    co_event_t ev_queue_head;   /*head of event queue             */
    int ev_occurred_cnt;           /* number of event occurred      */
    pthread_cond_t ev_occurred_cond;    /*event occurred condition  */
    int          favournew;  /* favour new threads on startup     */
    ucontext_t   sched_mctx;
    co_t         co_current;
};
typedef struct sched_st * sched_t;

void co_sched_eventmanager(sched_t s, int dopoll);

static void _co_coroutine_start(void)
{
    printf("_co_coroutine_start\n");
    co_t t = co_get_current_co();
    t->start_func(t->start_arg);
    printf("_co_coroutine_start exit\n");
}


co_t co_get_current_co()
{
    sched_t s = (sched_t)pthread_getspecific(co_sched_key); 
    return s->co_current; 
}

/* create and init a scheduler struct */
sched_t co_scheduler_create()
{
    sched_t s = (sched_t)malloc(sizeof(struct sched_st));
    /* initialize the essential threads */
    s->co_current = NULL;

    /* initalize the thread queues */
    co_pqueue_init(&s->RQ);
   
    pthread_mutex_init(&s->ev_lock, NULL);
    pthread_cond_init(&s->ev_occurred_cond, NULL);
    s->ev_queue_head = NULL;

    /* initialize scheduling hints */
    s->favournew = 1; /* the default is the original behaviour */

    return s;
}

/* the heart of this library: the thread scheduler */
void *co_schedule_loop(sched_t s)
{
    /*
     * bootstrapping
     */
    /* mark this thread as the special scheduler thread */
    getcontext(&s->sched_mctx);

    /*
     * endless scheduler loop
     */
    for (;;) {
        /*
         * Find next thread in ready queue
         */
        s->co_current = co_pqueue_delmax(&s->RQ);
        if(NULL != s->co_current) {
            printf("co_scheduler: thread \"%s\" selected (prio=%d, qprio=%d)\n",
                   s->co_current->name, s->co_current->prio, s->co_current->q_prio);

            /*
             * Set running start time for new thread
             * and perform a context switch to it
             */
            printf("co_scheduler: switching to thread 0x%lx (\"%s\")\n",
                   (unsigned long)s->co_current, s->co_current->name);

            /* ** ENTERING THREAD ** - by switching the machine context */
            swapcontext(&s->sched_mctx, &s->co_current->mctx);

            printf("co_scheduler: cameback from thread 0x%lx (\"%s\")\n",
                   (unsigned long)s->co_current, s->co_current->name);
        }

        if (co_pqueue_elements(&s->RQ) == 0)
            /* still no NEW or READY threads, so we have to wait for new work */
            co_sched_eventmanager(s, FALSE /* wait */);
        else
            /* already NEW or READY threads exists, so just poll for even more work */
            co_sched_eventmanager(s, TRUE  /* poll */);
    }

    /* NOTREACHED */
    return NULL;
}

/*
 * Look whether some events already occurred (or failed) and move
 * corresponding coroutines to ready queue.
 */
void co_sched_eventmanager(sched_t s, int dopoll)
{
    printf("co_sched_eventmanager: enter in %s mode\n",
               dopoll ? "polling" : "waiting");

    co_event_t ev_head = NULL;

    /* now decide how to poll for events and timers */
    if (dopoll) {
        /* do a polling with immediate timeout.
         * already have event occurred or new
         * coroutine waiting in the RQ 
         */
        pthread_mutex_lock(&s->ev_lock);
        if (s->ev_occurred_cnt > 0) {
            ev_head = s->ev_queue_head;
            s->ev_queue_head = NULL;
        }
        pthread_mutex_unlock(&s->ev_lock);
    }
    else {
        /* do a polling without a timeout,
           i.e. wait for the event only with blocking */
        pthread_mutex_lock(&s->ev_lock);
        while(s->ev_occurred_cnt <= 0) {
            pthread_cond_wait(&s->ev_occurred_cond, &s->ev_lock);
            printf("wakeup %d\n", s->ev_occurred_cnt);
        }
        ev_head = s->ev_queue_head;
        s->ev_queue_head = NULL;
        pthread_mutex_unlock(&s->ev_lock);
    }

    /* loop all events, put corresponding coroutines into RQ */
    while(ev_head!=NULL) {
        co_event_t ev = ev_head;
        ev_head = ev_head->ev_next;
        ev->ev_next = NULL;
        co_t t = ev->coroutine;
        /*goto end of coroutine's event queue, link event*/
        if (t->events == NULL) {
            t->events = ev;
        }else {
            co_event_t prev = t->events;
            co_event_t cur = prev->ev_next;
            while(cur != NULL) {
                prev = cur;
                cur = cur->ev_next;
            }
            prev->ev_next = ev;
        }
        /* change the uc_link of mctx, make sure coroutine will 
         * back to schedule loop */
        if (ev->ev_type == CO_EVENT_NEW_CO) {
            co_t t = ev->coroutine;
            getcontext(&t->mctx);
            t->mctx.uc_stack.ss_sp = t->stack;
            t->mctx.uc_stack.ss_size = t->stacksize;
            ev->coroutine->mctx.uc_link = &s->sched_mctx;
            makecontext(&t->mctx, _co_coroutine_start, 0);
        }
        /* before put coroutine into RQ, check if it already in RQ */
        co_t t1 = co_pqueue_head(&s->RQ);
        int already_in_rq = FALSE;
        for(; t1!=NULL; t1=co_pqueue_walk(&s->RQ, t1, CO_WALK_NEXT)) {
           if (t == t1) {
               already_in_rq = TRUE;
               break;
           }
        }
        /*
         * move last coroutine to ready queue if any events occurred for it.
         * we insert it with a slightly increased queue priority to it a
         * better chance to immediately get scheduled, else the last running
         * thread might immediately get again the CPU which is usually not
         * what we want, because we oven use pth_yield() calls to give others
         * a chance.
         */
        if (!already_in_rq) {
            printf("insert coroutine  %p to RQ\n", t);
            co_pqueue_insert(&s->RQ, t->prio+1, t);
        }
        pthread_mutex_lock(&s->ev_lock);
        s->ev_occurred_cnt -= 1;
        pthread_mutex_unlock(&s->ev_lock);
    }
    printf("co_sched_eventmanager: leaving\n");
    return;
}

static int num_of_sched = 0;
static sched_t * g_co_sched_list = NULL;
static int next_sched_idx = 0;

static void* _lunch_sched(void* arg)
{
    int *id = (int*) arg;
    sched_t sched = co_scheduler_create();
    g_co_sched_list[*id] = sched;
    sched->id = *id; 
    free(id);
    pthread_setspecific(co_sched_key, sched);
    co_schedule_loop(sched);  
}

void co_sched_init() 
{
    pthread_key_create(&co_sched_key, NULL);
}

void co_lunch_scheduler(int num)
{
    int i;
    int ret;
    pthread_t th;
    
    pthread_once(&co_sched_once, co_sched_init);
    num_of_sched = num;
    g_co_sched_list = (sched_t*)malloc(num * sizeof(sched_t));
    for(i=0;i<num;i++) {
        printf("create thread %d\n", i);
        int * id = (int*)malloc(sizeof(int));
        *id = i;
        ret = pthread_create(&th, NULL, _lunch_sched, id);
        if(ret != 0) {
            printf("create thread failed! ret %d\n", ret);
        }
    }
}

co_t co_ccb_alloc(unsigned int stacksize, void* stackaddr)
{
    co_t t = (co_t) malloc(sizeof(struct co_st));
    t->stacksize = stacksize;
    t->stack = NULL;
    if (NULL != stackaddr) {
       t->stack = (char *)stackaddr;
    }else {
       t->stack = (char *)malloc(stacksize);
    }
    t->stackguard = (long *)((long)t->stack);
    *t->stackguard = 0xDEAD;
    return t;
}



co_t co_create_co(void* (*func)(void*), void *arg)
{
    co_t t;
    t = co_ccb_alloc(64*1024, NULL);
    t->prio = CO_PRIO_STD;
    t->events = NULL;
    t->start_func = func;
    t->start_arg = arg;
    co_event_t ev = (co_event_t) malloc(sizeof(struct co_event_st));
    ev->coroutine = t;
    ev->ev_type = CO_EVENT_NEW_CO;
    sched_t s = g_co_sched_list[next_sched_idx];
    next_sched_idx = (next_sched_idx + 1) % num_of_sched;
    pthread_mutex_lock(&s->ev_lock);
    if (s->ev_queue_head == NULL) {
        s->ev_queue_head = ev;
    }else {
        co_event_t c;
        c = s->ev_queue_head;
        while(NULL != c->ev_next) {
            c = c->ev_next;
        }
        c->ev_next = ev;
        ev->ev_next = NULL;
    }
    printf("put event\n");
    s->ev_occurred_cnt += 1;
    pthread_cond_signal(&s->ev_occurred_cond);
    pthread_mutex_unlock(&s->ev_lock);
}



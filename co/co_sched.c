#include "co.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_key_t co_sched_key = 0;

struct sched_st {
    co_pqueue_t RQ;     /* queue of coroutines ready to run       */
    pthread_mutex_t ev_lock;    /*lock to protect event queue     */
    co_event_t ev_queue_head;   /*head of event queue             */
    int ev_occurred_cnt;           /* number of event occurred      */
    pthread_cond_t ev_occurred_cond;    /*event occurred condition  */
    int          favournew;  /* favour new threads on startup     */
    co_t         co_sched;
    co_t         co_current;
};
typedef struct sched_st * sched_t;

void co_sched_eventmanager(sched_t s, int dopoll);

co_t co_get_current_co()
{
    sched_t s = (sched_t)pthread_getspecific(co_sched_key); 
    return s->co_current; 
}

/* initialize the scheduler ingredients */
int co_scheduler_init(sched_t s)
{
    /* initialize the essential threads */
    s->co_sched   = NULL;
    s->co_current = NULL;

    /* initalize the thread queues */
    co_pqueue_init(&s->RQ);
   
    pthread_mutex_init(&s->ev_lock, NULL);
    pthread_cond_init(&s->ev_occurred_cond, NULL);
    s->ev_queue_head = NULL;

    /* initialize scheduling hints */
    s->favournew = 1; /* the default is the original behaviour */

    return TRUE;
}

/* the heart of this library: the thread scheduler */
void *co_scheduler(sched_t s)
{
    /*
     * bootstrapping
     */
    /* mark this thread as the special scheduler thread */
    s->co_sched->state = CO_STATE_SCHEDULER;

    /*
     * endless scheduler loop
     */
    for (;;) {
        /*
         * Find next thread in ready queue
         */
        s->co_current = co_pqueue_delmax(&s->RQ);
        printf("co_scheduler: thread \"%s\" selected (prio=%d, qprio=%d)",
               s->co_current->name, s->co_current->prio, s->co_current->q_prio);

        /*
         * Set running start time for new thread
         * and perform a context switch to it
         */
        printf("co_scheduler: switching to thread 0x%lx (\"%s\")",
               (unsigned long)s->co_current, s->co_current->name);

        /* ** ENTERING THREAD ** - by switching the machine context */
        co_mctx_switch(&s->co_sched->mctx, &s->co_current->mctx);

        printf("co_scheduler: cameback from thread 0x%lx (\"%s\")",
                  (unsigned long)s->co_current, s->co_current->name);

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
    printf("co_sched_eventmanager: enter in %s mode",
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
        while(s->ev_occurred_cnt <= 0)
            pthread_cond_wait(&s->ev_occurred_cond, &s->ev_lock);
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
        /* before put coroutine into RQ, check if it already in RQ */
        co_t t1 = co_pqueue_head(&s->RQ);
        int already_in_rq = FALSE;
        while(NULL != co_pqueue_walk(&s->RQ, t1, CO_WALK_NEXT)) {
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
            co_pqueue_insert(&s->RQ, t->prio+1, t);
        }
    }
    printf("co_sched_eventmanager: leaving");
    return;
}


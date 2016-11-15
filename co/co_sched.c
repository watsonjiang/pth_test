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
    int ev_occured_cnt;           /* number of event occured      */
    pthread_cond_t ev_occurred_cond;    /*event occured condition  */
    int          favournew;  /* favour new threads on startup     */
    co_t         co_sched;
    co_t         co_current;
};
typedef struct sched_st * sched_t;

void co_sched_eventmanager(sched_t s, co_time_t *now, int dopoll);

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
    co_time_t running;
    co_time_t snapshot;
    co_t t;

    /*
     * bootstrapping
     */
    /* mark this thread as the special scheduler thread */
    s->co_sched->state = CO_STATE_SCHEDULER;

    /*
     * endless scheduler loop
     */
    for (;;) {
        co_t tmp_head = NULL;
        co_t tmp_cur = NULL;
        tmp_cur = tmp_head;
        pthread_mutex_lock(&s->ev_lock);
        while(tmp_cur!=NULL) {
            s->ev_occured_cnt -= 1;
            t = tmp_cur; 
            tmp_cur = t->q_next;
            if (s->favournew)
                co_pqueue_insert(&s->RQ, co_pqueue_favorite_prio(&s->RQ), t);
            else
                co_pqueue_insert(&s->RQ, CO_PRIO_STD, t);
        }
        pthread_mutex_unlock(&s->ev_lock);

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
        /*
         * migrate old treads in ready queue into higher
         * priorities to avoid starvation and insert last running
         * thread back into this queue, too.
         */
        co_pqueue_increase(&s->RQ);
        if (s->co_current != NULL)
            co_pqueue_insert(&s->RQ, s->co_current->prio, s->co_current);

        /*
         * Manage the events in the waiting queue, i.e. decide whether their
         * events occurred and move them to the ready queue. But wait only if
         * we have already no new or ready threads.
         */
        if (co_pqueue_elements(&s->RQ) == 0)
            /* still no NEW or READY threads, so we have to wait for new work */
            co_sched_eventmanager(s, &snapshot, FALSE /* wait */);
        else
            /* already NEW or READY threads exists, so just poll for even more work */
            co_sched_eventmanager(s, &snapshot, TRUE  /* poll */);
    }

    /* NOTREACHED */
    return NULL;
}

/*
 * Look whether some events already occurred (or failed) and move
 * corresponding threads from waiting queue back to ready queue.
 */
void co_sched_eventmanager(sched_t s, co_time_t *now, int dopoll)
{
    co_t nexttimer_thread;
    co_event_t nexttimer_ev;
    co_time_t nexttimer_value;
    co_event_t evh;
    co_event_t ev;
    co_t t;
    co_t tlast;
    int this_occurred;
    int any_occurred;
    struct timeval delay;
    struct timeval *pdelay;
    char minibuf[128];
    int rc;
    int n;

    printf("co_sched_eventmanager: enter in %s mode",
               dopoll ? "polling" : "waiting");

    co_event_t ev_head = NULL;
    /* now decide how to poll for events and timers */
    if (dopoll) {
        /* do a polling with immediate timeout.
         * already have event occured or new
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
        while(s->ev_occured_cnt <= 0)
            pthread_cond_wait(&s->ev_occured_cond, &s->ev_lock);
        ev_head = s->ev_queue_head;
        s->ev_queue_head = NULL;
        pthread_mutex_unlock(&s->ev_lock);
    }

    /* loop all coroutines, put which has event occured into RQ */
    while(ev_head!=NULL) {
        co_event_t ev = ev_head;
        ev_head = ev_head->ev_next;
        co_t t = ev->coroutine;
        tlast = t;
        if (t->events == NULL) {
            t->events = ev;
        }else {
            t->events->ev_next = ev;
        }
        /*
         * move last coroutine to ready queue if any events occurred for it.
         * we insert it with a slightly increased queue priority to it a
         * better chance to immediately get scheduled, else the last running
         * thread might immediately get again the CPU which is usually not
         * what we want, because we oven use pth_yield() calls to give others
         * a chance.
         */
         co_pqueue_insert(&s->RQ, t->prio+1, t);
    }
    printf("co_sched_eventmanager: leaving");
    return;
}


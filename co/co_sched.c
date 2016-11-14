#include "pth.h"

co_pqueue_t co_NQ;      /* queue of new threads                  */

pthread_key_t co_sched_key = 0;
pthread_once_t init_once = PTHREAD_ONCE_INIT;

struct sched_st {
    co_pqueue_t RQ;     /* queue of coroutines ready to run       */
    co_pqueue_t WQ;     /* queue of coroutines waiting for event  */
    pthread_mutex_t ev_lock;    /*lock to protect event queue     */
    int ev_occured_cnt;           /* number of event occured      */
    pthread_cond_t ev_occured_cond;    /*event occured condition  */
    int          favournew;  /* favour new threads on startup     */
    co_t         co_sched;
    co_t         co_current;
};
typedef sched_st * sched_t;

co_t co_get_current_co()
{
    sched_t s = (sched_t)pthread_getspecific(co_sched_key); 
    return s->co_current; 
}

/* initialize the scheduler ingredients */
int co_scheduler_init(sched_st s)
{
    /* initialize the essential threads */
    s->co_sched   = NULL;
    s->co_current = NULL;

    /* initalize the thread queues */
    co_pqueue_init(&s->RQ);
    co_pqueue_init(&s->WQ);

    /* initialize scheduling hints */
    s->co_favournew = 1; /* the default is the original behaviour */

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
    s->co_sched->state = PTH_STATE_SCHEDULER;

    /* initialize the snapshot time for bootstrapping the loop */
    co_time_set(&snapshot, PTH_TIME_NOW);

    /*
     * endless scheduler loop
     */
    for (;;) {
        /*
         * Move threads from new queue to ready queue and optionally
         * give them maximum priority so they start immediately.
         */
        while ((t = co_pqueue_tail(&co_NQ)) != NULL) {
            co_pqueue_delete(&co_NQ, t);
            t->state = PTH_STATE_READY;
            if (s->co_favournew)
                co_pqueue_insert(&s->co_RQ, co_pqueue_favorite_prio(&s->co_RQ), t);
            else
                co_pqueue_insert(&s->co_RQ, PTH_PRIO_STD, t);
        }

        /*
         * Find next thread in ready queue
         */
        s->co_current = co_pqueue_delmax(&s->co_RQ);
        if (s->co_current == NULL) {
            fprintf(stderr, "**Pth** SCHEDULER INTERNAL ERROR: "
                            "no more thread(s) available to schedule!?!?\n");
            abort();
        }
        printf("co_scheduler: thread \"%s\" selected (prio=%d, qprio=%d)",
               co_current->name, co_current->prio, co_current->q_prio);

        /*
         * Set running start time for new thread
         * and perform a context switch to it
         */
        printf("co_scheduler: switching to thread 0x%lx (\"%s\")",
               (unsigned long)co_current, co_current->name);

        /* ** ENTERING THREAD ** - by switching the machine context */
        co_mctx_switch(&s->co_sched->mctx, &s->co_current->mctx);

        printf("co_scheduler: cameback from thread 0x%lx (\"%s\")",
               (unsigned long)co_current, co_current->name);

        /*
         * If thread wants to wait for an event
         * move it to waiting queue now
         */
        if (s->co_current != NULL && s->co_current->state == PTH_STATE_WAITING) {
            co_debug2("co_scheduler: moving thread \"%s\" to waiting queue",
                       co_current->name);
            co_pqueue_insert(&s->co_WQ, s->co_current->prio, s->co_current);
            s->co_current = NULL;
        }

        /*
         * migrate old treads in ready queue into higher
         * priorities to avoid starvation and insert last running
         * thread back into this queue, too.
         */
        co_pqueue_increase(&s->co_RQ);
        if (s->co_current != NULL)
            co_pqueue_insert(&s->co_RQ, s->co_current->prio, s->co_current);

        /*
         * Manage the events in the waiting queue, i.e. decide whether their
         * events occurred and move them to the ready queue. But wait only if
         * we have already no new or ready threads.
         */
        if (   co_pqueue_elements(&s->RQ) == 0
            && co_pqueue_elements(&co_NQ) == 0)
            /* still no NEW or READY threads, so we have to wait for new work */
            co_sched_eventmanager(&snapshot, FALSE /* wait */);
        else
            /* already NEW or READY threads exists, so just poll for even more work */
            co_sched_eventmanager(&snapshot, TRUE  /* poll */);
    }

    /* NOTREACHED */
    return NULL;
}

/*
 * Look whether some events already occurred (or failed) and move
 * corresponding threads from waiting queue back to ready queue.
 */
void co_sched_eventmanager(co_time_t *now, int dopoll)
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

    /* initialize next timer */
    co_time_set(&nexttimer_value, PTH_TIME_ZERO);
    nexttimer_thread = NULL;
    nexttimer_ev = NULL;

    /* for all threads in the waiting queue... */
    any_occurred = FALSE;
    for (t = co_pqueue_head(&co_WQ); t != NULL;
         t = co_pqueue_walk(&co_WQ, t, PTH_WALK_NEXT)) {

        /* cancellation support */
        if (t->cancelreq == TRUE)
            any_occurred = TRUE;

        /* ... and all their events... */
        if (t->events == NULL)
            continue;
        /* ...check whether events occurred */
        ev = evh = t->events;
        do {
            if (ev->ev_status == PTH_STATUS_PENDING) {
                this_occurred = FALSE;
                /* Timer */
                else if (ev->ev_type == PTH_EVENT_TIME) {
                    if (co_time_cmp(&(ev->ev_args.TIME.tv), now) < 0)
                        this_occurred = TRUE;
                    else {
                        /* remember the timer which will be elapsed next */
                        if ((nexttimer_thread == NULL && nexttimer_ev == NULL) ||
                            co_time_cmp(&(ev->ev_args.TIME.tv), &nexttimer_value) < 0) {
                            nexttimer_thread = t;
                            nexttimer_ev = ev;
                            co_time_set(&nexttimer_value, &(ev->ev_args.TIME.tv));
                        }
                    }
                }
                /* tag event if it has occurred */
                if (this_occurred) {
                    printf("co_sched_eventmanager: [non-I/O] event occurred for thread \"%s\"", t->name);
                    ev->ev_status = PTH_STATUS_OCCURRED;
                    any_occurred = TRUE;
                }
            }
        } while ((ev = ev->ev_next) != evh);
    }
    if (any_occurred)
        dopoll = TRUE;

    /* now decide how to poll for events and timers */
    if (dopoll) {
        /* do a polling with immediate timeout.
         * already have event occured or new
         * coroutine waiting in the RQ 
         */
         //DO NOTHING. 
    }
    else if (nexttimer_ev != NULL) {
        /* do a polling with a timeout set to the next timer,
           i.e. wait for the fd sets or the next timer */
        co_time_set(&delay, &nexttimer_value);
        co_time_sub(&delay, now);
        pthread_mutex_lock(&s->ev_lock);
        int rc = 0;
        while(s->ev_occured == 0 ) 
            rc = pthread_cond_timewait(&s->ev_occured_cond, &s->ev_lock, delay);
        if (rc == ETIMEOUT) {
            /* it was an explicit timer event, standing for its own */
            co_debug2("co_sched_eventmanager: [timeout] event occurred for thread \"%s\"",
                       nexttimer_thread->name);
            nexttimer_ev->ev_status = PTH_STATUS_OCCURRED;
        } 
        pthread_mutex_unlock(&s->ev_lock);
    }
    else {
        /* do a polling without a timeout,
           i.e. wait for the event only with blocking */
        pthread_mutex_lock(&s->ev_lock);
        while(s->ev_occured <= 0)
            pthread_cond_wait(&s->ev_occured_cond, &s->ev_lock);
        pthread_mutex_unlock(&s->ev_lock);
    }

    /* loop all coroutines, put which has event occured into RQ */
    t = co_pqueue_head(&s->WQ);
    while(t != NULL) {
        if (t->events != NULL) {
            ev = evh = t->events;
            do {
                if (ev->ev_status == CO_STATUS_OCCURRED) {
                    any_occured = TRUE; 
                    pthread_mutex_lock(&s->ev_lock);
                    s->ev_occured_cnt -= 1;
                    pthread_mutex_unlock(&s->ev_lock);
                }
            }while((ev = ev->ev_next) != evh); 
        }

        tlast = t;
        t = co_pqueue_walk(&s->WQ, t, CO_WALK_NEXT);

        /*
         * move last coroutine to ready queue if any events occurred for it.
         * we insert it with a slightly increased queue priority to it a
         * better chance to immediately get scheduled, else the last running
         * thread might immediately get again the CPU which is usually not
         * what we want, because we oven use pth_yield() calls to give others
         * a chance.
         */
        if (any_occured) {
            co_pqueue_delete(&WQ, tlast); 
            tlast->state = CO_STATE_READY;
            co_pqueue_insert(&RQ, tlast->prio+1, tlast);
        }
    }

    printf("co_sched_eventmanager: leaving");
    return;
}


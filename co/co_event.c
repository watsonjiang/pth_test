#include "co.h"


/* event structure */
struct co_event_st {
    struct co_event_st *ev_next;
    struct co_event_st *ev_prev;
    co_status_t ev_status;
    int ev_type;
    int ev_goal;
    union {
        struct { co_time_t tv; }                                   TIME;
        struct { void *p; }                                        MSG;
    } ev_args;
};

/* event structure destructor */
static void co_event_destructor(void *vp)
{
    /* free this single(!) event. That it is just a single event is a
       requirement for co_event(PTH_MODE_STATIC, ...), or else we would
       get into horrible trouble on asychronous cleanups */
    co_event_free((co_event_t)vp, PTH_FREE_THIS);
    return;
}

/* event structure constructor */
co_event_t co_event(unsigned long spec, ...)
{
    co_event_t ev;
    co_key_t *ev_key;
    va_list ap;

    va_start(ap, spec);

    /* allocate new or reuse static or supplied event structure */
    if (spec & PTH_MODE_REUSE) {
        /* reuse supplied event structure */
        ev = va_arg(ap, co_event_t);
    }
    else if (spec & PTH_MODE_STATIC) {
        /* reuse static event structure */
        ev_key = va_arg(ap, co_key_t *);
        if (*ev_key == PTH_KEY_INIT)
            co_key_create(ev_key, co_event_destructor);
        ev = (co_event_t)co_key_getdata(*ev_key);
        if (ev == NULL) {
            ev = (co_event_t)malloc(sizeof(struct co_event_st));
            co_key_setdata(*ev_key, ev);
        }
    }
    else {
        /* allocate new dynamic event structure */
        ev = (co_event_t)malloc(sizeof(struct co_event_st));
    }
    if (ev == NULL)
        return co_error((co_event_t)NULL, errno);

    /* create new event ring out of event or insert into existing ring */
    if (spec & PTH_MODE_CHAIN) {
        co_event_t ch = va_arg(ap, co_event_t);
        ev->ev_prev = ch->ev_prev;
        ev->ev_next = ch;
        ev->ev_prev->ev_next = ev;
        ev->ev_next->ev_prev = ev;
    }
    else {
        ev->ev_prev = ev;
        ev->ev_next = ev;
    }

    /* initialize common ingredients */
    ev->ev_status = PTH_STATUS_PENDING;

    /* initialize event specific ingredients */
    if (spec & PTH_EVENT_TIME) {
        /* interrupt request event */
        co_time_t tv = va_arg(ap, co_time_t);
        ev->ev_type = PTH_EVENT_TIME;
        ev->ev_goal = (int)(spec & (PTH_UNTIL_OCCURRED));
        ev->ev_args.TIME.tv = tv;
    }
    else if (spec & PTH_EVENT_MSG) {
        /* message event */
        void *p = va_arg(ap, void*);
        ev->ev_type = PTH_EVENT_MSG;
        ev->ev_goal = (int)(spec & (PTH_UNTIL_OCCURRED));
        ev->ev_args.MSG.p = p;
    }
    else
        return co_error((co_event_t)NULL, EINVAL);

    va_end(ap);

    /* return event */
    return ev;
}

/* determine type of event */
unsigned long co_event_typeof(co_event_t ev)
{
    if (ev == NULL)
        return co_error(0, EINVAL);
    return (ev->ev_type | ev->ev_goal);
}

/* event extractor */
int co_event_extract(co_event_t ev, ...)
{
    va_list ap;

    if (ev == NULL)
        return co_error(FALSE, EINVAL);
    va_start(ap, ev);

    /* extract event specific ingredients */
    if (ev->ev_type & PTH_EVENT_TIME) {
        /* interrupt request event */
        co_time_t *tv = va_arg(ap, co_time_t *);
        *tv = ev->ev_args.TIME.tv;
    }
    else if (ev->ev_type & PTH_EVENT_MSG) {
        /* message event */
        void **p = va_arg(ap, void **);
        *mp = ev->ev_args.MSG.p;
    }
    else
        return co_error(FALSE, EINVAL);
    va_end(ap);
    return TRUE;
}

/* concatenate one or more events or event rings */
co_event_t co_event_concat(co_event_t evf, ...)
{
    co_event_t evc; /* current event */
    co_event_t evn; /* next event */
    co_event_t evl; /* last event */
    co_event_t evt; /* temporary event */
    va_list ap;

    if (evf == NULL)
        return co_error((co_event_t)NULL, EINVAL);

    /* open ring */
    va_start(ap, evf);
    evc = evf;
    evl = evc->ev_next;

    /* attach additional rings */
    while ((evn = va_arg(ap, co_event_t)) != NULL) {
        evc->ev_next = evn;
        evt = evn->ev_prev;
        evn->ev_prev = evc;
        evc = evt;
    }

    /* close ring */
    evc->ev_next = evl;
    evl->ev_prev = evc;
    va_end(ap);

    return evf;
}

/* isolate one event from a possible appended event ring */
co_event_t co_event_isolate(co_event_t ev)
{
    co_event_t ring;

    if (ev == NULL)
        return co_error((co_event_t)NULL, EINVAL);
    ring = NULL;
    if (!(ev->ev_next == ev && ev->ev_prev == ev)) {
        ring = ev->ev_next;
        ev->ev_prev->ev_next = ev->ev_next;
        ev->ev_next->ev_prev = ev->ev_prev;
        ev->ev_prev = ev;
        ev->ev_next = ev;
    }
    return ring;
}

/* determine status of the event */
co_status_t co_event_status(co_event_t ev)
{
    if (ev == NULL)
        return co_error(FALSE, EINVAL);
    return ev->ev_status;
}

/* walk to next or previous event in an event ring */
co_event_t co_event_walk(co_event_t ev, unsigned int direction)
{
    if (ev == NULL)
        return co_error((co_event_t)NULL, EINVAL);
    do {
        if (direction & PTH_WALK_NEXT)
            ev = ev->ev_next;
        else if (direction & PTH_WALK_PREV)
            ev = ev->ev_prev;
        else
            return co_error((co_event_t)NULL, EINVAL);
    } while ((direction & PTH_UNTIL_OCCURRED) && (ev->ev_status != PTH_STATUS_OCCURRED));
    return ev;
}

/* deallocate an event structure */
int co_event_free(co_event_t ev, int mode)
{
    co_event_t evc;
    co_event_t evn;

    if (ev == NULL)
        return co_error(FALSE, EINVAL);
    if (mode == PTH_FREE_THIS) {
        ev->ev_prev->ev_next = ev->ev_next;
        ev->ev_next->ev_prev = ev->ev_prev;
        free(ev);
    }
    else if (mode == PTH_FREE_ALL) {
        evc = ev;
        do {
            evn = evc->ev_next;
            free(evc);
            evc = evn;
        } while (evc != ev);
    }
    return TRUE;
}

/* wait for one or more events */
int co_wait(co_event_t ev_ring)
{
    int nonpending;
    co_event_t ev;

    /* at least a waiting ring is required */
    if (ev_ring == NULL)
        return co_error(-1, EINVAL);
    co_debug2("co_wait: enter from thread \"%s\"", co_current->name);

    /* mark all events in waiting ring as still pending */
    ev = ev_ring;
    do {
        ev->ev_status = PTH_STATUS_PENDING;
        co_debug2("co_wait: waiting on event 0x%lx", (unsigned long)ev);
        ev = ev->ev_next;
    } while (ev != ev_ring);

    /* link event ring to current thread */
    co_current->events = ev_ring;

    /* move thread into waiting state
       and transfer control to scheduler */
    co_current->state = PTH_STATE_WAITING;
    co_yield(NULL);

    /* check for cancellation */
    co_cancel_point();

    /* unlink event ring from current thread */
    co_current->events = NULL;

    /* count number of actually occurred (or failed) events */
    ev = ev_ring;
    nonpending = 0;
    do {
        if (ev->ev_status != PTH_STATUS_PENDING) {
            co_debug2("co_wait: non-pending event 0x%lx", (unsigned long)ev);
            nonpending++;
        }
        ev = ev->ev_next;
    } while (ev != ev_ring);

    /* leave to current thread with number of occurred events */
    co_debug2("co_wait: leave to thread \"%s\"", co_current->name);
    return nonpending;
}

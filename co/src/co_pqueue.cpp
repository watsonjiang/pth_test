#include "co.h"
#include "co_pqueue.h"
#include <pthread.h>


/* initialize a priority queue; O(1) */
void co_pqueue_init(co_pqueue_t *q)
{
    if (q != NULL) {
        q->q_head = NULL;
        q->q_num  = 0;
    }
    return;
}

/* insert thread into priority queue; O(n) */
void co_pqueue_insert(co_pqueue_t *q, int prio, co_t t)
{
    co_t c;
    int p;
    if (q == NULL)
        return;
    if (q->q_head == NULL || q->q_num == 0) {
        /* add as first element */
        t->q_prev = t;
        t->q_next = t;
        t->q_prio = prio;
        q->q_head = t;
    }
    else if (q->q_head->q_prio < prio) {
        /* add as new head of queue */
        t->q_prev = q->q_head->q_prev;
        t->q_next = q->q_head;
        t->q_prev->q_next = t;
        t->q_next->q_prev = t;
        t->q_prio = prio;
        t->q_next->q_prio = prio - t->q_next->q_prio;
        q->q_head = t;
    }
    else {
        /* insert after elements with greater or equal priority */
        c = q->q_head;
        p = c->q_prio;
        while ((p - c->q_next->q_prio) >= prio && c->q_next != q->q_head) {
            c = c->q_next;
            p -= c->q_prio;
        }
        t->q_prev = c;
        t->q_next = c->q_next;
        t->q_prev->q_next = t;
        t->q_next->q_prev = t;
        t->q_prio = p - prio;
        if (t->q_next != q->q_head)
            t->q_next->q_prio -= t->q_prio;
    }
    q->q_num++;
    return;
}

/* remove thread with maximum priority from priority queue; O(1) */
co_t co_pqueue_delmax(co_pqueue_t *q)
{
    co_t t;

    if (q == NULL)
        return NULL;
    pthread_mutex_lock(&q->lock);
    if (q->q_head == NULL)
        t = NULL;
    else if (q->q_head->q_next == q->q_head) {
        /* remove the last element and make queue empty */
        t = q->q_head;
        t->q_next = NULL;
        t->q_prev = NULL;
        t->q_prio = 0;
        q->q_head = NULL;
        q->q_num  = 0;
    }
    else {
        /* remove head of queue */
        t = q->q_head;
        t->q_prev->q_next = t->q_next;
        t->q_next->q_prev = t->q_prev;
        t->q_next->q_prio = t->q_prio - t->q_next->q_prio;
        t->q_prio = 0;
        q->q_head = t->q_next;
        q->q_num--;
    }
    pthread_mutex_unlock(&q->lock);
    return t;
}

/* remove thread from priority queue; O(n) */
void co_pqueue_delete(co_pqueue_t *q, co_t t)
{
    if (q == NULL)
        return;
    pthread_mutex_lock(&q->lock);
    if (q->q_head == NULL) {
        pthread_mutex_unlock(&q->lock);
        return;
    }
    else if (q->q_head == t) {
        if (t->q_next == t) {
            /* remove the last element and make queue empty */
            t->q_next = NULL;
            t->q_prev = NULL;
            t->q_prio = 0;
            q->q_head = NULL;
            q->q_num  = 0;
        }
        else {
            /* remove head of queue */
            t->q_prev->q_next = t->q_next;
            t->q_next->q_prev = t->q_prev;
            t->q_next->q_prio = t->q_prio - t->q_next->q_prio;
            t->q_prio = 0;
            q->q_head = t->q_next;
            q->q_num--;
        }
    }
    else {
        t->q_prev->q_next = t->q_next;
        t->q_next->q_prev = t->q_prev;
        if (t->q_next != q->q_head)
            t->q_next->q_prio += t->q_prio;
        t->q_prio = 0;
        q->q_num--;
    }
    pthread_mutex_unlock(&q->lock);
    return;
}

/* move a thread inside queue to the top; O(n) */
int co_pqueue_favorite(co_pqueue_t *q, co_t t)
{
    if (q == NULL)
        return FALSE;
    pthread_mutex_lock(&q->lock);
    if (q->q_head == NULL || q->q_num == 0) {
        pthread_mutex_unlock(&q->lock);
        return FALSE;
    }
    /* element is already at top */
    if (q->q_num == 1) {
        pthread_mutex_unlock(&q->lock);
        return TRUE;
    }
    /* move to top */
    co_pqueue_delete(q, t);
    co_pqueue_insert(q, co_pqueue_favorite_prio(q), t);
    pthread_mutex_lock(&q->lock);
    return TRUE;
}

/* increase priority of all(!) threads in queue; O(1) */
void co_pqueue_increase(co_pqueue_t *q)
{
    if (q == NULL)
        return;
    if (q->q_head == NULL)
        return;
    /* <grin> yes, that's all ;-) */
    q->q_head->q_prio += 1;
    return;
}

/* walk to last thread in queue */
co_t co_pqueue_tail(co_pqueue_t *q)
{
    if (q == NULL)
        return NULL;
    if (q->q_head == NULL)
        return NULL;
    return q->q_head->q_prev;
}

/* walk to next or previous thread in queue; O(1) */
co_t co_pqueue_walk(co_pqueue_t *q, co_t t, int direction)
{
    co_t tn;

    if (q == NULL || t == NULL)
        return NULL;
    tn = NULL;
    if (direction == CO_WALK_PREV) {
        if (t != q->q_head)
            tn = t->q_prev;
    }
    else if (direction == CO_WALK_NEXT) {
        tn = t->q_next;
        if (tn == q->q_head)
            tn = NULL;
    }
    return tn;
}

/* check whether a thread is in a queue; O(n) */
int co_pqueue_contains(co_pqueue_t *q, co_t t)
{
    co_t tc;
    int found;

    found = FALSE;
    for (tc = co_pqueue_head(q); tc != NULL;
         tc = co_pqueue_walk(q, tc, CO_WALK_NEXT)) {
        if (tc == t) {
            found = TRUE;
            break;
        }
    }
    return found;
}


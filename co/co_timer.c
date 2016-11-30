#include "co.h"
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
/*
 * a module implements timer service.
 */

typedef void (*timeout_callback_t)(void * arg);

typedef struct co_timer_entry_st * co_timer_entry_t;
struct co_timer_entry_st {
    co_timer_entry_t next;
    struct timespec timeout;
    timeout_callback_t cb;
    void * cb_arg;
};

struct co_timer_st {
    co_timer_entry_t timer_list;
    pthread_mutex_t new_timer_lock;
    pthread_cond_t  new_timer_cond;
    co_timer_entry_t new_timer_list;
};

typedef struct co_timer_st * co_timer_t;

void co_timer_init(co_timer_t *t)
{
    co_timer_t tmp = (co_timer_t)malloc(sizeof(struct co_timer_st));
    pthread_mutex_init(&tmp->new_timer_lock, NULL);
    pthread_cond_init(&tmp->new_timer_cond, NULL);
    tmp->new_timer_list == NULL;
    tmp->timer_list = NULL;
    *t = tmp;
}

void co_timer_schedule(co_timer_t t, struct timespec abs_timeout, timeout_callback_t cb, void *cb_arg)
{
    co_timer_entry_t entry = (co_timer_entry_t)malloc(sizeof(struct co_timer_entry_st));
    entry->next = NULL;
    entry->timeout = abs_timeout;
    entry->cb = cb;
    entry->cb_arg = cb_arg;
    pthread_mutex_lock(&t->new_timer_lock);
    entry->next = t->new_timer_list;
    t->new_timer_list = entry;
    pthread_cond_signal(&t->new_timer_cond); 
    pthread_mutex_unlock(&t->new_timer_lock);
}

static int co_time_cmp(struct timespec *t1, struct timespec *t2)
{
    if (t1->tv_sec < t2->tv_sec) {
        return -1;
    }else if (t1->tv_sec > t2->tv_sec) {
        return 1;
    }else if (t1->tv_nsec < t2->tv_nsec) {
        return -1;
    }else if (t1->tv_nsec > t2->tv_nsec) {
        return 1;
    }

    return 0;
}

static void emit_timeout(co_timer_t t, struct timespec * now)
{
    co_timer_entry_t cur = NULL;
    if(NULL == t->timer_list) {
        return;
    }

    cur = t->timer_list;
    while(co_time_cmp(&cur->timeout, now)<0) {
        cur->cb(cur->cb_arg);
        cur = cur->next; 
    }
}

static void handle_new_timer(co_timer_t t)
{
    pthread_mutex_lock(&t->new_timer_lock);
    while(NULL != t->new_timer_list) {
        co_timer_entry_t new_timer = t->new_timer_list;
        t->new_timer_list = new_timer->next;
        if(NULL == t->timer_list) {
            t->timer_list = t->new_timer_list;
            t->new_timer_list = t->new_timer_list->next;
            t->timer_list->next = NULL;
            continue;
        }
        co_timer_entry_t e = NULL; 
        for(e=t->timer_list; e!=NULL; e=e->next) {
            if(co_time_cmp(&t->new_timer_list->timeout, &e->timeout)>=0) {
                co_timer_entry_t tmp = t->new_timer_list;
                t->new_timer_list = tmp->next;
                tmp->next = e->next;
                e->next = tmp;
                continue;
            }
        }
    }
    pthread_mutex_unlock(&t->new_timer_lock);
}

void _co_timer_loop(co_timer_t t)
{
    struct timespec now;
    while(1) {
        clock_gettime(CLOCK_REALTIME, &now);
        emit_timeout(t, &now);
        pthread_mutex_lock(&t->new_timer_lock);
        while(NULL == t->new_timer_list) {
            if(NULL == t->timer_list) { //no timer, wait for new timer
                pthread_cond_wait(&t->new_timer_cond, &t->new_timer_lock);
            }else{
                struct timespec *next_timeout;
                int ret;
                next_timeout = &t->timer_list->timeout;
                ret = pthread_cond_timedwait(&t->new_timer_cond, &t->new_timer_lock, next_timeout);
                if(ETIMEDOUT == ret) {
                    break;
                }
            }
        }
        pthread_mutex_unlock(&t->new_timer_lock);
        if(NULL!=t->new_timer_list) { //new timer comes
            handle_new_timer(t);    
        }
    }
}

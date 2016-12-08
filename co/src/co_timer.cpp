#include "co.h"
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/*
 * a module implements timer service.
 */

typedef struct co_timer_entry_st * co_timer_entry_t;
struct co_timer_entry_st {
    co_timer_entry_t next;
    co_timer_entry_t prev;
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

void co_timer_schedule(co_timer_t t, struct timespec *abs_timeout, timeout_callback_t cb, void *cb_arg)
{
    co_timer_entry_t entry = (co_timer_entry_t)malloc(sizeof(struct co_timer_entry_st));
    entry->next = NULL;
    entry->timeout = *abs_timeout;
    entry->cb = cb;
    entry->cb_arg = cb_arg;
    pthread_mutex_lock(&t->new_timer_lock);
    entry->next = t->new_timer_list; 
    t->new_timer_list = entry;
    pthread_cond_signal(&t->new_timer_cond); 
    pthread_mutex_unlock(&t->new_timer_lock);
}

int co_time_cmp(struct timespec *t1, struct timespec *t2)
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

void emit_timeout(co_timer_t t, struct timespec * now)
{
    if (NULL==t->timer_list) {
       return;
    }
    do{
        if(co_time_cmp(&t->timer_list->timeout, now)<0) {
           co_timer_entry_t prev, next, tmp;
           prev = t->timer_list->prev;
           next = t->timer_list->next;
           tmp = t->timer_list;
           printf("before cb\n");
           tmp->cb(tmp->cb_arg);
           printf("after cb\n");
           if (next==t->timer_list) { //only 1 entry in the list
              t->timer_list = NULL;
              free(tmp);
              break;
           }
           prev->next = next;
           next->prev = prev;
           t->timer_list = next;
           free(tmp);
        }else { //timer list is a sorted list. so...
           break;
        }
    }while(NULL!=t->timer_list);
}

void co_timer_handle_new_timer(co_timer_t t)
{
    pthread_mutex_lock(&t->new_timer_lock);
    while(NULL != t->new_timer_list) {
        co_timer_entry_t new_timer = t->new_timer_list;
        t->new_timer_list = new_timer->next;
        if(NULL == t->timer_list) {
            new_timer->next = new_timer;
            new_timer->prev = new_timer;
            t->timer_list = new_timer;
            continue;
        }
        co_timer_entry_t e = t->timer_list; 
        do{
            if(co_time_cmp(&e->timeout, &new_timer->timeout)<0) {
                e = e->next;
                continue;
            }else {
                co_timer_entry_t prev = e->prev;
                prev->next = new_timer;
                new_timer->prev = prev;
                new_timer->next = e;
                e->prev = new_timer;
                new_timer = NULL;
                break;
            }
        }while(e != t->timer_list);
        if (NULL != new_timer) { //new_timer is biggest timeout in ttimer_list
           co_timer_entry_t prev = t->timer_list->prev;
           new_timer->prev = prev;
           new_timer->next = t->timer_list;
           prev->next = new_timer;
           t->timer_list->prev = new_timer;
           new_timer = NULL;
        }
    }
    pthread_mutex_unlock(&t->new_timer_lock);
}

void co_timer_mainloop(co_timer_t t)
{
    struct timespec now;
    while(1) {
        clock_gettime(CLOCK_REALTIME, &now);
        emit_timeout(t, &now);
        pthread_mutex_lock(&t->new_timer_lock);
        while(NULL == t->new_timer_list) {
            if(NULL == t->timer_list) { //no timer, wait for new timer
                printf("wait forever!!!\n");
                pthread_cond_wait(&t->new_timer_cond, &t->new_timer_lock);
            }else{
                struct timespec *next_timeout;
                int ret;
                next_timeout = &t->timer_list->timeout;
                printf("wait for timeout!!!\n");
                ret = pthread_cond_timedwait(&t->new_timer_cond, &t->new_timer_lock, next_timeout);
                printf("back from timedwait! %d %d\n", ETIMEDOUT, ret);
                if(ETIMEDOUT == ret) {
                    break;
                }
            }
        }
        pthread_mutex_unlock(&t->new_timer_lock);
        if(NULL!=t->new_timer_list) { //new timer comes
            co_timer_handle_new_timer(t);    
        }
    }
}

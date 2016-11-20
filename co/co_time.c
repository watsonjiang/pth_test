#include "co.h"
#include <time.h>
#include <pthread.h>
/*
 * a module implements timer service.
 */

struct co_timer_st {
    pthread_mutex_t lock;
    pthread_cond_t  new_timeout_cond;
    int new_timeout_cnt;

};

typedef struct co_timer_st * co_timer_t;


void co_timer_init(co_timer_t t, co_sched_t s)
{

}

void co_timer_schedule(co_timer_t t, struct timeval abs_timeout)
{
    pthread_mutex_lock(&t->lock);
    new_timeout_cnt += 1;     
    pthread_mutex_unlockk(&t->lock);
}

static struct timeval get_next_timeout()
{
   return NULL;
}

static void emit_timeout()
{

}

void _co_timer_loop(co_timer_t t)
{
    while(1) {
        struct timeval now;
        struct timeval next_timeout = get_next_timeout();
        gettimeofday(&now, NULL);
        while( time_cmp(next_timeout, now) < 0) {
             emit_timeout(next_timeout); 
        }
        pthread_mutex_lock(&t->lock);
        int ret;
        while(t->new_timeout_cnt <= 0) {
            ret = pthread_cond_timedwait(&t->new_timeout_cond, &t->lock, next_timeout);
        }
        pthread_mutex_unlock(&t->lock);
        if (ret == ETIMEDOUT) {
            emit_timeout(next_timeout);
        }
 
    }
}

#include "co.h"
#include <time.h>
#include <pthread.h>
#include <string.h>
/*
 * a module implements timer service.
 */

typedef struct co_timeout_entry_st * co_timeout_entry_t;
struct co_timeout_entry_st {
    co_timeout_entry_t next;
    struct timespec timeout;
    co_t coroutine;
}

struct co_timer_wheel_slot_st {
    co_timeout_entry_t head;
};
typedef struct co_timer_wheel_slot_st * co_timer_wheel_slot_t;

struct co_timer_st {
    struct co_timer_wheel_slot_st wheel[60];
    pthread_mutex_t lock;
    pthread_cond_t  new_timeout_cond;
    co_timeout_entry_t new_timeout_list_head;
    co_sched_t sched;    //the scheduler to receive timeout event
};

typedef struct co_timer_st * co_timer_t;


void co_timer_init(co_timer_t *t, co_sched_t s)
{
    co_timer_t tmp = (co_timer_t)malloc(sizeof(struct co_timer_st));
    pthread_mutex_init(&tmp->lock, NULL);
    pthread_cond_init(&tmp->new_timeout_cond, NULL);
    tmp->new_timeout_list_head == NULL;
    tmp->sched = s;
    memset(&tmp->wheel[0], 0, 60 * sizeof(struct co_timer_wheel_slot_st));
    *t = tmp;
}

void co_timer_schedule(co_timer_t t, struct timeval abs_timeout, co_t coroutine)
{
    pthread_mutex_lock(&t->lock);
        
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

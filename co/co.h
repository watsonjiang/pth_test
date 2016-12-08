#ifndef CO_H
#define CO_H
#include <sys/time.h>
#include <ucontext.h>

/* essential values */
#ifndef NULL
#define NULL (void *)0
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

/* bitmask generation */
#define _BIT(n) (1<<(n))

/* thread states */
typedef enum co_state_en {
    CO_STATE_SCHEDULER = 0,         /* the special scheduler thread only       */
    CO_STATE_NEW,                   /* spawned, but still not dispatched       */
    CO_STATE_READY,                 /* ready, waiting to be dispatched         */
    CO_STATE_WAITING,               /* suspended, waiting until event occurred */
    CO_STATE_DEAD                   /* terminated, waiting to be joined        */
} co_state_t;

typedef struct co_event_st * co_event_t;
typedef struct co_st * co_t;
/* thread control block */
struct co_st {
    /* priority queue handling */
    co_t          q_next;               /* next thread in pool                         */
    co_t          q_prev;               /* previous thread in pool                     */
    int            q_prio;               /* (relative) priority of thread when queued   */

    /* standard thread control block ingredients */
    int            prio;                 /* base priority of thread                     */
    char           name[40];             /* name of thread (mainly for debugging)       */
    co_state_t     state;                /* current state indicator for thread          */
   
    /* event handling */
    co_event_t     events;               /* events the tread is waiting for             */

    /* machine context */
    ucontext_t     mctx;                 /* last saved machine context                  */ 
    char          *stack;                /* pointer to thread stack                     */
    unsigned int   stacksize;            /* size of thread stack                        */
    long          *stackguard;           /* stack overflow guard                        */
    void        *(*start_func)(void *);  /* start routine                               */
    void          *start_arg;            /* start argument                              */
 

    int            ref_cnt;              /* refer count                                 */

};

/* return current co */
extern co_t co_get_current_co();

typedef struct sched_st * sched_t;
extern sched_t co_scheduler_create();
extern void* co_schedule_loop(sched_t s);

/* pqueue related */
/* coroutine priority queue */
struct co_pqueue_st {
    co_t q_head;
    int   q_num;
    pthread_mutex_t lock;
    pthread_mutexattr_t lock_attr;
    pthread_cond_t not_empty_cond; 
};
typedef struct co_pqueue_st co_pqueue_t;

#define co_pqueue_elements(q) \
    ((q) == NULL ? (-1) : (q)->q_num)

extern co_t co_pqueue_tail(co_pqueue_t *q);

extern co_t co_pqueue_delmax(co_pqueue_t *q);

extern co_t co_pqueue_walk(co_pqueue_t *q, co_t t, int direction);

/* walk to first thread in queue; O(1) */
#define co_pqueue_head(q) \
    ((q) == NULL ? NULL : (q)->q_head)

/* event related */
/* event status codes */
typedef enum {
    CO_STATUS_PENDING,
    CO_STATUS_OCCURRED,
    CO_STATUS_FAILED
} co_status_t;

/* event structure */
struct co_event_st {
    struct co_event_st *ev_next;
    struct co_event_st *ev_prev;
    co_t coroutine;
    co_status_t ev_status;
    int ev_type;
    void *data;
};
/* event walking directions */
#define CO_WALK_NEXT                _BIT(1)
#define CO_WALK_PREV                _BIT(2)

/* thread priority values */
#define CO_PRIO_MAX                 +5
#define CO_PRIO_STD                  0
#define CO_PRIO_MIN                 -5


/* event subject classes */
#define CO_EVENT_NEW_CO             _BIT(1)
#define CO_EVENT_TIME               _BIT(2)

extern void co_lunch_scheduler(int num);

/* timer related */
typedef struct co_timer_st * co_timer_t;
typedef void (*timeout_callback_t)(void * arg); 
extern void co_timer_schedule(co_timer_t t, struct timespec *abs_timeout, timeout_callback_t cb, void *cb_arg);
#endif

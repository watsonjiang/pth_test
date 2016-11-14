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

/* event walking directions */
#define CO_WALK_NEXT                _BIT(1)
#define CO_WALK_PREV                _BIT(2)

/* thread priority values */
#define CO_PRIO_MAX                 +5
#define CO_PRIO_STD                  0
#define CO_PRIO_MIN                 -5


/* event subject classes */
#define CO_EVENT_TIME               _BIT(1)
#define CO_EVENT_MSG                _BIT(2)

/* event occurange restrictions */
#define CO_UNTIL_OCCURRED           _BIT(11)

/* event structure handling modes */
#define CO_MODE_REUSE               _BIT(20)
#define CO_MODE_CHAIN               _BIT(21)

/* event deallocation types */
enum { CO_FREE_THIS, CO_FREE_ALL };

/* event status codes */
typedef enum {
    CO_STATUS_PENDING,
    CO_STATUS_OCCURRED,
    CO_STATUS_FAILED
} co_status_t;

typedef struct timeval co_time_t;

typedef struct co_event_st * co_event_t;

/* thread states */
typedef enum co_state_en {
    CO_STATE_SCHEDULER = 0,         /* the special scheduler thread only       */
    CO_STATE_NEW,                   /* spawned, but still not dispatched       */
    CO_STATE_READY,                 /* ready, waiting to be dispatched         */
    CO_STATE_WAITING,               /* suspended, waiting until event occurred */
    CO_STATE_DEAD                   /* terminated, waiting to be joined        */
} co_state_t;

typedef struct co_pqueue_st * co_pqueue_t;

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
    int            dispatches;           /* total number of thread dispatches           */
    co_state_t     state;                /* current state indicator for thread          */
   
    /* event handling */
    co_event_t     events;               /* events the tread is waiting for             */

    /* machine context */
    ucontext_t     mctx;                 /* last saved machine context                  */ 
    char          *stack;                /* pointer to thread stack                     */
    unsigned int   stacksize;            /* size of thread stack                        */
    long          *stackguard;           /* stack overflow guard                        */
    int            stackloan;            /* stack type                                  */
    void        *(*start_func)(void *);  /* start routine                               */
    void          *start_arg;            /* start argument                              */

    /* cancellation support */
    int            cancelreq;            /* cancellation request is pending             */
    unsigned int   cancelstate;          /* cancellation state of thread                */

};

/* return current co */
extern co_t co_get_current_co();

#endif

#ifndef CO_H
#define CO_H

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
#define PTH_WALK_NEXT                _BIT(1)
#define PTH_WALK_PREV                _BIT(2)

    /* thread priority values */
#define PTH_PRIO_MAX                 +5
#define PTH_PRIO_STD                  0
#define PTH_PRIO_MIN                 -5


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
    
    char          *stack;                /* pointer to thread stack                     */
    unsigned int   stacksize;            /* size of thread stack                        */
    long          *stackguard;           /* stack overflow guard                        */
    int            stackloan;            /* stack type                                  */
    void        *(*start_func)(void *);  /* start routine                               */
    void          *start_arg;            /* start argument                              */

    /* thread joining */
    int            joinable;             /* whether thread is joinable                  */
    void          *join_arg;             /* joining argument                            */

    /* per-thread specific storage */
    const void   **data_value;           /* thread specific  values                     */
    int            data_count;           /* number of stored values                     */

    /* cancellation support */
    int            cancelreq;            /* cancellation request is pending             */
    unsigned int   cancelstate;          /* cancellation state of thread                */

};

#endif

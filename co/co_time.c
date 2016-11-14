#include "co.h"

#define CO_TIME_NOW  (co_time_t *)(0)
#define CO_TIME_ZERO &co_time_zero
#define CO_TIME(sec,usec) { sec, usec }
#define co_time_equal(t1,t2) \
        (((t1).tv_sec == (t2).tv_sec) && ((t1).tv_usec == (t2).tv_usec))

/* a global variable holding a zero time */
co_time_t co_time_zero = { 0L, 0L };

/* calculate: t1 = t2 */
#if defined(HAVE_GETTIMEOFDAY_ARGS1)
#define __gettimeofday(t) gettimeofday(t)
#else
#define __gettimeofday(t) gettimeofday(t, NULL)
#endif
#define co_time_set(t1,t2) \
    do { \
        if ((t2) == CO_TIME_NOW) \
            __gettimeofday((t1)); \
        else { \
            (t1)->tv_sec  = (t2)->tv_sec; \
            (t1)->tv_usec = (t2)->tv_usec; \
        } \
    } while (0)

/* time value constructor */
co_time_t co_time(long sec, long usec)
{
    co_time_t tv;

    tv.tv_sec  = sec;
    tv.tv_usec = usec;
    return tv;
}

/* timeout value constructor */
co_time_t co_timeout(long sec, long usec)
{
    co_time_t tv;
    co_time_t tvd;

    co_time_set(&tv, CO_TIME_NOW);
    tvd.tv_sec  = sec;
    tvd.tv_usec = usec;
    co_time_add(&tv, &tvd);
    return tv;
}

/* calculate: t1 <=> t2 */
int co_time_cmp(co_time_t *t1, co_time_t *t2)
{
    int rc;

    rc = t1->tv_sec - t2->tv_sec;
    if (rc == 0)
         rc = t1->tv_usec - t2->tv_usec;
    return rc;
}

/* calculate: t1 = t1 + t2 */
#if cpp
#define co_time_add(t1,t2) \
    (t1)->tv_sec  += (t2)->tv_sec; \
    (t1)->tv_usec += (t2)->tv_usec; \
    if ((t1)->tv_usec > 1000000) { \
        (t1)->tv_sec  += 1; \
        (t1)->tv_usec -= 1000000; \
    }
#endif

/* calculate: t1 = t1 - t2 */
#if cpp
#define co_time_sub(t1,t2) \
    (t1)->tv_sec  -= (t2)->tv_sec; \
    (t1)->tv_usec -= (t2)->tv_usec; \
    if ((t1)->tv_usec < 0) { \
        (t1)->tv_sec  -= 1; \
        (t1)->tv_usec += 1000000; \
    }
#endif

/* calculate: t1 = t1 / n */
void co_time_div(co_time_t *t1, int n)
{
    long q, r;

    q = (t1->tv_sec / n);
    r = (((t1->tv_sec % n) * 1000000) / n) + (t1->tv_usec / n);
    if (r > 1000000) {
        q += 1;
        r -= 1000000;
    }
    t1->tv_sec  = q;
    t1->tv_usec = r;
    return;
}

/* calculate: t1 = t1 * n */
void co_time_mul(co_time_t *t1, int n)
{
    t1->tv_sec  *= n;
    t1->tv_usec *= n;
    t1->tv_sec  += (t1->tv_usec / 1000000);
    t1->tv_usec  = (t1->tv_usec % 1000000);
    return;
}

/* convert a time structure into a double value */
double co_time_t2d(co_time_t *t)
{
    double d;

    d = ((double)t->tv_sec*1000000 + (double)t->tv_usec) / 1000000;
    return d;
}

/* convert a time structure into a integer value */
int co_time_t2i(co_time_t *t)
{
    int i;

    i = (t->tv_sec*1000000 + t->tv_usec) / 1000000;
    return i;
}

/* check whether time is positive */
int co_time_pos(co_time_t *t)
{
    if (t->tv_sec > 0 && t->tv_usec > 0)
        return 1;
    else
        return 0;
}


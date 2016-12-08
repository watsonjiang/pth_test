#ifndef CO_TIMER_H
#define CO_TIMER_H

void co_timer_init(co_timer_t *t);

void co_timer_schedule(co_timer_t t, struct timespec *abs_timeout, timeout_callback_t cb, void *cb_arg);

#endif /*CO_TIMER_H*/

#ifndef CO_PQUEUE_H
#define CO_PQUEUE_H
#include "co.h"
/* a priority queue implementation.*/

/* initialize a priority queue; O(1) */
void co_pqueue_init(co_pqueue_t *q);
/* insert thread into priority queue; O(n) */
void co_pqueue_insert(co_pqueue_t *q, int prio, co_t t);
/* remove thread with maximum priority from priority queue; O(1) */
co_t co_pqueue_delmax(co_pqueue_t *q);
/* remove thread from priority queue; O(n) */
void co_pqueue_delete(co_pqueue_t *q, co_t t);
/* determine priority required to favorite a thread; O(1) */
#define co_pqueue_favorite_prio(q) \
    ((q)->q_head != NULL ? (q)->q_head->q_prio + 1 : CO_PRIO_MAX)
/* move a thread inside queue to the top; O(n) */
int co_pqueue_favorite(co_pqueue_t *q, co_t t);
/* increase priority of all(!) threads in queue; O(1) */
void co_pqueue_increase(co_pqueue_t *q);
/* return number of elements in priority queue: O(1) */
#define co_pqueue_elements(q) \
    ((q) == NULL ? (-1) : (q)->q_num)

/* walk to first thread in queue; O(1) */
#define co_pqueue_head(q) \
    ((q) == NULL ? NULL : (q)->q_head)
/* walk to last thread in queue */
co_t co_pqueue_tail(co_pqueue_t *q);
/* walk to next or previous thread in queue; O(1) */
co_t co_pqueue_walk(co_pqueue_t *q, co_t t, int direction);
/* check whether a thread is in a queue; O(n) */
int co_pqueue_contains(co_pqueue_t *q, co_t t);

#endif /*CO_PQUEUE_H*/

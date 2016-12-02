#include "co.h"
#include <stdio.h>
#include <time.h>

void timeout(void * arg)
{
   int n = *(int*)arg; 
   printf("timeout %d!!!\n", n);
   printf("timeout %d!!!\n", n);
   printf("timeout %d!!!\n", n);
   printf("timeout %d!!!\n", n);
   printf("timeout %d!!!\n", n);
   printf("timeout %d!!!\n", n);

}

int main()
{
   co_timer_t timer;
   co_timer_init(&timer);
   struct timespec now;
   clock_gettime(CLOCK_REALTIME, &now);
   now.tv_sec += 3;
   int to1 = 1;
   co_timer_schedule(timer, &now, timeout, (void*)&to1);
   int to2 = 2;
   co_timer_schedule(timer, &now, timeout, (void*)&to2);
   co_timer_mainloop(timer);
   printf("done\n");
   return 0;
}

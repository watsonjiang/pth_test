#include <unittestdef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "co.h"
#include "co_timer.h"

using namespace uts;

extern void co_timer_mainloop(co_timer_t t);

co_timer_t timer;

void timeout(void * arg)
{
   int n = *(int*)arg; 
   printf("timeout %d!!!\n", n);
}

void* foo(void * dummy)
{
   while(1) {
      int to3 = 3;
      sleep(3);
      struct timespec now;
      clock_gettime(CLOCK_REALTIME, &now);
      now.tv_sec += 1;
      printf("schedule timer\n");
      co_timer_schedule(timer, &now, timeout, (void*)&to3);
   } 
}

void
testcase1(uts::TestContext& context)
{
   co_timer_init(&timer);
   struct timespec now;
   clock_gettime(CLOCK_REALTIME, &now);
   now.tv_sec += 3;
   int to1 = 1;
   co_timer_schedule(timer, &now, timeout, (void*)&to1);
   int to2 = 2;
   co_timer_schedule(timer, &now, timeout, (void*)&to2);

   pthread_t th;
   pthread_create(&th, NULL, foo, NULL);

   co_timer_mainloop(timer);
   printf("done\n");
}

DefineTestSuite(CoTimerTest, uts::root());
DefineTestCase(testcase1, CoTimerTest);

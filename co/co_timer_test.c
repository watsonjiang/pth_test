#include "co.h"
#include <stdio.h>

int main()
{
   co_timer_t timer;
   co_timer_init(&timer);
   _co_timer_loop(&timer);
   printf("done\n");
   return 0;
}

#include "co.h"
#include <stdio.h>
#include <pthread.h>
#include <ucontext.h>

void co_ccb_ref_inc(co_t t)
{

}

void co_ccb_ref_dec(co_t t)
{

}

void * test_co_func(void *args)
{
    printf("test_co_func hello\n");
}

int main() {
  
    co_lunch_scheduler(1);
    sleep(1);

    co_create_co(test_co_func, NULL); 

    while(1) {
        sleep(1);
        printf("sleep\n");
    }
}

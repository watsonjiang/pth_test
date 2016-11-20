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
    int * i = (int *)args;
    printf("test_co_func hello %d\n", *i);
}

int main() {
  
    co_lunch_scheduler(1);
    sleep(1);
    int id_list[1000];
    int i;
    for(i=0;i<1000;i++) {
       id_list[i] = i;
       co_create_co(test_co_func, &id_list[i]); 
    }

    while(1) {
        sleep(1);
        printf("sleep\n");
    }
}

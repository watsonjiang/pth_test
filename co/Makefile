# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CFLAGS= -g -fpic

LDFLAGS= -g -fPIC -lpthread  

EX_OBJS=test.o

SO_OBJS=co_pqueue.o \
		co_sched.o \
		co_timer.o

BINS=libco.so test co_timer_test
# project lifecycle target.
all: release

release: $(BINS) 

libco.so: $(SO_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(SO_OBJS) 

test: $(EX_OBJS) libco.so
	$(CC) $(LDFLAGS) -L. -lpthread -lco -o $@ $(EX_OBJS)

co_timer_test: co_timer_test.o libco.so
	$(CC) $(LDFLAGS) -L. -lpthread -lco -o $@ co_timer_test.o

# Other Targets
clean:
	-$(RM) $(SO_OBJS) $(EX_OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean
.SECONDARY:

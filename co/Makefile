# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CFLAGS= -g -fpic

LDFLAGS= -g -fPIC -lpthread  

EX_OBJS=test.o

SO_OBJS=co_pqueue.o \
		co_sched.o \
		co_time.o

BINS=libco.so test
# project lifecycle target.
all: release

release: $(BINS) 

libco.so: $(SO_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(SO_OBJS) 

test: $(EX_OBJS) libco.so
	$(CXX) $(LDFLAGS) -L. -lpthread -lco -o $@ $(EX_OBJS)

# Other Targets
clean:
	-$(RM) $(SO_OBJS) $(EX_OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

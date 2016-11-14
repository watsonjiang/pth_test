# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CFLAGS= -g -fpic

LDFLAGS= -g -fPIC -lpthread  

EX_OBJS=

SO_OBJS=co_pqueue.o \
		co_time.o \
		co_event.o

BINS=libco.so
# project lifecycle target.
all: release

release: $(BINS) 

libco.so: $(SO_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(SO_OBJS) 

#js: $(EX_OBJS)
#	$(CXX) $(LDFLAGS) -L. -ljs -o $@ $(EX_OBJS)

# Other Targets
clean:
	-$(RM) $(SO_OBJS) $(EX_OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

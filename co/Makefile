# make file for co.
# author: watson
# Date: Dec 8, 2016

RM := rm -rf

CXXFLAGS= -g -fpic -I./inc

LDFLAGS= -g -fPIC -lpthread  

SO_OBJS=./src/co_pqueue.o \
		./src/co_sched.o \
		./src/co_timer.o

BINS=libco.so 
# project lifecycle target.
all: release

release: $(BINS) 

libco.so: $(SO_OBJS)
	$(CXX) $(LDFLAGS) -shared -o $@ $(SO_OBJS) 

# Other Targets
clean:
	-$(RM) $(SO_OBJS) $(EX_OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean
.SECONDARY:

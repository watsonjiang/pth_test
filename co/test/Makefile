# make file for co test.
# author: watson
# Date: Dec 8, 2016

RM := rm -rf

CXXFLAGS= -I../inc -I../src -I ../../uts/inc 

OBJS= ./co_timer_test.o 

BINS=./co_test 

all:test 

test: $(BINS)
	@export LD_LIBRARY_PATH=../:../../uts:$$LD_LIBRARY_PATH;\
        for f in $(BINS); do echo "Invoking: $$f"; $$f; done

co_test: $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking:  C++ Linker'
	$(CXX) -L../ -lco -L../../uts -luts -o "$@" $^ 
	@echo 'Finished building target: $@'
	@echo ' '


# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

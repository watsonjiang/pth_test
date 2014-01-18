# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS= -I../inc -I../src -I../../pth-2.0.7 -fpic

LDFLAGS= -L../ -L../../pth-2.0.7 -lpth -luts

OBJS=./uts_test.o 

BINS=./uts_test 

all:test 

test: $(BINS)
	@export LD_LIBRARY_PATH=../../pth-2.0.7/.libs:../:$$LD_LIBRARY_PATH;\
        for f in $(BINS); do echo "Invoking: $$f"; $$f; done

uts_test: uts_test.o
	@echo 'Building target: $@'
	@echo 'Invoking:  C++ Linker'
	$(CXX) $(LDFLAGS) -o "$@" $^ 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS= -I../inc -I../src -I../../pth-2.0.7 -I ../../uts/inc -I ../../tracing/inc -fpic

OBJS= palscriptruntime_test.o
#      paltime_test.o \
#      palscript_test.o \
#      palnumber_test.o \

BINS=js_test 

all:test 

test: $(BINS)
	@export LD_LIBRARY_PATH=../../pth-2.0.7/.libs:../:../../uts:../../tracing:$$LD_LIBRARY_PATH;\
        for f in $(BINS); do echo "Invoking: $$f"; $$f; done

js_test: $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking:  C++ Linker'
	$(CXX) -L../ -L../../pth-2.0.7 -L../../uts -L../../tracing -lpth -ljs -luts -ltracing -lrt -o "$@" $^ 
	@echo 'Finished building target: $@'
	@echo ' '


# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS= -I./inc -fpic

OBJS=./src/functiontracer.o \
     ./src/tracemanager.o \
     ./src/tracepeer.o \
     ./src/traceappender.o \
     ./src/tracing.o

BINS=./libtracing.so
# project lifecycle target.
# build->test->release
all: release

release: $(BINS) 

libtracing.so: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC linker'
	$(CXX) -fPIC -shared -o $@ $(OBJS) 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

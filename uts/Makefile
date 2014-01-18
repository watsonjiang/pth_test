# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS= -I./inc -I../pth -fpic

LDFLAGS= -fPIC -shared -L ../pth/.libs -lpth

OBJS=./src/main.o \
     ./src/testqualifier.o \
     ./src/unittest.o \
     ./src/unittestdef.o \
     ./src/testrunner.o

BINS=./libuts.so
# project lifecycle target.
# build->test->release
all: release

release: $(BINS) 

libuts.so: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC linker'
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

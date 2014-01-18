# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS=-g -I. \
         -I../pth \
         -I../js/src \
         -fpic -DLINUX

LDFLAGS=-g -fPIC -L ../js -ljs \
        -L../pth -lpth 

OBJS=ScriptObjectEchoServer.o \
     js.o

BINS=js
# project lifecycle target.
all: release

release: $(BINS) 

js: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) 

test: release
	LD_LIBRARY_PATH=../js:../pth;./js main.js

# Other Targets
clean:
	-$(RM) $(OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:

# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

BINS=libjs.so js

# project lifecycle target.
all: 
	cd src && make
	cp src/libjs.so .
	cp src/js .

# Other Targets
clean:
	cd src && make clean
	-$(RM) $(BINS)

.PHONY: all clean test
.SECONDARY:

# Generic make rules for alan compiler
# Set:
#	COMPILER : which command to run the C compiler
#	LINKER : which command to run the linker
#	OS_FLAGS : what flags must be passed to both compiler and linker
#	EXTRA_COMPILER_FLAGS : what extra flags to pass to the compiler
#	EXTRA_LINKER_FLAGS : what extra flags to pass to the linker
# Then include this file in your Makefile, and you are done

CC = $(COMPILER)
CFLAGS = $(INCLUDE) $(OS_FLAGS) $(EXTRA_COMPILER_FLAGS)

LINK = $(LINKER)
LINKFLAGS = $(EXTRA_LINKER_FLAGS)


# Include common list of sources
include sources.mk

help:
	@echo "This the generic Makefile for the Alan compiler"
	@echo "The following targets are available:"
	@echo
	@echo "    make all  - Build and test the alan compiler"
	@echo "    make alan - Build the alan compiler"
	@echo "    make unit - Build and run the unit tests"
	@echo "    make clean"

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main target
#
.PHONY: all
all: unit alan test


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main target
#
alan: $(ALANOBJS)
	$(LINK) -o alan $(ALANOBJS) $(LINKFLAGS)
	-@if ! test -d ../bin; then mkdir ../bin 2> /dev/null ; fi
	cp alan ../bin


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#

.PHONY: unit
unit: unittests
	@./unittests

unittests : $(UNITOBJS)
	$(LINK) -o unittests $(UNITOBJS) $(LINKFLAGS)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Run all unit tests!
#
test: unit
	-cd testing; ../../bin/regr

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Clean
#
.PHONY: clean
clean:
	-rm *.o


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
include dependencies.mk


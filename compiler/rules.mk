# Generic make rules for alan compiler
# Set:
#	COMPILER : which command to run the C compiler
#	LINKER : which command to run the linker
#	OS_FLAGS : what flags must be passed to both compiler and linker
#	INCLUDES : directives to include the required directories
#	EXTRA_COMPILER_FLAGS : what extra flags to pass to the compiler
#	EXTRA_LINKER_FLAGS : what extra flags to pass to the linker
#	DEPENDFLAGS: flags to the dependency preprocessor
# Then include this file in your Makefile, and you are done

CC = $(COMPILER)
CFLAGS = $(INCLUDES) -I../interpreter $(OS_FLAGS) $(EXTRA_COMPILER_FLAGS)

LINK = $(LINKER)
LINKFLAGS = $(OS_FLAGS) $(EXTRA_LINKER_FLAGS)


# Include common list of sources
include sources.mk

.PHONY: help
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
alan: checkTarget $(ALANOBJS)
	$(LINK) -o alan $(ALANOBJS) $(LINKFLAGS)
	-@if ! test -d ../bin; then mkdir ../bin 2> /dev/null ; fi
	cp alan ../bin/alan.exe


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
# Run all tests!
#
.PHONY: test
test: unit
	cd testing ; ../../bin/jregr

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Clean
#
.PHONY: clean
clean:
	-rm *.o


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
#######################################################################
ARCH	= $(strip $(CFLAGS))
.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo "Removing objects for '`cat .arch`'" ; \
		echo "Re-building      for '$(ARCH)'..." ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi

###################################################################
#
# Version number file generation
#
alan.version.h : ../alan.version.h
	cp ../alan.version.h .

alan.version.c:  ../alan.version.c
	cp ../alan.version.c .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h: ../alan.version

../alan.version:
	cd ..; venum alan time


###################################################################
#
# Run all tests!
# No tests except unit tests are available
#
.PHONY: test
test: unit


###################################################################
# Dependencies are generated only for command line case
# Dependencies on libraries are only in the way for other cases,
# so special dependencies that we need are set explicitly below
#
CPP	= gcc
depend:
	@-for f in $(UNITTESTSSRCS) $(ALANSRCS) ; \
	  do \
	  $(CPP) $(DEPENDFLAGS) $(DEPENDINCLUDES) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk


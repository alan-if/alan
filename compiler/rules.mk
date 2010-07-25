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
# Alan command line compiler
#
ALANOBJDIR = .alan
ALANOBJECTS = $(addprefix $(ALANOBJDIR)/,${ALANSRCS:.c=.o})
-include $(ALANOBJECTS:.o=.d)
$(ALANOBJECTS): $(ALANOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(ALANOBJDIR):
	@mkdir $(ALANOBJDIR)

alan: $(ALANOBJDIR) $(ALANOBJECTS)
	$(LINK) -o alan $(ALANOBJECTS) $(LINKFLAGS)
	-@if ! test -d ../bin; then mkdir ../bin 2> /dev/null ; fi
	cp alan ../bin/alan.exe


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#
.PHONY: unit
unit: unittests
	@./unittests

UNITTESTSOBJDIR = .unittests
UNITTESTSOBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTSSRCS:.c=.o})
-include $(ALANOBJECTS:.o=.d)
$(UNITTESTSOBJECTS): $(UNITTESTSOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(UNITTESTSOBJDIR):
	@mkdir $(UNITTESTSOBJDIR)

unittests: $(UNITTESTSOBJDIR) $(UNITTESTSOBJECTS)
	$(LINK) -o unittests $(UNITTESTSOBJECTS) $(LINKFLAGS)


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Run all tests!
#
.PHONY: test
test: unit
	cd testing ; ../../bin/jregr -noansi


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Clean
#
.PHONY: clean
clean:
	-rm *.o .*/*.o


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

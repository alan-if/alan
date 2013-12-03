# Generic make rules for alan compiler
# Set:
#	COMPILER : which command to run the C compiler
#	LINKER : which command to run the linker
#	OSFLAGS : what flags must be passed to both compiler and linker
#	INCLUDES : directives to include the required directories
#	EXTRA_COMPILER_FLAGS : what extra flags to pass to the compiler
#	EXTRA_LINKER_FLAGS : what extra flags to pass to the linker

ifneq ($(EMACS),)
JREGROUTPUT = -noansi
else
UNITOUTPUT ?= -c
endif

CC = $(COMPILER)
CFLAGS = $(INCLUDES) -I../interpreter $(OSFLAGS) $(EXTRA_COMPILER_FLAGS)

LINK = $(LINKER)
LINKFLAGS = $(OSFLAGS) $(EXTRA_LINKER_FLAGS)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main target to do everything
#
.PHONY: all
all: unit build

# Target to just build
.PHONY: build
build: alan

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Run all tests!
#

.PHONY: test
test:
	@cd ..;bin/jregr -bin bin -dir compiler/testing $(JREGROUTPUT)
	@cd ..;bin/jregr -bin bin -dir compiler/testing/positions $(JREGROUTPUT)

# Clean
.PHONY: clean
clean:
	-rm *.o .*/*.o .*/*.d

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Alan command line compiler
#
ALANOBJDIR = .alan
ALANOBJECTS = $(addprefix $(ALANOBJDIR)/,${ALANSRCS:.c=.o}) $(ALANOBJDIR)/alan.version.o
-include $(ALANOBJECTS:.o=.d)
$(ALANOBJECTS): $(ALANOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(ALANOBJDIR):
	@mkdir $(ALANOBJDIR)

alan: $(ALANOBJDIR) $(ALANOBJECTS)
	$(LINK) -o alan $(ALANOBJECTS) $(LINKFLAGS)
	-@if ! test -d ../bin; then mkdir ../bin 2> /dev/null ; fi
	cp alan ../bin/alan


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#
.PHONY: unit
unit: cgreenrunnertests isolated_unittests


UNITTESTSOBJDIR = .unittests
UNITTESTSOBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTSSRCS:.c=.o}) $(UNITTESTSOBJDIR)/alan.version.o
UNITTESTSDLLOBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTSDLLSRCS:.c=.o}) $(UNITTESTSOBJDIR)/alan.version.o

# Dependencies, if they don't exist yet
-include $(UNITTESTSOBJECTS:.o=.d)

# Rule to compile objects to subdirectory
.PRECIOUS: $(UNITTESTSOBJDIR)/%.o
$(UNITTESTSOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

# Create directory if it doesn't exist
$(UNITTESTSOBJDIR):
	@mkdir $(UNITTESTSOBJDIR)

unittests: CFLAGS += $(CGREENINCLUDE)
unittests: LIBS = $(CGREENLIB)
unittests: $(UNITTESTSOBJDIR) $(UNITTESTSOBJECTS)
	$(LINK) -o unittests $(UNITTESTSOBJECTS) $(LINKFLAGS) $(LIBS)

unittests.dll: CFLAGS += $(CGREENINCLUDE)
unittests.dll: LIBS = $(CGREENLIB)
unittests.dll: $(UNITTESTSOBJDIR) $(UNITTESTSOBJECTS)
	$(LINK) -shared -o $@ $(UNITTESTSDLLOBJECTS) $(LINKFLAGS) $(LIBS)

# ... that can be run with the cgreen runner
cgreenrunnertests: CFLAGS += $(CGREENINCLUDE)
cgreenrunnertests: LIBS = $(CGREENLIB)
cgreenrunnertests: unittests.dll
ifeq ($(shell uname), Darwin)
	arch -i386 cgreen-runner $^ --suite compiler_unit_tests $(UNITOUTPUT)
else
	cgreen-runner ./$^ --suite compiler_unit_tests $(UNITOUTPUT)
endif

# Here we try to build a runnable DLL for each module where it can be 
# tested in total isolation (with everything else mocked away,
# except lists.c and memory.c)

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTSOBJDIR)/, $(addsuffix .o, lists util lmList options))

# A test .dll for a module is built from its .o and the _test.o (and some extras)
$(UNITTESTSOBJDIR)/%_tests.dll: $(UNITTESTSOBJDIR)/%.o $(UNITTESTSOBJDIR)/%_tests.o $(ISOLATED_UNITTESTS_EXTRA_OBJS)
	$(LINK) -shared -Wl,--no-undefined -o $@ $^ $(LINKFLAGS) $(LIBS)

ISOLATED_UNITTESTS_DLLS = $(addprefix $(UNITTESTSOBJDIR)/,$(patsubst %,%_tests.dll,$(MODULES_WITH_ISOLATED_UNITTESTS)))

# Then run all _tests.dll's with the cgreen-runner
isolated_unittests: CFLAGS += $(CGREENINCLUDE)
isolated_unittests: LIBS = $(CGREENLIB)
isolated_unittests: $(UNITTESTSOBJDIR) $(ISOLATED_UNITTESTS_DLLS)
ifeq ($(shell uname), Darwin)
	@for f in $(ISOLATED_UNITTESTS_DLLS) ; do \
		arch -i386 cgreen-runner $$f --suite Compiler $(UNITOUTPUT) ; \
	done
else
	@for f in $(ISOLATED_UNITTESTS_DLLS) ; do \
		cgreen-runner $$f --suite Compiler $(UNITOUTPUT) ; \
	done
endif

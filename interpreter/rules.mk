#######################################################################
# This is a set of common rules for building Alan Interpreters
# It is included from Makefile to complement rules and settings
# specific to the host it is run on, set in such files as
# Makefile.thoni, Makefile.cygwin, Makefile.Darwin, also included from
# Makefile
# This file will build a standard command line arun and all unittests

BUILD := $(shell if [ -f ../BUILD_NUMBER ] ; then cat ../BUILD_NUMBER; else echo 0; fi)

CFLAGS	= $(COMPILEFLAGS) $(EXTRA_COMPILER_FLAGS) $(WARNINGFLAGS) -DBUILD=$(BUILD) $(OSFLAGS)
LDFLAGS = $(LINKFLAGS) $(EXTRA_LINKER_FLAGS) $(OSFLAGS)

#######################################################################
# Standard console Arun
ARUNOBJDIR = .arun
ARUNOBJECTS = $(addprefix $(ARUNOBJDIR)/,${ARUNSRCS:.c=.o}) $(ARUNOBJDIR)/alan.version.o

# Dependencies
-include $(ARUNOBJECTS:.o=.d)

# Rule to compile objects to subdirectory
$(ARUNOBJECTS): $(ARUNOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(ARUNOBJDIR):
	@mkdir $(ARUNOBJDIR)

arun: $(ARUNOBJDIR) $(ARUNOBJECTS)
	$(LINK) -o $@ $(LDFLAGS) $(ARUNOBJECTS) $(LIBS)
	cp $@ ../bin/

#######################################################################
# Standard console Arun with gcov
gcov: EXTRA_COMPILER_FLAGS = -fprofile-arcs -ftest-coverage
gcov: EXTRA_LINKER_FLAGS = -fprofile-arcs -ftest-coverage
GCOVOBJDIR = .gcov
GCOVOBJECTS = $(addprefix $(GCOVOBJDIR)/,${ARUNSRCS:.c=.o}) $(GCOVOBJDIR)/alan.version.o

# Dependencies
-include $(GCOVOBJECTS:.o=.d)

# Rule to compile objects to subdirectory
$(GCOVOBJECTS): $(GCOVOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(GCOVOBJDIR):
	@mkdir $(GCOVOBJDIR)

gcov: $(GCOVOBJDIR) $(GCOVOBJECTS)
	$(LINK) -o $@ $(LDFLAGS) $(GCOVOBJECTS) $(LIBS)
	cp $@ ../bin/

#######################################################################
#
# CGreen unit tests
#
# Define
#	CGREEN to something ("yes") to build with cgreen unit tests
#	CGREENINCLUDE so that #include "cgreen/cgreen.h" works if needed
#	CGREENLIB to something to link with (e.g. -lcgreen)
UNITTESTSOBJDIR = .unittests
UNITTESTS_USING_MAIN_OBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTS_USING_MAIN_SRCS:.c=.o}) $(UNITTESTSOBJDIR)/alan.version.o
UNITTESTS_USING_RUNNER_OBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTS_USING_RUNNER_SRCS:.c=.o}) $(UNITTESTSOBJDIR)/alan.version.o
UNITTESTS_ALL_OBJECTS = $(addprefix $(UNITTESTSOBJDIR)/,${UNITTESTS_ALL_SRCS:.c=.o}) $(UNITTESTSOBJDIR)/alan.version.o

# Dependencies, if they don't exist yet
-include $(UNITTESTS_USING_MAIN_OBJECTS:.o=.d)

# Rule to compile objects to subdirectory
$(UNITTESTSOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

# Create directory if it doesn't exist
$(UNITTESTSOBJDIR):
	@mkdir $(UNITTESTSOBJDIR)

unittests: CFLAGS += $(CGREENINCLUDE)
unittests: LIBS = $(CGREENLIB)
unittests: $(UNITTESTSOBJDIR) $(UNITTESTS_USING_MAIN_OBJECTS) add_unittests.include
	$(LINK) -o $@ $(LDFLAGS) $(UNITTESTS_USING_MAIN_OBJECTS) $(LIBS)

unit_tests: unittests
	@./unittests $(UNITOUTPUT)

# Build the DLL...
unittests.dll: LIBS = $(CGREENLIB)
unittests.dll: $(UNITTESTSOBJDIR) $(UNITTESTS_USING_RUNNER_OBJECTS) sources.mk
	$(LINK) -shared -o $@ $(LDFLAGS) $(UNITTESTS_USING_RUNNER_OBJECTS) $(LINKFLAGS) $(LIBS)

# ... that can be run with the cgreen runner
cgreenrunnertests: CFLAGS += $(CGREENINCLUDE)
cgreenrunnertests: LIBS = $(CGREENLIB) $(ALLOCLIBS)
cgreenrunnertests: unittests.dll
ifeq ($(shell uname), Darwin)
	arch -i386 cgreen-runner $^ --suite Interpreter $(UNITOUTPUT)
else
	cgreen-runner ./$^ --suite Interpreter $(UNITOUTPUT)
endif

# To make sure that all modules are included when we run the ones that
# are to be run in the "old fashion" way with a main program that
# collects all the tests. This means that we must remember to manually
# add every new test (and change name in more than one place). So to
# make some automation of this until they have all migrate to the
# reflective runner, here we generate a list of those modules in a
# fashion that the main program can include. And pow, the correct
# modules are called and collected.
add_unittests.include : sources.mk
	-@rm $@
	for f in $(MODULES_WITH_UNITTESTS_USING_MAIN:.c=) ; do \
		echo "    ADD_UNITTESTS_FOR($$f);" >> $@ ; \
	done


# Here we try to build a runnable DLL for each module where it can be 
# tested in total isolation (with everything else mocked away,
# except lists.c and memory.c)

# A test .dll for a module is built from its .o and the _test.o (and some extras)
$(UNITTESTSOBJDIR)/%_tests.dll: $(UNITTESTSOBJDIR)/%.o $(UNITTESTSOBJDIR)/%_tests.o $(UNITTESTSOBJDIR)/lists.o $(UNITTESTSOBJDIR)/memory.o
	$(LINK) -shared -o $@ $^ $(LDFLAGS) $(LIBS)

ISOLATED_UNITTESTS_DLLS = $(addprefix $(UNITTESTSOBJDIR)/,$(patsubst %,%_tests.dll,$(MODULES_WITH_ISOLATED_UNITTESTS)))

# Then run all _tests.dll's with the cgreen-runner
isolated_unittests: CC = gcc
isolated_unittests: LINK = gcc
isolated_unittests: CFLAGS += $(CGREENINCLUDE)
isolated_unittests: LIBS = $(CGREENLIB)
isolated_unittests: $(UNITTESTSOBJDIR) $(ISOLATED_UNITTESTS_DLLS)
ifeq ($(shell uname), Darwin)
	@for f in $(ISOLATED_UNITTESTS_DLLS) ; do \
		arch -i386 cgreen-runner $$f --suite Interpreter $(UNITOUTPUT) ; \
	done
else
	@for f in $(ISOLATED_UNITTESTS_DLLS) ; do \
		cgreen-runner $$f --suite Interpreter $(UNITOUTPUT) ; \
	done
endif

.PHONY: unit
ifneq ($(CGREEN),yes)
unit:
	echo "No unit tests run, cgreen not available"
else
unit: cgreenrunnertests isolated_unittests
endif

#######################################################################
.PHONY: clean
clean:
	-rm *.o .*/*.o .*/*.d


###################################################################
#
# Run all tests!
# No tests except unit tests are available
# Interpreter is tested through the regressions tests
.PHONY: test
test: unit


# Extra dependencies for WinGLK case, really needed? How to make them work in subdirs?
readline.o : resources.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h main.h \
  glkstart.h glkio.h resources.h utils.h
glkio.o: glkio.c glkio.h

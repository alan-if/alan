#######################################################################
# This is a set of common rules for building Alan Interpreters
# It is included from Makefile to complement rules and settings
# specific to the host it is run on, set in such files as
# Makefile.thoni, Makefile.cygwin, Makefile.Darwin, also included from
# Makefile
# This file will build a standard command line arun and all unittests
ifneq ($(EMACS),)
JREGROUTPUT = -noansi
endif

# Build designations:
#   BUILD includes a dash, if no-empty, so can be used with $(VERSION)$(BUILD)
#   BUILDNUMBER is just the number
#   BUILDNAME is "Build"$(BUILDNUMBER), e.g. "Build1667"
BUILD_NUMBER_FILE = $(wildcard ../BUILD_NUMBER)
ifeq ($(BUILD_NUMBER_FILE),)
  BUILD:=
  BUILDNUMBER:=
  BUILDNAME:= 
else
  BUILD_FILE_CONTENT := $(shell cat $(BUILD_NUMBER_FILE))
  BUILD:= -$(BUILD_FILE_CONTENT)
  BUILDNUMBER:= $(BUILD_FILE_CONTENT)
  BUILDNAME:=Build$(BUILDNUMBER)
endif

CC = $(COMPILER)
CFLAGS	= $(COMPILEFLAGS) $(EXTRA_COMPILER_FLAGS) -DBUILD=$(BUILD) $(OSFLAGS) $(ARCHFLAGS) -MMD

LINK = $(LINKER)
LDFLAGS = $(LINKFLAGS) $(EXTRA_LINKER_FLAGS) $(OSFLAGS) $(ARCHFLAGS)

v:
	echo $(BUILD)
	echo $(BUILDNUMBER)
	echo $(BUILDNAME)
	echo $(VERSION)


# Default top rule if platform specific makefile doesn't add a default
# that is found before this
all: unit arun

#######################################################################
build: arun

#######################################################################
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
# Interpreter is tested through the regressions tests
UNITOUTPUT ?= -c

.PHONY: test
test:
	@cd ..;java -jar bin/jregr.jar -bin bin -dir regression $(JREGROUTPUT)
	@cd ..;java -jar bin/jregr.jar -bin bin -dir regression/versions/interpreter $(JREGROUTPUT)
	@cd ..;java -jar bin/jregr.jar -bin bin -dir regression/tracing $(JREGROUTPUT)
	@cd ..;java -jar bin/jregr.jar -bin bin -dir regression/saving $(JREGROUTPUT)
	@cd ..;java -jar bin/jregr.jar -dir regression/restore $(JREGROUTPUT)		# Uses sh not the executables

#######################################################################
# Standard console Arun
ARUNOBJDIR = .arun
ARUNOBJECTS = $(addprefix $(ARUNOBJDIR)/,${ARUNSRCS:.c=.o}) $(ARUNOBJDIR)/alan.version.o

# Dependencies
-include $(ARUNOBJECTS:.o=.d)

# Rule to compile objects to subdirectory
$(ARUNOBJECTS): $(ARUNOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(ARUNOBJDIR):
	@mkdir $(ARUNOBJDIR)

arun: $(ARUNOBJDIR) $(ARUNOBJECTS)
	$(LINK) -o $@ $(LDFLAGS) $(ARUNOBJECTS) $(LIBS)
	cp -f $@ ../bin/

#######################################################################
# Settings for TermGLK => glkarun
TERMGLKROOT ?= $(GLKLIBROOT)/glkterm
TERMGLKDEFS = -DHAVE_GLK
TERMGLKINCLUDE ?= -I$(TERMGLKROOT)
TERMGLKLIB = -L$(TERMGLKROOT) -lglkterm

glkarun: EXTRA_COMPILER_FLAGS = $(TERMGLKINCLUDE) $(TERMGLKDEFS)
glkarun: LIBS = $(TERMGLKLIB) $(CURSESLIB)

GLKARUNOBJDIR = .glkarun
GLKARUNOBJECTS = $(addprefix $(GLKARUNOBJDIR)/,${GLKARUNSRCS:.c=.o}) $(GLKARUNOBJDIR)/alan.version.o
-include $(GLKARUNOBJECTS:.o=.d)
$(GLKARUNOBJECTS): $(GLKARUNOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(GLKARUNOBJDIR):
	@mkdir $(GLKARUNOBJDIR)

glkarun: $(GLKARUNOBJDIR) $(GLKARUNOBJECTS)
	$(LINK) -o $@ $(LDFLAGS) $(GLKARUNOBJECTS) $(LIBS)
	cp -f $@ ../bin/


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
	$(CC) $(CFLAGS) -o $@ -c $<
$(UNITTESTSOBJDIR)/%_tests.o: %_tests.c
	$(CC) $(CFLAGS) -o $@ -c $<

# Create directory if it doesn't exist
$(UNITTESTSOBJDIR):
	@mkdir $(UNITTESTSOBJDIR)

# Build the DLL...
unittests.dll: LIBS = $(CGREENLIB)
unittests.dll: $(UNITTESTSOBJDIR) $(UNITTESTS_USING_RUNNER_OBJECTS) sources.mk
	$(LINK) -shared -o $@ $(UNITTESTS_USING_RUNNER_OBJECTS) $(LDFLAGS) $(LIBS)

# ... that can be run with the cgreen runner
cgreenrunnertests: CFLAGS += $(CGREENINCLUDE)
cgreenrunnertests: LIBS = $(CGREENLIB)
cgreenrunnertests: unittests.dll
ifeq ($(OS), Darwin)
	arch -i386 cgreen-runner $^ --suite interpreter_unit_tests $(UNITOUTPUT)
else
	cgreen-runner ./$^ --suite interpreter_unit_tests $(UNITOUTPUT)
endif

# Here we try to build a runnable DLL for each module where it can be 
# tested in total isolation (with everything else mocked away,
# except lists.c and memory.c)
ISOLATED_UNITTESTS_EXTRA_MODULES = memory options lists sysdep

-include $(addprefix $(UNITTESTSOBJDIR)/,$(patsubst %,%.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))
-include $(addprefix $(UNITTESTSOBJDIR)/,$(patsubst %,%_tests.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTSOBJDIR)/, $(addsuffix .o, $(ISOLATED_UNITTESTS_EXTRA_MODULES)))

# A test .dll for a module is built from its .o and the _test.o (and some extras)
$(UNITTESTSOBJDIR)/%_tests.dll: $(UNITTESTSOBJDIR)/%.o $(UNITTESTSOBJDIR)/%_tests.o
	$(LINK) -shared -o $@ $(sort $(ISOLATED_UNITTESTS_EXTRA_OBJS) $^) $(LDFLAGS) $(LIBS)

ISOLATED_UNITTESTS_DLLS = $(addprefix $(UNITTESTSOBJDIR)/,$(patsubst %,%_tests.dll,$(MODULES_WITH_ISOLATED_UNITTESTS)))

# Then run all _tests.dll's with the cgreen-runner
isolated_unittests: CFLAGS += $(CGREENINCLUDE)
isolated_unittests: LIBS = $(CGREENLIB)
isolated_unittests: $(UNITTESTSOBJDIR) $(ISOLATED_UNITTESTS_EXTRA_OBJS) $(ISOLATED_UNITTESTS_DLLS)
ifeq ($(OS), Darwin)
	arch -i386 cgreen-runner $$f --suite Interpreter $(UNITOUTPUT) $(ISOLATED_UNITTESTS_DLLS)
else
	cgreen-runner $$f --suite Interpreter $(UNITOUTPUT) $(ISOLATED_UNITTESTS_DLLS)
endif


# Extra dependencies for WinGLK case, really needed? How to make them work in subdirs?
readline.o : resources.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h main.h \
  glkstart.h glkio.h resources.h utils.h
glkio.o: glkio.c glkio.h

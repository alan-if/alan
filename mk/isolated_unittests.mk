######################################################################
include ../mk/include_guard.mk
#
# Common rules to build cgreen unittests as isolated shared libs
#
# Requrires:
#   UNITTESTS_OBJDIR: directory in which to store .o and .d 
#   MODULES_WITH_ISOLATED_UNITTESTS: basename of .c/_tests.c files for the module
#   ISOLATED_UNITTESTS_EXTRA_MODULES: basename of extra modules to link with
#   SUITE: name of suite
#   LDLIBS: if case you need other libraries when linking except -lcgreen -lm

######################################################################
# Do we even have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

.SECONDARY:

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTS_OBJDIR)/, $(addsuffix .o, $(ISOLATED_UNITTESTS_EXTRA_MODULES)))

# Include depencency information for modules and their tests
-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))
-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTS_OBJDIR)/, $(addsuffix .o, $(ISOLATED_UNITTESTS_EXTRA_MODULES)))


# A test lib for a module is built from its .o and the _test.o (and some extras)
$(UNITTESTS_OBJDIR)/%_tests.$(SOEXTENSION): $(UNITTESTS_OBJDIR)/%.o $(UNITTESTS_OBJDIR)/%_tests.o $(ISOLATED_UNITTESTS_EXTRA_OBJS)
	$(LINK) -shared -o $@ $(sort $(ISOLATED_UNITTESTS_EXTRA_OBJS) $^) -lcgreen $(LDLIBS)

ISOLATED_UNITTESTS_LIBS = $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.$(SOEXTENSION),$(MODULES_WITH_ISOLATED_UNITTESTS)))

# Build all isolated unittest shared libraries (required by coverage target)
isolated_unittests_build: $(UNITTESTS_OBJDIR) $(ISOLATED_UNITTESTS_LIBS)

# Then run all _tests.$(SOEXTENSION)'s with the cgreen-runner
isolated_unittests: isolated_unittests_build
	cgreen-runner --suite $(SUITE) $(UNITOUTPUT) $(ISOLATED_UNITTESTS_LIBS)

# Rules to compile objects to subdirectory
$(UNITTESTS_OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
$(UNITTESTS_OBJDIR)/%_tests.o: %_tests.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(UNITTESTS_OBJDIR)/%.o: | $(UNITTESTS_OBJDIR)

# Create directory if it doesn't exist
$(UNITTESTS_OBJDIR):
	@mkdir $(UNITTESTS_OBJDIR)

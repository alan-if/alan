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

.SECONDARY:

# Include depencency information for modules and their tests
-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))
-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTS_OBJDIR)/, $(addsuffix .o, $(ISOLATED_UNITTESTS_EXTRA_MODULES)))
ISOLATED_UNITTESTS_LIBS = $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.$(SOEXTENSION),$(MODULES_WITH_ISOLATED_UNITTESTS)))

# An isolated test lib for a module is built from <module>.o and <module>_test.o (and some extras)
$(UNITTESTS_OBJDIR)/%_tests.$(SOEXTENSION): $(UNITTESTS_OBJDIR)/%.o $(UNITTESTS_OBJDIR)/%_tests.o $(ISOLATED_UNITTESTS_EXTRA_OBJS)
	$(LINK) -shared -o $@ $(sort $(ISOLATED_UNITTESTS_EXTRA_OBJS) $^) $(LDLIBS) -lcgreen

# Then run all _tests.$(SOEXTENSION)'s with the cgreen-runner
# Clients need to prevent running this if Cgreen is not available ("ifeq ($(CGREEN),)")
isolated_unittests: $(ISOLATED_UNITTESTS_LIBS)
	$(CGREEN) --suite $(SUITE) $(UNITOUTPUT) $(ISOLATED_UNITTESTS_LIBS)

# Rules to compile objects to subdirectory
$(UNITTESTS_OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(UNITTESTS_OBJDIR)/%_tests.o: %_tests.c
	$(CC) $(CFLAGS) -o $@ -c $<


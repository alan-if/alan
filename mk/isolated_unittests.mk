######################################################################
include ../mk/include_guard.mk
#
# Common rules to build cgreen unittests as isolated shared libs
#
# Requrires:
#   UNITTESTS_OBJDIR: directory in which to store .o and .d 
#   MODULES_WITH_ISOLATED_UNITTESTS: basename of .c/_tests.c files for the module
#   SUITE: name of suite
#   LDLIBS: if case you need other libraries when linking except -lcgreen -lm

######################################################################
# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))
-include $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.d,$(MODULES_WITH_ISOLATED_UNITTESTS)))

ISOLATED_UNITTESTS_EXTRA_OBJS = $(addprefix $(UNITTESTS_OBJDIR)/, $(addsuffix .o, $(ISOLATED_UNITTESTS_EXTRA_MODULES)))


# A test lib for a module is built from its .o and the _test.o (and some extras)
$(UNITTESTS_OBJDIR)/%_tests.$(SOEXTENSION): $(UNITTESTS_OBJDIR)/%.o $(UNITTESTS_OBJDIR)/%_tests.o
	$(LINK) -shared -o $@ $^ -lcgreen -lm $(LDLIBS)

ISOLATED_UNITTESTS_LIBS = $(addprefix $(UNITTESTS_OBJDIR)/,$(patsubst %,%_tests.$(SOEXTENSION),$(MODULES_WITH_ISOLATED_UNITTESTS)))

# Then run all _tests.$(SOEXTENSION)'s with the cgreen-runner
isolated_unittests: CFLAGS += $(CGREENINCLUDE)
isolated_unittests: LDLIBS += $(CGREENLINKLIB)
isolated_unittests: $(UNITTESTS_OBJDIR) $(ISOLATED_UNITTESTS_LIBS)
	cgreen-runner --suite $(SUITE) $(UNITOUTPUT) $(ISOLATED_UNITTESTS_LIBS)

# Rule to compile objects to subdirectory
$(UNITTESTS_OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
$(UNITTESTS_OBJDIR)/%_tests.o: %_tests.c
	$(CC) $(CFLAGS) -o $@ -c $<

# Create directory if it doesn't exist
$(UNITTESTS_OBJDIR):
	@mkdir $(UNITTESTS_OBJDIR)

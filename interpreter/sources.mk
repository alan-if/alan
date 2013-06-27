# Declaration of sources required for various types of builds
#
# Unittests are done using the excellent
# CGreen unit test, stub and mocking framework by
# Marcus Baker et al. (http://sourceforge.net/projects/cgreen)

# Either using its runner which discovers test automatically...
MODULES_WITH_ISOLATED_UNITTESTS = \
	rules

MODULES_WITH_UNITTESTS_USING_RUNNER = \
	act.c \
	reverse.c \
	rules.c \
	params.c

# Or using a main program which requires adding every test manually
MODULES_WITH_UNITTESTS_USING_MAIN = \
	AltInfo.c \
	ParameterPosition.c \
	StateStack.c \
	args.c \
	debug.c \
	exe.c \
	inter.c \
	instance.c \
	main.c \
	parse.c \
	save.c \
	set.c \
	stack.c \
	state.c \
	sysdep.c \
	output.c \
	word.c \

MODULES_WITHOUT_UNITTESTS = \
	Container.c \
	Location.c \
	actor.c \
	attribute.c \
	checkentry.c \
	class.c \
	current.c \
	compatibility.c \
	decode.c \
	dictionary.c \
	event.c \
	lists.c \
	literal.c \
	memory.c \
	msg.c \
	options.c \
	readline.c \
	scan.c \
	score.c \
	syntax.c \
	syserr.c \
	term.c \
	utils.c

# All sources common for the main build
MAINSRCS = \
	$(MODULES_WITH_UNITTESTS_USING_RUNNER) \
	$(MODULES_WITH_UNITTESTS_USING_MAIN) \
	$(MODULES_WITHOUT_UNITTESTS) \
	$(ALLOCSRCS)

MODULES_WITH_UNITTESTS_USING_RUNNER_TESTSRCS = ${MODULES_WITH_UNITTESTS_USING_RUNNER:.c=Tests.c}
MODULES_WITH_UNITTESTS_USING_MAIN_TESTSRCS = ${MODULES_WITH_UNITTESTS_USING_MAIN:.c=Tests.c}
UNITTESTSMAIN = unittests.c xml_reporter.c gopt.c

UNITTESTS_USING_MAIN_SRCS = $(UNITTESTSMAIN) $(MODULES_WITH_UNITTESTS_USING_RUNNER_TESTSRCS) $(MODULES_WITH_UNITTESTS_USING_MAIN_TESTSRCS) $(MODULES_WITHOUT_UNITTESTS)
UNITTESTS_USING_RUNNER_SRCS = $(MODULES_WITH_UNITTESTS_USING_RUNNER_TESTSRCS) $(MODULES_WITH_UNITTESTS_USING_MAIN) $(MODULES_WITHOUT_UNITTESTS)

UNITTESTS_ALL_SRCS = $(MAINSRCS) $(MODULES_WITH_UNITTESTS_USING_MAIN_TESTSRCS) $(MODULES_WITH_UNITTESTS_USING_RUNNER_TESTSRCS) $(UNITTESTSMAIN)

ARUNSRCS = arun.c $(MAINSRCS)
GLKSRCS = glkstart.c glkio.c
WINARUNSRCS = ${GLKSRCS} ${ARUNSRCS} winglk.c
GLKARUNSRCS = ${GLKSRCS} ${ARUNSRCS}
GARARUNSRCS = ${GLKSRCS} ${ARUNSRCS}

# Sources triggering new version marking
VERSIONSRCS = $(MAINSRCS) arun.c glkstart.c glkio.c winglk.c

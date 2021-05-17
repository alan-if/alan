include ../mk/include_guard.mk
# Declaration of sources required for various types of builds
#
# Unittests are done using the excellent
# CGreen unit test, stub and mocking framework by
# Marcus Baker et al. (https://github.com/cgreen-devs/cgreen)

# Either using its runner which discovers test automatically...
# With everything mocked so they run in complete isolation...
MODULES_WITH_ISOLATED_UNITTESTS = \
	compatibility \
	dictionary \
	exe \
	instance \
	lists \
	literal \
	memory \
	readline \
	rules \
	save \
	stack \
	sysdep \

# ... or in one library linked with all modules (much less clean...)
# These have corresponding xxxTests.c which #include xxx.c to get at
# the internals (inappropriate intimacy...). They are then compiled
# and linked together with the non-unittested modules in a library
# that can be cgreen-runner'd. They should be migrated to isolated
# unittests...
MODULES_WITH_LINKED_UNITTESTS = \
	args.c \
	AltInfo.c \
	act.c \
	debug.c \
	exe.c \
	instance.c \
	inter.c \
	main.c \
	output.c \
	ParameterPosition.c \
	params.c \
	parse.c \
	reverse.c \
	set.c \
	stack.c \
	state.c \
	StateStack.c \
	sysdep.c \
	utils.c \
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
	rules.c \
	save.c \
	scan.c \
	score.c \
	syntax.c \
	syserr.c \
	term.c \
	types.c \
	fnmatch.c \
	converter.c


MODULES_WITH_LINKED_UNITTESTS_SRCS = ${MODULES_WITH_LINKED_UNITTESTS:.c=Tests.c}
UNITTESTSMAIN = unittests.c xml_reporter.c gopt.c

LINKED_UNITTESTS_SRCS = $(MODULES_WITH_LINKED_UNITTESTS_SRCS) $(MODULES_WITHOUT_UNITTESTS)

ALL_MODULES = $(MODULES_WITH_LINKED_UNITTESTS) $(MODULES_WITHOUT_UNITTESTS)

ARUNSRCS = arun.c $(ALL_MODULES)
GLKSRCS = glkstart.c glkio.c
WINARUNSRCS = ${GLKSRCS} ${ARUNSRCS} winglk.c
GLKARUNSRCS = ${GLKSRCS} ${ARUNSRCS}
GARARUNSRCS = ${GLKSRCS} ${ARUNSRCS}

# Sources triggering new version marking
VERSIONSRCS = $(ARUNSRCS) glkstart.c glkio.c winglk.c

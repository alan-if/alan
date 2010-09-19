# Declaration of sources required for various types of builds
#
# UNITTESTED are compiled and unittested with the excellent
# CGreen unit test, stub and mocking framework by
# Marcus Baker et al. (http://sourceforge.net/projects/cgreen)

UNITTESTED = \
	AltInfo.c \
	StateStack.c \
	args.c \
	act.c \
	exe.c \
	inter.c \
	main.c \
	parse.c \
	params.c \
	parameterPosition.c \
	reverse.c \
	save.c \
	set.c \
	stack.c \
	state.c \
	sysdep.c \
	output.c \
	word.c \

OTHERSRCS = \
	Container.c \
	Location.c \
	actor.c \
	attribute.c \
	checkentry.c \
	class.c \
	current.c \
	debug.c \
	decode.c \
	dictionary.c \
	event.c \
	instance.c \
	lists.c \
	literal.c \
	memory.c \
	msg.c \
	options.c \
	readline.c \
	rules.c \
	scan.c \
	score.c \
	syntax.c \
	syserr.c \
	term.c \
	utils.c

# All sources common for the main build
MAINSRCS = \
	$(UNITTESTED) \
	$(OTHERSRCS)

UNITTESTEDSRCS = ${UNITTESTED:.c=Tests.c}

UNITTESTSSRCS = unittests.c $(UNITTESTEDSRCS) $(OTHERSRCS)
ARUNSRCS = arun.c $(MAINSRCS) smartall.c
GLKSRCS = glkstart.c glkio.c
WINARUNSRCS = ${GLKSRCS} ${ARUNSRCS} winglk.c
GLKARUNSRCS = ${GLKSRCS} ${ARUNSRCS}
GARARUNSRCS = ${GLKSRCS} ${ARUNSRCS}

# Sources triggering new version marking
VERSIONSRCS = $(MAINSRCS) arun.c glkstart.c glkio.c winglk.c

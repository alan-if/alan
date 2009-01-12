# Declaration of sources required for various types of builds
#
# UNIT2 is the excellent CGreen unit test, stub and mocking framework by
# Marcus Baker et al. (http://sourceforge.net/projects/cgreen)

UNIT2SRC = \
	unit2.c \
	AltInfoTests.c \
	StateStackTests.c \
	actTests.c \
	exeTests.c \
	interTests.c \
	mainTests.c \
	parseTests.c \
	paramsTests.c \
	reverseTests.c \
	saveTests.c \
	setTests.c \
	stackTests.c \
	stateTests.c \
	sysdepTests.c \
	outputTests.c \
	smartall.c

UNIT2REQUIREDSRCS = \
	actor.c \
	attribute.c \
	checkentry.c \
	Container.c \
	Location.c \
	args.c \
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
	score.c \
	syntax.c \
	syserr.c \
	term.c \
	utils.c \
	word.c \

# All sources for the main build
MAINSRCS = \
	actor.c \
	AltInfo.c \
	attribute.c \
	checkentry.c \
	Container.c \
	Location.c \
	StateStack.c \
	act.c \
	args.c \
	class.c \
	current.c \
	debug.c \
	decode.c \
	dictionary.c \
	event.c \
	exe.c \
	instance.c \
	inter.c \
	lists.c \
	literal.c \
	main.c \
	memory.c \
	msg.c \
	options.c \
	output.c \
	params.c \
	parse.c \
	readline.c \
	reverse.c \
	rules.c \
	save.c \
	score.c \
	set.c \
	stack.c \
	state.c \
	syntax.c \
	sysdep.c \
	syserr.c \
	term.c \
	utils.c \
	word.c \
	smartall.c

UNIT2SRCS = $(UNIT2SRC) $(UNIT2REQUIREDSRCS)
UNIT2OBJECTS = ${UNIT2SRCS:.c=.o} alan.version.o


VERSIONSRCS = $(MAINSRCS) arun.c
VERSIONOBJECTS = ${VERSIONSRCS:.c=.o}

ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
TERMARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
GARARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}


# Declaration of sources required for various types of builds
#
# UNIT2 is the excellent CGreen unit test, stub and mocking framework by
# Marcus Baker (http://sourceforge.net/projects/cgreen)

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
	smartall.c

UNIT2REQUIREDSRCS = \
	AttributeEntry.c \
	checkentry.c \
	Container.c \
	Location.c \
	args.c \
	debug.c \
	decode.c \
	lists.c \
	options.c \
	readline.c \
	rules.c \
	syserr.c \
	term.c \

# All sources for the main build
MAINSRCS = \
	AltInfo.c \
	AttributeEntry.c \
	checkentry.c \
	Container.c \
	Location.c \
	StateStack.c \
	act.c \
	args.c \
	debug.c \
	decode.c \
	exe.c \
	inter.c \
	lists.c \
	main.c \
	options.c \
	params.c \
	parse.c \
	readline.c \
	reverse.c \
	rules.c \
	save.c \
	set.c \
	stack.c \
	state.c \
	sysdep.c \
	syserr.c \
	term.c \
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


# Declaration of sources required for various types of builds
#

# UNIT1 is a home brewed unit test framework where all unit tested source
# gets included in a single source file
UNIT1SRC = \
	unit.c

# But that means that there is a lot of source files that are required
# in that build in order to resolve all dependencies
UNIT1REQUIREDSRCS = \
	AltInfo.c \
	AltInfoArray.c \
	CheckEntryArray.c \
	Container.c \
	act.c \
	args.c \
	debug.c \
	decode.c \
	exe.c \
	options.c \
	params.c \
	parse.c \
	readline.c \
	reverse.c \
	rules.c \
	save.c \
	state.c \
	sysdep.c \
	term.c \

# UNIT2 is the excellent CGreen unit test, stub and mocking framework by
# Marcus Baker (http://sourceforge.net/projects/cgreen)
UNIT2SRC = \
	unit2.c \
	AltInfoArrayTests.c \
	AltInfoTests.c \
	actTests.c \
	exeTests.c \
	parseTests.c \
	reverseTests.c \
	saveTests.c \
	stateTests.c \
	stackTests.c \
	sysdepTests.c \

UNIT2REQUIREDSRCS = \
	CheckEntryArray.c \
	Container.c \
	args.c \
	debug.c \
	decode.c \
	inter.c \
	main.c \
	options.c \
	params.c \
	readline.c \
	rules.c \
	set.c \
	syserr.c \
	term.c \

# All sources for the main build
MAINSRCS = \
	AltInfo.c \
	AltInfoArray.c \
	CheckEntryArray.c \
	Container.c \
	act.c \
	args.c \
	debug.c \
	decode.c \
	exe.c \
	inter.c \
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

UNIT1SRCS = $(UNIT1SRC) $(UNIT1REQUIREDSRCS)
UNIT1OBJECTS = ${UNIT1SRCS:.c=.o} alan.version.o

UNIT2SRCS = $(UNIT2SRC) $(UNIT2REQUIREDSRCS)
UNIT2OBJECTS = ${UNIT2SRCS:.c=.o} alan.version.o


VERSIONSRCS = $(MAINSRCS) $(BUILDSRCS) arun.c
VERSIONOBJECTS = ${VERSIONSRCS:.c=.o}

ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
TERMARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
GARARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}


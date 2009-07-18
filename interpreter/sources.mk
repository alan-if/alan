# Declaration of sources required for various types of builds
#
# UNITTESTS is the excellent CGreen unit test, stub and mocking framework by
# Marcus Baker et al. (http://sourceforge.net/projects/cgreen)

UNITTESTSSRC = \
	unittests.c \
	smartall.c \
			\
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
	wordTests.c \

UNITTESTSREQUIREDSRCS = \
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

# All sources for the main build
# TODO Break out UNITTESTEDSRCS from MAINSRCS as in compiler source.mk
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
MAINOBJECTS = ${MAINSRCS:.c=.o}

UNITTESTSSRCS = $(UNITTESTSSRC) $(UNITTESTSREQUIREDSRCS)

GLKSRCS = glkstart.c glkio.c
GLKOBJECTS = ${GLKSRCS:.c=.o}

VERSIONSRCS = $(MAINSRCS) arun.c
VERSIONOBJECTS = ${VERSIONSRCS:.c=.o}

# Final object sets to be used in targets
UNITTESTSOBJECTS = ${UNITTESTSSRCS:.c=.o} alan.version.o
ARUNOBJECTS = $(MAINOBJECTS) arun.o alan.version.o
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS} winglk.o arun.res
GARARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
TERMARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}


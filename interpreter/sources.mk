# Declaration of sources required for various types of builds
#
# UNITTESTS is the excellent CGreen unit test, stub and mocking framework by
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

MAINOBJECTS = ${MAINSRCS:.c=.o}

UNITTESTEDSRCS = ${UNITTESTED:.c=Tests.c}

UNITTESTSSRCS = unittests.c $(UNITTESTEDSRCS) $(OTHERSRCS) smartall.c alan.version.c
ARUNSRCS = arun.c $(MAINSRCS) smartall.c alan.version.c

GLKSRCS = glkstart.c glkio.c
GLKOBJECTS = ${GLKSRCS:.c=.o}

VERSIONSRCS = $(MAINSRCS) arun.c
VERSIONOBJECTS = ${VERSIONSRCS:.c=.o}

# Final object sets to be used in targets
UNITTESTSOBJECTS = ${UNITTESTSSRCS:.c=.o}
ARUNOBJECTS = ${ARUNSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS} winglk.o arun.res
GARARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
TERMARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}


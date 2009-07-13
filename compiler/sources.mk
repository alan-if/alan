# This file is included in other Makefiles to ensure
# that they all use the same sets

TMSRCS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

# Sources required for both builds
BUILDSRCS = \
	alan.c \
	alt.c \
	article.c \
	chk.c \
	cnt.c \
	context.c \
	dump.c \
	encode.c \
	evt.c \
	initialize.c \
	lim.c \
	msg.c \
	nam.c \
	opt.c \
	options.c \
	rul.c \
	sco.c \
	scr.c \
	set.c \
	spa.c \
	srcp.c \
	stp.c \
	str.c \
	syn.c \
	sysdep.c \
	timing.c \
	type.c \
	util.c \
	wht.c \

# Excluded from BUILDSRCS because there are unit tests for these
UNITTESTED = \
	add.c \
	adv.c \
	atr.c \
	cla.c \
	description.c \
	elm.c \
	emit.c \
	exp.c \
	ext.c \
	id.c \
	ifid.c \
	ins.c \
	lst.c \
	prop.c \
	res.c \
	resource.c \
	stm.c \
	stx.c \
	sym.c \
	vrb.c \
	whr.c \
	wrd.c \

# More Sources required for Alan program build
ALANSRCS = \
	main.c \
	$(UNITTESTED)


UNITTESTSRCS = ${UNITTESTED:.c=Test.c}

MAINSRCS = $(TMCSRCS) $(ALANSRCS) $(BUILDSRCS)
MAINOBJECTS = ${MAINSRCS:.c=.o} alan.version.o

VERSIONSRCS = $(ALANSRCS) $(BUILDSRCS) $(TMSRCS)


# Sources for the test framework
UNITSRCS = $(UNITTESTSRCS) \
	unit.c \
	unitList.c \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c

TESTSRCS = $(UNITSRCS) $(BUILDSRCS)
TESTOBJS = ${TESTSRCS:.c=.o} alan.version.o


# This file is included in other Makefiles to ensure
# that they all use the same sets

# Sources for the compiler generator ToolMaker
TMSRCS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk

# C sources generated from ToolMaker
TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

# Sources which have unittests defined
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

OTHERSRCS = \
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


# Sources required for Alan program build
ALANSRCS = \
	main.c \
	$(TMCSRCS) \
	$(UNITTESTED) \
	$(OTHERSRCS)

# Sources for the test framework
UNITTESTEDSRCS = ${UNITTESTED:.c=Test.c}
UNITSRCS = $(UNITTESTEDSRCS) \
	unitList.c \
	unitmock.c \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c

UNITTESTSMAIN = unittests.c xml_reporter.c gopt.c

UNITTESTSSRCS = $(UNITTESTSMAIN) $(UNITSRCS) $(OTHERSRCS)
UNITTESTSDLLSRCS = $(UNITSRCS) $(OTHERSRCS)

# Version timestamp dependencies
VERSIONSRCS = $(ALANSRCS) $(TMSRCS)


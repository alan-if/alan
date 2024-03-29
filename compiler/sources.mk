include ../mk/include_guard.mk

# This file is included in other Makefiles to ensure
# that they all use the same sets

# Sources for the compiler generator ToolMaker:
TMSRCS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk

# C sources generated from ToolMaker:
TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c\
	lmList.c

# Sources for unittests:
# Either using its runner which discovers test automatically...
# With everything mocked so they run in complete isolation...
# Sources here also needs to be in OTHERSRCS in order for the
# module itself to be compiled and linked with
MODULES_WITH_ISOLATED_UNITTESTS = \
	atr \
	context \
	converter \
	emit \
	exp \
	ext \
	id \
	ins \
	lmlog \
	prop \
	stm \
	sym \
	whr \

# Sources which have unittests defined using an explicit runner (xxxTests.c)
UNITTESTED = \
	add.c \
	adv.c \
	atr.c \
	cla.c \
	description.c \
	elm.c \
	exp.c \
	ext.c \
	ifid.c \
	ins.c \
	lst.c \
	prop.c \
	res.c \
	resource.c \
	stm.c \
	stx.c \
	sym.c \
	util.c \
	vrb.c \
	whr.c \
	wrd.c \

OTHERSRCS = \
	alan.c \
	alt.c \
	article.c \
	charset.c \
	chk.c \
	cnt.c \
	context.c \
	converter.c \
	dump.c \
	encode.c \
	emit.c \
	evt.c \
	id.c \
	initialize.c \
	lim.c \
	lmlog.c \
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
	wht.c \


# Sources required for Alan program build, except the version files
ALAN_SRCS = \
	main.c \
	$(TMCSRCS) \
	$(UNITTESTED) \
	$(OTHERSRCS)

# Sources for the linked tests
UNITTESTED_SRCS = ${UNITTESTED:.c=Test.c}
UNITSRCS = $(UNITTESTED_SRCS) \
	unitList.c \
	unitmock.c \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c

UNITTESTS_SRCS = $(UNITSRCS) $(OTHERSRCS)

# Version timestamp dependencies
VERSIONSRCS = $(ALAN_SRCS) $(TMSRCS)

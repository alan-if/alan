# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

CPP	= gcc -MM -ansi

CYGWIN =  -DCOMPILER=\"[GCC/Mingw32]\" -mno-cygwin
#CYGWIN =  -DCOMPILER=\"[GCC/CygWin32]\"

CFLAGS = -g -Wall $(CYGWIN) -funsigned-char $(INCLUDE)

LDFLAGS = -g $(CYGWIN)

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

# Sources which are in the unit tests
TESTSRCS = \
	ins.c \
	cla.c \
	adv.c \
	sym.c \
	whr.c \


# Sources required for both builds
BUILDSRCS = \
	alan.c \
	alt.c \
	chk.c \
	dump.c \
	elm.c \
	encode.c \
	evt.c \
	exp.c \
	id.c \
	lim.c \
	msg.c \
	nam.c \
	opt.c \
	rul.c \
	sco.c \
	scr.c \
	slt.c \
	srcp.c \
	stm.c \
	stp.c \
	str.c \
	stx.c \
	syn.c \
	vrb.c \
	wht.c \
	timing.c \
	util.c \
	spa.c \
	sysdep.c \
	options.c


# More Sources required for Alan program build
# Excluded from BUILDSRCS because they are in the unit tests
ALANSRCS = \
	main.c \
	adv.c \
	atr.c \
	cla.c \
	res.c \
	emit.c \
	ext.c \
	ins.c \
	lst.c \
	sym.c \
	whr.c \
	wrd.c \

MAINSRCS = $(ALANSRCS) $(BUILDSRCS) $(TMCSRCS)
MAINOBJECTS = ${MAINSRCS:.c=.o} alan.version.o

VERSIONSRCS = $(ALANSRCS) $(BUILDSRCS) $(TMSRCS)


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main targets
#
# If you build this on any machine without ToolMaker, remove "src"
#
all alan : .tmstamp test $(MAINOBJECTS) Makefile
	gcc -o alan $(MAINOBJECTS) $(LDFLAGS)
	@-mkdir ../bin 2> /dev/null
	cp alan.exe ../bin

.tmstamp: $(TMSRCS)
	$(MAKE) -f Makefile.tm

src:
	$(MAKE) -f Makefile.src

fromsrc: $(OBJECTS) alan.version.o
	$(LD) -o alan $(OBJECTS) alan.version.o $(LDFLAGS)


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#

# Sources for the test framework
UNITSRCS = unit.c \
	unitList.c \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c

TESTSRCS = $(UNITSRCS) $(BUILDSRCS)
TESTOBJS = ${TESTSRCS:.c=.o} alan.version.o

unit : $(TESTOBJS) 

# Run all unit tests!
test: unit
	@echo -n "******************************"
	@unit
	@echo "******************************"

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

smScanx.o: smScanx.c

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
depend:
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

#
# Version number generation
#
alan.version.h :
	cd ..; venum -write always -all alan
	cp ../alan.version.h .

version.h :
	cd ..; venum -write always -all alan
	cp ../version.h .

alan.version.c: $(VERSIONSRCS) ../alan.version
	cd ..; venum alan time
	cp ../alan.version.c .

acode.h: ../interpreter/acode.h
	cp ../interpreter/acode.h .
#
include dependencies.mk


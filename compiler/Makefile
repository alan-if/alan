# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

CYGWIN =  -mno-cygwin # Does not work because of intricate library problems

#GCOV =	-fprofile-arcs -ftest-coverage

CFLAGS = -g -Wall $(GCOV) -funsigned-char $(INCLUDE) $(CYGWIN)

LDFLAGS = -g $(CYGWIN)

CPP	= gcc -MM -ansi


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
	add.c \
	alt.c \
	chk.c \
	cnt.c \
	context.c \
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

all : .tmstamp test alan Makefile

gcov :
	$(MAKE) "GCOV=-fprofile-arcs -ftest-coverage"

alan: $(MAINOBJECTS)
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
	@unit

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
	cp -p ../alan.version.h .

version.h :
	cd ..; venum -write always -all alan
	cp -p ../version.h .

../alan.version: $(VERSIONSRCS) 
	cd ..; venum alan time

alan.version.c:  ../alan.version
	cp -p ../alan.version.c .

#
include dependencies.mk


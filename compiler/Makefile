# Makefile for alan compiler on Windows using Cygwin32
#

CPP	= gcc -MM -ansi

COMPILER= -DCOMPILER=\"[CygWin32]\"

CFLAGS = -g -Wall -funsigned-char $(INCLUDE) $(COMPILER)

LDFLAGS = -g

EXTRAS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

TMOBJS = ${TMSRCS:.c=.o}

SRCS = \
	alan.c \
	main.c \
	options.c \
	adv.c \
	alt.c \
	atr.c \
	chk.c \
	cla.c \
	dump.c \
	encode.c \
	emit.c \
	elm.c \
	evt.c \
	exp.c \
	ext.c \
	id.c \
	ins.c \
	lim.c \
	lst.c \
	msg.c \
	opt.c \
	res.c \
	rul.c \
	sco.c \
	scr.c \
	slt.c \
	srcp.c \
	stm.c \
	stp.c \
	stx.c \
	str.c \
	sym.c \
	syn.c \
	vrb.c \
	whr.c \
	wht.c \
	wrd.c \
	timing.c \
	util.c \
	spa.c \
	sysdep.c

#OBSOLETE
#	nam.c \
#	act.c \
#	cnt.c \
#	loc.c \
#	obj.c \

VERSIONSRCS = $(SRCS) $(EXTRAS)

VERSIONOBJECTS = ${SRCS:.c=.o}

OBJECTS = $(VERSIONOBJECTS) $(TMOBJS) alan.version.o

all alan alan.exe : $(OBJECTS) Makefile
	gcc -o alan $(OBJECTS) $(LDFLAGS)	
	-mkdir ../bin
	cp alan.exe ../bin

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

smScanx.o: smScanx.c

src:
	$(MAKE) -f Makefile.src

fromsrc: $(OBJECTS) alan.version.o
	$(LD) -o alan $(OBJECTS) alan.version.o $(LDFLAGS)

.PHONY: unit
# Run all unit test
unit:
	make sym
	make cla
	make ins

# Unit Tests
# Each unit is built separately with the test in a main program (xxxTest)

# All unit tests
symTest.o : symTest.c sym.c unitTest.h
symTest : symTest.o util.o lmList.o id.o cla.o dump.o srcp.o str.o lst.o slt.o

sym:
	$(MAKE) $@Test; $@Test

claTest.o : claTest.c cla.c unitTest.h
claTest : claTest.o util.o sym.o dump.o id.o lst.o srcp.o slt.o lmList.o str.o

cla:
	$(MAKE) $@Test; $@Test

insTest.o : insTest.c ins.c unitTest.h
insTest : insTest.o util.o sym.o dump.o id.o lst.o srcp.o slt.o lmList.o str.o

ins:
	$(MAKE) $@Test; $@Test

depend:
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.mk; \
	done

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

#
include dependencies.mk


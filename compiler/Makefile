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

UNITSRCS = \
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

SRCS = \
	alan.c \
	main.c \
	options.c \
	$(UNITSRCS)

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


UNITOBJS = ${UNITSRCS:.c=.o}

# Run all unit tests
unit : unit.o \
	alt.o \
	atr.o \
	chk.o \
	dump.o \
	encode.o \
	emit.o \
	elm.o \
	evt.o \
	exp.o \
	ext.o \
	id.o \
	ins.o \
	lim.o \
	lst.o \
	msg.o \
	opt.o \
	res.o \
	rul.o \
	sco.o \
	scr.o \
	slt.o \
	srcp.o \
	stm.o \
	stp.o \
	stx.o \
	str.o \
	syn.o \
	vrb.o \
	whr.o \
	wht.o \
	wrd.o \
	timing.o \
	util.o \
	spa.o \
	sysdep.o \
	unitList.o \
	alan.o \
	options.o \
	alan.version.o \
	pmParse.o pmPaSema.o \
	pmErr.o \
	smScanx.o smScSema.o

test: unit
	unit

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
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


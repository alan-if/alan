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

OBJECTS = \
	alan.o \
	alan.version.o \
	main.o \
	options.o \
	act.o \
	adv.o \
	alt.o \
	atr.o \
	chk.o \
	cnt.o \
	dump.o \
	encode.o \
	emit.o \
	elm.o \
	evt.o \
	exp.o \
	ext.o \
	lim.o \
	loc.o \
	lst.o \
	msg.o \
	nam.o \
	obj.o \
	opt.o \
	res.o \
	rul.o \
	sco.o \
	scr.o \
	srcp.o \
	stm.o \
	stp.o \
	stx.o \
	str.o \
	sym.o \
	syn.o \
	vrb.o \
	whr.o \
	wht.o \
	wrd.o \
	timing.o \
	spa.o \
	sysdep.o \
	$(TMOBJS)

all alan alan.exe : $(OBJECTS) Makefile
	gcc -o alan $(OBJECTS) $(LDFLAGS)
	cp alan.exe ../bin

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

smScanx.o: smScanx.c

alan.version.c : ../alan.version.c alan.version.h
	cp ../alan.version.c .

alan.version.h : ../alan.version.h version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

fromsrc: $(OBJECTS) alan.version.o
	$(LD) -o alan $(OBJECTS) alan.version.o $(LDFLAGS)

include dependencies.mk

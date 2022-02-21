# Makefile for alan compiler on Windows using Cygwin32
#

CPP	= gcc -MM -ansi

COMPILER= -DCOMPILER=\"[CygWin32]\"

CFLAGS = $(OPTIMIZE) -g -Wall -funsigned-char $(INCLUDE) $(COMPILER)

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

VERSIONOBJECTS = \
	alan.o \
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

OBJECTS = $(VERSIONOBJECTS) alan.version.o

all alan alan.exe : $(OBJECTS) Makefile
	gcc -o alan $(OBJECTS) $(LDFLAGS)	
	-mkdir ../bin
	-cp alan.exe ../bin

.PHONY:release
release:
	$(MAKE) "OPTIMIZE=-O4" alan.exe


spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

smScanx.o: smScanx.c

#
include dependencies.mk

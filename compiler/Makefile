# Makefile for alan			Date: 1990-01-14 14:47:59/toolmake
#


CPP	= gcc -MM

TMLIB	= $(TMHOME)/lib/ansi-c

#CC	= acc -Xc -vc
CC	= gcc -ansi -pedantic -Wall -I/usr/local/include/ansi
CFLAGS	= $(DEBUG)

LD	= $(CC)
LDFLAGS	= -lm

EXTRAS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMSRCS = \
	lmList.c \
	smScan.c smScSema.c\
	pmErr.c \
	pmParse.c pmPaSema.c

SRCS = \
	$(TMSRCS) \
	dump.c \
	emit.c \
	encode.c \
	act.c \
	adv.c \
	alt.c \
	atr.c \
	chk.c \
	cla.c \
	cnt.c \
	elm.c \
	evt.c \
	exp.c \
	ext.c \
	lim.c \
	loc.c \
	lst.c \
	msg.c \
	nam.c \
	obj.c \
	opt.c \
	rul.c \
	sco.c \
	scr.c \
	srcp.c \
	stm.c \
	stp.c \
	str.c \
	stx.c \
	sym.c \
	syn.c \
	vrb.c \
	whr.c \
	wht.c \
	wrd.c \
	time.c \
	sysdep.c \
	spa.c

MAINSRC = alan.c

LIB = alan.a

.PRECIOUS: $(LIB)

#.KEEP_STATE:

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}

#-- MAIN TARGET
debug all: alan

debug:=	DEBUG= -g
all:=	DEBUG= -O

alan: $(LIB)($(OBJECTS)) version.o alan.o Makefile
	ranlib $(LIB)
	$(LD) -o $@ $(CFLAGS) $(MAINOBJ) version.o $(LIB) $(LDFLAGS)
	cp alan.prod ..
	cp alan ../`arch`

pmParse.c pmPaSema.c pmParse.h pmErr.c pmErr.h : alan.pmt $(TMLIB)/Parse.imp
	imp alan.pmt

alan.pmt : alan.pmk alan.tmk
	pmk alan

smScan.c smScSema.c smScan.h : alan.smt $(TMLIB)/Scan.imp
	imp alan.smt

alan.smt : alan.smk alan.tmk
	smk alan

lmList.c lmList.h alanCommon.h : alan.lmt $(TMLIB)/List.imp
	imp alan.lmt

alan.lmt : alan.lmk alan.tmk
	lmk alan

malloc: $(LIB)($(OBJECTS)) version.o alan.o
	$(CC) -c $(CFLAGS) -DMALLOC alan.c
	$(CC) -o alan $(CFLAGS) alan.o version.o /usr/lib/debug/malloc.o $(LIB) $(LDFLAGS)
	-rm alan.o

purify: $(LIB)($(OBJECTS)) version.o alan.o
	purify $(CC) -o alan $(CFLAGS) alan.o version.o $(LIB) $(LDFLAGS)

NONANSI:
	mv alan.tmk alan.tmk.ansi
	sed -e s/ansi-c/c/ alan.tmk.ansi > alan.tmk
	make alanCommon.h $(TMSRCS)
	mv -f alan.tmk.ansi alan.tmk

MSDOS:
	mv alan.tm alan.tm.ansi
	sed s/SunOS/MSDOS/ alan.tmk.sun > alan.tmk
	make alanCommon.h $(TMSRCS)
	mv -f alan.tmk.sun alan.tmk

version.c: $(LIB)($(OBJECTS)) ../[0-9]*
	@-rm [0-9]*
	@cp ../[0-9]* .
	venum +t

acode.h: ../interpreter/acode.h
	-ln -s ../interpreter/acode.h

sysdep.h: ../interpreter/sysdep.h
	-ln -s ../interpreter/sysdep.h

sysdep.c: ../interpreter/sysdep.c
	-ln -s ../interpreter/sysdep.c

alan.prod: alan.pml
	sed -e "1,/P R O D/d" -e "/Summary/,$$ d" alan.pml > alan.prod

#-- Cheating MAKE
link:
	$(CC) -o alan $(CFLAGS) $(OBJECTS) version.o $(LDFLAGS)

#-- ARCHIVE
DOSSUBS = -e s/alanToken/alanToke/ -e s/alanParse/alanPars/
archives:
	-\rm Makefile*~
	make MSDOS
	zip -k compiler.zip *.[ch] Makefile.cmd
	sed $(DOSSUBS) alan.c > ALAN.C
	sed $(DOSSUBS) alanErr.c > ALANERR.C
	sed $(DOSSUBS) alanErr.h > ALANERR.H
	sed $(DOSSUBS) alanParse.c > ALANPARS.C
	sed $(DOSSUBS) alanParse.h > ALANPARS.H
	sed $(DOSSUBS) alanSema.c > ALANSEMA.C
	sed $(DOSSUBS) alanScan.c > ALANSCAN.C
	sed $(DOSSUBS) alanScan.h > ALANSCAN.H
	sed $(DOSSUBS) alanScanSema.c > ALANSCSE.C
	zip -k -m compiler.zip *.C *.H
	make NONANSI
	lha c compiler.lha *.c *.h Makefile* dependencies.mk
	make .smart

#-- release
release:
	cvs tag v`basename ../[0-9]* | sed s/\\\.//`
	
#--
CLEAN: clean
	-rm alanCommon.h pmParse* pmPaSema* pmErr* smScan* smScSema* lmList* alan.voc alan.[psl]mt

clean:
	-rm core *~ *.zoo *.bak *.lzh

#--
#FILETER = | sed -e "\,/usr/include, d" -e "s,./,," | sort -u 
FILETER =
depend:
	@cvs update
	@echo "Updating dependencies.mk, Makefile.amiga"
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> "dependencies.mk"; \
	done
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga

include dependencies.mk

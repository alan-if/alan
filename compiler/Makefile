# Makefile for alan compiler
#


CPP	= gcc -MM

TMLIB	= $(TMHOME)/lib/ansi-c

purify:=		CC = acc -Xc -vc
all debug release:=	CC = gcc -ansi -pedantic -Wall -I/usr/local/include/ansi
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

alan: tm $(OBJECTS) version.o $(MAINOBJ) Makefile alan.prod
	$(LD) -o $@ $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	cp alan.prod ..
	cp alan ../`arch`

tm: .pmkstamp .smkstamp .lmkstamp

.lmkstamp : alan.lmk alan.tmk $(TMHOME)/lib/ansi-c/List.imp
	lmk $(LMKQ) alan
	@touch .lmkstamp

.pmkstamp: alan.pmk alan.tmk $(TMHOME)/lib/ansi-c/Parse.imp $(TMHOME)/lib/ansi-c/Err.imp
	pmk $(PMKQ) alan
	@touch .pmkstamp

.smkstamp : .pmkstamp alan.smk $(TMHOME)/lib/ansi-c/Scan.imp
	smk $(SMKQ) alan
	@touch .smkstamp

malloc: tm $(OBJECTS) version.o $(MAINOBJ)
	$(CC) -c $(CFLAGS) -DMALLOC alan.c
	$(CC) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)
	-rm alan.o

purify: tm $(OBJECTS) version.o alan.o
	purify -handle-signals $(CC) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) $(LDFLAGS)

NONANSI:
	mv alan.tmk alan.tmk.ansi
	sed -e s/ansi-c/c/ alan.tmk.ansi > alan.tmk
	make tm
	mv -f alan.tmk.ansi alan.tmk

DOSSUBS = -e s/alanCommon/alanComm/
MSDOS:
	mv alan.tmk alan.tmk.sun
	sed s/SunOS/MSDOS/ alan.tmk.sun > alan.tmk
	make tm
	for f in `grep alanCommon *.[ch]` ; do \
		sed $(DOSSUBS) $$f > `toupper $$f` ; \
	end
	mv -f alan.tmk.sun alan.tmk

version.c: $(OBJECTS) ../[0-9]*
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
archives:
	-\rm Makefile*~
	make NONANSI
	lha u compiler.lha *.c *.h Makefile* dependencies.mk
	zip -k -f compiler.zip *.[ch] Makefile.cmd
	make MSDOS
	zip -k -m compiler.zip *.C *.H
	touch alan.tmk
	make tm

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

# Makefile for alan compiler
#

GCC	= gcc2.4.5
CPP	= $(GCC) -MM -ansi -I/usr/local/include/ansi

TMLIB	= $(TMHOME)/lib/ansi-c


CC = $(GCC) -ansi -pedantic -Wall -I/usr/local/include/ansi
CFLAGS	= $(DEBUG)


LD	= $(GCC)
LDFLAGS	= -L/usr/local/lib/4.1.2

EXTRAS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c\
	lmList.c

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
	res.c \
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
	timing.c \
	sysdep.c \
	spa.c

MAINSRC = alan.c

LIB = alan.a

#.PRECIOUS: $(LIB)

#.KEEP_STATE:

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}

#-- MAIN TARGET
debug all: alan

debug purify:=		DEBUG= -g
all release :=	DEBUG= -O4

alan: tm alanobj

alanobj: $(OBJECTS) $(MAINOBJ) version version.o Makefile alan.prod
	$(LD) -o alan $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	cp alan.prod $(ALANDIR)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp alan $(ALANDIR)/bin/`arch`

src: tm version.c version.h

fromsrc: $(OBJECTS) $(MAINOBJ) version.o Makefile alan.prod
	$(LD) -o alan $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp alan $(ALANDIR)/bin/`arch`

tm: .pmkstamp .smkstamp .lmkstamp

.lmkstamp : alan.lmk alan.tmk $(TMHOME)/lib/ansi-c/List.imp
	lmk $(LMKQ) alan
	@touch .lmkstamp

.pmkstamp: alan.pmk alan.tmk $(TMHOME)/lib/ansi-c/Parse.imp $(TMHOME)/lib/ansi-c/Err.imp
	pmk $(PMKQ) alan
	@touch .pmkstamp

.smkstamp : .pmkstamp alan.smk alan.voc $(TMHOME)/lib/ansi-c/Scan.imp
	smk $(SMKQ) alan
	@touch .smkstamp

pmParse.h pmParse.c pmPaSema.c pmErr.c alan.voc alan.pml:
	touch alan.pmk
	make .pmkstamp

smScan.h smScan.c smScSema.c:
	touch alan.smk
	make .smkstamp

lmList.h lmList.c:
	touch alan.lmk
	make .lmkstamp

malloc: tm $(OBJECTS) version.o $(MAINOBJ)
	$(CC) -c $(CFLAGS) -DMALLOC alan.c
	$(LD) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)
	-rm alan.o

purify: tm $(OBJECTS) version.o alan.o
	purify -handle-signals $(LD) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) $(LDFLAGS)

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
	for f in `grep -l alanCommon *.[ch]` ; do \
		sed $(DOSSUBS) $$f > `toupper $$f` ; \
	done
	mv -f alan.tmk.sun alan.tmk

version.c + version.h: ../[0-9]*
	-@cd ..;rm ../[0-9]*_[0-9]*_[0-9]*;sh version.sh
	@-rm [0-9]*
	@cp ../[0-9]* .
	venum -c


version: $(OBJECTS) ../[0-9]*
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
	$(LD) -o alan $(CFLAGS) $(OBJECTS) version.o $(LDFLAGS)

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
	@echo "Updating dependencies.mk!"
	@echo > dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.mk; \
	done
	@make Makefile.amiga.
#	@make alan.make.

Makefile.amiga.:
	@echo Updating Makefile.amiga!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga

alan.make.:
	@echo Updating alan.make!
	@echo !Automagically created makefile for Alan compiler on VMS > alan.make
	@echo CQ=/STANDARD=PORTABLE >> alan.make
	@echo OBJECTS = $(VMSOBJECTS) >> alan.make
	@echo >> alan.make
	@cat dependencies.mk >> alan.make
	@echo >> alan.make
	@echo alan.exe : "#(OBJECTS)" version.obj >> alan.make
	@/usr/5bin/echo \\t$$ link/exe=alan "#(LQ)" $(VMSLINKOBJS) >> alan.make
	@/usr/5bin/echo \\t$$ copy alan.exe "<->" >> alan.make

include dependencies.mk

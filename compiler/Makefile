# Makefile for alan compiler
#

ARCHS	= sun4 solaris2 hp
VERSION	= `ls [0-9]*_[0-9]*_[0-9]*.* | tail -1 | sed s/\\\\.//g | sed s/_//g`


GCC	= gcc
CPP	= $(GCC) -MM -ansi

TMLIB	= $(TMHOME)/lib/ansi-c


CC = $(GCC) -pedantic -Wall
CFLAGS	= $(DEBUG) $(INCLUDE)

tcov := CC = acc -a
tcov := LD = acc -a

LD	= $(GCC) $(DEBUG)

sun4:= LDFLAGS	= -L/usr/local/lib/4.1.2
sun4:= INCLUDE = -I/usr/local/include/ansi

solaris1:= LDFLAGS = -L/usr/local/lib/4.1.2
solaris1:= INCLUDE = -I/usr/local/include/ansi

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

SRCS = \
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

ALLSRCS = $(MAINSRC) $(SRCS) $(TMSRCS)

VERSIONSRCS = $(SRCS) $(MAINSRC) $(EXTRAS)

LIB = alan.a

#.KEEP_STATE:

OBJECTS = ${ALLSRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}


#-- MAIN TARGET

DEBUG= -g

all debug tcov: objarch alan alan.prod Makefile
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp alan $(ALANDIR)/bin/`arch`
	cp alan.prod $(ALANDIR)

arch:
	make `arch`

release:
	@for a in $(ARCHS) ; do \
		if fresh $(ALANDIR)/bin/$$a/alan \: $(SRCS) ; then \
		cp $(ALANDIR)/bin/$$a/alan $(ALANDIR)/bin/$$a/alan$(VERSION) ; \
		else \
			echo $(ALANDIR)/bin/$$a/alan is not fresh! ; \
			touch ../.notfresh ; \
		fi ; \
	done

sun3 sun4 solaris1 solaris2 hp: objarch fromsrc

objarch:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test `cat .arch` != `arch`; then \
	    echo "Removing *.o for `cat .arch`" ; \
	    rm *.o; \
	    echo `arch` > .arch; \
	fi

alan: tm $(OBJECTS) .version version.o
	$(LD) -o alan $(OBJECTS) version.o $(LDFLAGS)

src: tm sysdep.h sysdep.c acode.h version.h

fromsrc: $(OBJECTS) $(MAINOBJ) version.o
	$(LD) -o alan $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp alan $(ALANDIR)/bin/`arch`

tm: .pmkstamp .smkstamp .lmkstamp

.lmkstamp : alan.lmk alan.tmk $(TMLIB)/List.imp
	lmk $(LMKQ) alan
	touch .lmkstamp

.pmkstamp: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp
	pmk $(PMKQ) alan
	touch .pmkstamp

.smkstamp : .pmkstamp alan.smk alan.voc $(TMLIB)/Scan.imp
	smk $(SMKQ) alan
	@sed -e "1,/START of scanning tables/d" -e "/END of scanning tables/,$$ d" smScan.c > smScan.iso
	smk $(SMKQ) -set MAC alan
	@sed -e "1,/START of scanning tables/d" -e "/END of scanning tables/,$$ d" smScan.c > smScan.mac
#	Use the MSDOS source for code as it contains HUGE pointer arithmatic if necessary
	@make savetm
	@sed s/SunOS/MSDOS/ alan.tmk.orig > alan.tmk
	smk $(SMKQ) -set PC alan
	@sed -e "1,/START of scanning tables/d" -e "/END of scanning tables/,$$ d" smScan.c > smScan.pc
	@sed -e "1,/START of scanning tables/w smScan.head" -e "/END of scanning tables/,$$ w smScan.tail" smScan.c > /dev/null
	@make restoretm
	@cat smScan.head > smScanx.c
	@echo "#if ISO == 1" >> smScanx.c
	@cat smScan.iso >> smScanx.c
	@echo "#else" >> smScanx.c
	@echo "#if __mac__" >> smScanx.c
	@cat smScan.mac >> smScanx.c
	@echo "#endif" >> smScanx.c
	@echo "#if __dos__" >> smScanx.c
	@cat smScan.pc >> smScanx.c
	@echo "#endif" >> smScanx.c
	@echo "#endif" >> smScanx.c
	@cat smScan.tail >> smScanx.c
	@rm smScan.head smScan.tail smScan.iso smScan.mac smScan.pc smScan.c
	touch .smkstamp

pmParse.h pmParse.c pmPaSema.c pmErr.c alan.voc alan.pml:
	-rm .pmkstamp
	make .pmkstamp

smScan.h smScanx.c smScSema.c:
	-rm .smkstamp
	make .smkstamp

lmList.h lmList.c:
	-rm .lmkstamp
	make .lmkstamp

malloc: tm $(OBJECTS) version.o $(MAINOBJ)
	$(CC) -c $(CFLAGS) -DMALLOC alan.c
	$(LD) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)
	-rm alan.o

purify: tm $(OBJECTS) version.o alan.o
	purify -handle-signals $(LD) -o alan $(CFLAGS) alan.o version.o $(OBJECTS) $(LDFLAGS)

version.h version.c: ../version.sh
	-cd ..; rm [0-9]*_[0-9]*_[0-9]*; sh version.sh
	-rm [0-9]*_[0-9]*_[0-9]*
	cp ../[0-9]*_[0-9]*_[0-9]* .
	venum -c

.version: $(VERSIONSRCS)
	venum +t
	touch .version

acode.h: ../interpreter/acode.h
	-ln -s ../interpreter/acode.h

sysdep.h: ../interpreter/sysdep.h
	-ln -s ../interpreter/sysdep.h

sysdep.c: ../interpreter/sysdep.c
	-ln -s ../interpreter/sysdep.c

alan.prod: alan.pml
	sed -e "1,/P R O D/d" -e "/Summary/,$$ d" alan.pml > alan.prod

#- Other machines

savetm:
	@if test -f alan.tmk.orig; then \
	    echo "There is already an original!"; \
	    exit 1; \
	@fi
	mv alan.tmk alan.tmk.orig
	
restoretm:
	@mv -f alan.tmk.orig alan.tmk
	-rm .?mkstamp

NONANSI:
	make savetm
	sed -e s/ansi-c/c/ alan.tmk.orig > alan.tmk
	make src
	make restoretm

DOSSUBS = -e s/alanCommon/alanComm/
MSDOS:
	make src	
#	This was because some compilers on DOS can't cope with standard
#	pointer arithemtic in the generated scanner code
#	make savetm
#	sed s/SunOS/MSDOS/ alan.tmk.orig > alan.tmk
#	smk $(SMKQ) -set PC alan
#	make restoretm
	for f in `grep -l alanCommon *.[ch]` ; do \
		sed $(DOSSUBS) $$f > `toupper $$f` ; \
	done
	ramp iso2pc msg.c MSG.C
	ramp iso2pc sysdep.c SYSDEP.C

mac:
	make src
	refreshMac $(HOME)/mac/Alan/compiler
	sed -e 's/Token pmSySt/extern Token pmSySt/' -e 's/pmGrammar pmSeSt/extern pmGrammar pmSeSt/' pmParse.c > pmParse.tmp
	ramp iso2mac pmParse.tmp $(HOME)/mac/Alan/compiler/pmParse.c
	sed -e 's/extern Token pmSySt\[]/Token pmSySt\[64]/' -e 's/extern pmGrammar pmSeSt\[]/pmGrammar pmSeSt\[64]/' pmPaSema.c > pmPaSema.tmp
	ramp iso2mac pmPaSema.tmp $(HOME)/mac/Alan/compiler/pmPaSema.c

amiga:
#	make NONANSI
	lha u compiler.lha *.c *.h Makefile Makefile.amiga dependencies.mk

pc:
	make MSDOS
	zip -k -u compiler.zip *.[ch] Makefile.dos
	zip -k -m compiler.zip *.[CH]

#--
CLEAN: clean
	-rm alanCommon.h pmParse* pmPaSema* pmErr* smScan* smScSema* lmList* alan.voc *.o

clean:
	-rm core *~ *.zoo *.bak *.lzh

#--
#FILETER = | sed -e "\,/usr/, d" -e "s,./,," | sort -u 
FILETER =
depend: dependencies.mk. Makefile.amiga Makefile.dos # alan.make

dependencies.mk.:
	@echo "Updating dependencies.mk!"
	@echo > dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.mk; \
	done

Makefile.amiga: dependencies.mk Makefile
	@echo Updating Makefile.amiga!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga

Makefile.dos: dependencies.mk Makefile
	@echo Updating Makefile.dos!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.dos > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cp "#Makefile.tmp#" Makefile.dos
	@sed $(DOSSUBS) -e s/\\.o/\\.obj/ dependencies.mk >> Makefile.dos

alan.make: dependencies.mk
	echo Updating alan.make!
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

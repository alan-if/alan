# Makefile for alan			Date: 1990-01-14 14:47:59/toolmake
#


%: RCS/%,v
	@co -q $<

CPP	= gcc -MM

#CC	= acc -Xc -vc
CC	= gcc -ansi -pedantic -Wall -I/usr/local/include/ansi
CFLAGS	= $(DEBUG)

LD	= $(CC) $(DEBUG)
LDFLAGS	= -lm

OBJECTS = \
	alanParse.o alanSema.o \
	alanErr.o \
	alanScan.o alanScanSema.o\
	alanList.o \
	alan.o \
	dump.o \
	emit.o \
	encode.o \
	act.o \
	adv.o \
	alt.o \
	atr.o \
	chk.o \
	cla.o \
	cnt.o \
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
	rul.o \
	sco.o \
	scr.o \
	srcp.o \
	stm.o \
	stp.o \
	str.o \
	stx.o \
	sym.o \
	syn.o \
	vrb.o \
	whr.o \
	wht.o \
	wrd.o \
	time.o \
	spa.o


#-- MAIN TARGET
debug all: alan

debug:=	DEBUG= -g
all:=	DEBUG= -O

alan: .smart $(OBJECTS) sysdep.o version.o Makefile
	$(LD) -o $@ $(CFLAGS) $(OBJECTS) sysdep.o version.o $(LDFLAGS)
	cp alan.prod ..
	cp alan ../`arch`

malloc: .smart $(OBJECTS) sysdep.o version.o
	$(CC) -c $(CFLAGS) -DMALLOC alan.c
	$(CC) -o alan $(CFLAGS) $(OBJECTS) sysdep.o version.o /usr/lib/debug/malloc.o $(LDFLAGS)
	-rm alan.o

purify: $(OBJECTS) alan.prod sysdep.o version.o alan.prod
	purify $(CC) -o alan $(CFLAGS) $(OBJECTS) sysdep.o version.o $(LDFLAGS)

NONANSI:
	mv alan.tm alan.tm.ansi
	sed -e s/ANSI-C/C/ alan.tm.ansi > alan.tm
	make .smart
	mv -f alan.tm.ansi alan.tm

MSDOS:
	mv alan.tm alan.tm.ansi
	sed -e s/UNIX,\ BSD,\ SunOS/MSDOS/ alan.tm.ansi > alan.tm
	make .smart
	mv -f alan.tm.ansi alan.tm

version.c: $(OBJECTS) ../[0-9]*
	@co -l -f -q version.c
	@-rm [0-9]*
	@cp ../[0-9]* .
	venum +t
	@ci -u -q -m"." version.c


acode.h: ../interpreter/acode.h
	-co -l acode.h
	-rm -f acode.h
	-ln -s ../interpreter/acode.h
	-ci -u -q -m"." acode.h

../interpreter/acode.h:
	-co -q ../interpreter/acode.h

sysdep.h: ../interpreter/sysdep.h
	-co -l -q sysdep.h
	-rm -f sysdep.h
	-ln -s ../interpreter/sysdep.h
	-ci -u -q -m"." sysdep.h

../interpreter/sysdep.h:
	-co -q ../interpreter/sysdep.h

sysdep.c: ../interpreter/sysdep.c
	-co -l -q sysdep.c
	-rm -f sysdep.c
	-ln -s ../interpreter/sysdep.c
	-ci -u -q -m"." sysdep.c

../interpreter/sysdep.c:
	-co -q ../interpreter/sysdep.c

alan.prod: alan.pml
	sed -e "1,/P R O D/d" -e "/Summary/,$$ d" alan.pml > alan.prod

.smart: alan.grm alan.lex alan.tm alan.pm alan.sm alan.lm
	smartMaker -v alan C $@

#-- Cheating MAKE
link:
	$(CC) -o alan $(CFLAGS) $(OBJECTS) sysdep.o version.o $(LDFLAGS)

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
	-ci -u -q -m"New Release" RCS/*,v
	rcsfreeze v`basename ../[0-9]* | sed s/\\\.//`
	
#--
CLEAN:
	-rm *.o core *.tml *.pml *.sml *~ *.zoo *.bak *.lzh
	-rm alanToken* alanSrcp* alanParse* alanSema* alanScan* alanList* alanErr* alan.voc alan.[ps]mt
	-rcsclean RCS/*,v
	-co -q Makefile

clean:
	rm core *.tml *.sml *~ *.zoo *.bak *.lzh

#--
#FILETER = | sed -e "\,/usr/include, d" -e "s,./,," | sort -u 
FILETER =
depend:
	@-co -q RCS/*,v
	@echo "Updating dependencies.mk, Makefile.amiga"
	@-co -l -q dependencies.mk Makefile.amiga
	@rm dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> "dependencies.mk"; \
	done
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga
	@-ci -m"." -u dependencies.mk Makefile.amiga
	@-co -l -q dependencies.mk

include dependencies.mk

# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

CC	= $(GCC) -pedantic -Wall
#CC	= acc

GCC	= gcc
CPP	= $(GCC) -MM $(INCLUDE)


CFLAGS	= $(DEBUG) $(INCLUDE)

LD	= $(GCC) $(DEBUG)
LDFLAGS	= $(LQ)

sun4 := INCLUDE	= -I/usr/local/include/ansi

tcov := CC = acc -a
tcov := LD = acc -a


.KEEP_STATE:

#--
SRCS = debug.c \
	args.c \
	exe.c \
	inter.c \
	parse.c \
	rules.c \
	stack.c \
	decode.c \
	term.c \
	params.c \
	readline.c \
	c25to26.c \
	sysdep.c

MAINSRC = arun.c

VERSIONSRCS = $(SRCS) $(MAINSRC)

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}


debug purify:=	DEBUG= -g
all release:=	DEBUG= -O

#-- MAIN TARGET
debug all tcov: objarch arun dumpacd
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`


release: objarch arun dumpacd
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`/arun
	cp arun $(ALANDIR)/bin/`arch`/arun$(VERSION)

sun4 solaris1 solaris2 hp: objarch fromsrc

objarch:
	@if test -f .arch; then :; else echo "none" > .arch; fi
	@if test `cat .arch` != `arch`; then \
	    echo "Removing *.o for `cat .arch`!" ; \
	    rm *.o; \
	    echo `arch` > .arch; \
	fi

arun : $(OBJECTS) $(MAINOBJ) .version version.o Makefile
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)

dumpacd: dumpacd.c
	$(CC) -o dumpacd $(CFLAGS) dumpacd.c

purify: $(OBJECTS) version.o $(MAINOBJ)
	purify $(LD) -o arun $(CFLAGS) $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)

malloc: $(OBJECTS) version.o
	$(CC) -c $(CFLAGS) -DMALLOC arun.c
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)

src: version.h

fromsrc: $(OBJECTS) $(MAINOBJ) version.o
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

version.h version.c: ../version.sh
	-chdir ..; rm [0-9]*_[0-9]*_[0-9]*; sh version.sh
	-rm [0-9]*_[0-9]*_[0-9]*
	cp ../[0-9]*_[0-9]*_[0-9]* .
	venum -c

.version: $(SRCS)
	venum +t
	touch .version

#--
amiga: src
	lha u interpreter.lha *.c *.h Makefile Makefile.amiga dependencies.mk

archives: amiga interpreter.zip.

pc:
	ramp iso2pc sysdep.c SYSDEP.C
	zip -k -u interpreter.zip *.[ch] Makefile.dos
	zip -k -m interpreter.zip *.[CH]

#--
CLEAN: clean
	-rm *.o

#--
clean:
	-rm core *~ *.zoo *.bak


#--
#FILETER = | sed -e "\,/usr/include, d" -e "s,./,," | sort -u 
FILETER = 
depend: dependencies.mk. Makefile.amiga Makefile.dos # arun.make

dependencies.mk.:
	@echo Updating dependencies.mk!
	@rm dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER)>> "dependencies.mk"; \
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
	@sed -e s/\\.o/\\.obj/ dependencies.mk >> Makefile.dos

arun.make: dependencies.mk
	@echo Updating arun.make!
	@echo !Automagically created makefile for Arun interpreter on VMS > arun.make
	@echo CQ=/STANDARD=PORTABLE >> arun.make
	@echo OBJECTS = $(VMSOBJECTS) >> arun.make
	@echo >> arun.make
	@cat dependencies.mk >> arun.make
	@echo >> arun.make
	@echo arun.exe : "#(OBJECTS)" version.obj >> arun.make
	@/usr/5bin/echo \\t$$ link/exe=arun "#(LQ)" $(VMSLINKOBJS) >> arun.make
	@/usr/5bin/echo \\t$$ copy arun.exe "<->" >> arun.make

mac:
	refreshMac $(HOME)/mac/Alan/interpreter

include dependencies.mk

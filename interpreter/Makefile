# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

#INCLUDE	= -I/usr/local/include/ansi
CC	= $(GCC) -ansi -pedantic -Wall
#CC	= acc

GCC	= gcc
CPP	= $(GCC) -MM -ansi $(INCLUDE)


CFLAGS	= $(DEBUG) $(INCLUDE)

LD	= $(GCC) $(DEBUG)
LDFLAGS	= $(LQ)

#LDFLAGS = $(LQ) -readline -ltermcap -L$(HOME)/lib/`arch`
LDFLAGS = $(LQ)

sun4 := LDFLAGS = $(LQ) -lreadline -ltermcap -lns -L$(HOME)/lib/`arch` -L/usr/local/lib/4.1.2 -L/usr/local/lib

sun3 := LDFLAGS = $(LQ) -L/usr/local/lib/4.1.2 -L/usr/local/lib

solaris2 := LDFLAGS = $(LQ) -lreadline -ltermcap -lc -lucb -L$(HOME)/lib/`arch`
solaris2 := INCLUDE=-I/usr/include -I$(HOME)/include

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
	sysdep.c

MAINSRC = arun.c

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}


debug purify:=	DEBUG= -g
all release:=	DEBUG= -O

#-- MAIN TARGET
debug all tcov release: objarch arun

sun3 solaris2 hp: objarch fromsrc

objarch:
	if test -f .arch; then :; else echo "none" > .arch; fi
	if test `cat .arch` != `arch`; then \
	    rm *.o; \
	    echo `arch` > .arch; \
	fi
	@echo arch = `arch`

arun : $(OBJECTS) $(MAINOBJ) .version version.o Makefile
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

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

.version: $(OBJECTS)
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

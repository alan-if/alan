# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

SHELL	= /bin/sh

GCC	= gcc
CPP	= $(GCC) -MM -ansi -I/usr/local/include/ansi

CC	= $(GCC) -ansi -pedantic -Wall -I/usr/local/include/ansi

CFLAGS	= $(DEBUG)

LD	= $(GCC)
LDFLAGS	= $(LQ) -L/usr/local/lib/4.1.2

tcov := CC = acc -a
tcov := LD = acc -a

VERSIONFILE = [0-9]*_[0-9]*_[0-9]*

#.KEEP_STATE:

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
	sysdep.c

MAINSRC = arun.c

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}


#-- MAIN TARGET
debug all tcov: arun

debug purify:=	DEBUG= -g
all:=	DEBUG= -O

arun: $(OBJECTS) $(MAINOBJ) version.o Makefile
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

purify: $(OBJECTS) version.o $(MAINOBJ)
	purify $(LD) -o arun $(CFLAGS) $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)

malloc: $(OBJECTS) version.o
	$(CC) -c $(CFLAGS) -DMALLOC arun.c
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)

src: version.c version.h

fromsrc: $(OBJECTS) $(MAINOBJ) version.o
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

version.h:
	@-cd ..; rm $(VERSIONFILE); sh version.sh
	@-rm $(VERSIONFILE)
	@cp ../$(VERSIONFILE) .
	venum -c

version.c: $(OBJECTS) ../$(VERSIONFILE)
	@-rm $(VERSIONFILE)
	@cp ../$(VERSIONFILE) .
	venum +t

#--
archives: src
	-rm Makefile*~
	lha u interpreter.lha *.c *.h Makefile* dependencies.mk
	zip -k -u interpreter.zip *.[ch] Makefile*

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


include dependencies.mk

# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

ARCH	= `uname -sr | sed -e "s% %/%g"`

ARCHS	= sun4 solaris2 #hp
VERSION	= `ls [0-9]*_[0-9]*_[0-9]*.* | tail -1 | sed s/\\\\.//g | sed s/_//g`

GCC	= gcc
CPP	= $(GCC) -MM $(INCLUDE)

CC	= $(GCC) -Wall

OS	= `arch`
CFLAGS	= $(DEBUG) $(INCLUDE) -DCOMPILER=\"[gcc]\" -D__$(OS)__

LD	= $(GCC) $(DEBUG)
LDFLAGS	= $(LQ)

sun4 := INCLUDE	= -I/usr/local/include/ansi
sun4 := DEBUG = -g

tcov := CC = acc -a
tcov := LD = acc -a


#--
SRCS = 	main.c \
	args.c \
	exe.c \
	inter.c \
	parse.c \
	rules.c \
	stack.c \
	decode.c \
	params.c \
	debug.c \
	sysdep.c \
	readline.c \
	term.c

MAINSRC = arun.c

VERSIONSRCS = $(SRCS) $(MAINSRC)

OBJECTS = ${VERSIONSRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}
VMSOBJECTS = ${SRCS:.c=.obj}
VMSLINKOBJS = ${SRCS:.c=}


debug purify:=	DEBUG= -g
all release:=	DEBUG= -O
dumpacd:=	DEBUG= -g

#-- MAIN TARGET
debug all tcov: objarch arun dumpacd
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

arch:
	make `arch`

release:
	@-for a in $(ARCHS) ; do \
		if fresh $(ALANDIR)/bin/$$a/arun \: $(SRCS) ; then \
			cp $(ALANDIR)/bin/$$a/arun $(ALANDIR)/bin/$$a/arun$(VERSION) ; \
		else \
			echo $(ALANDIR)/bin/$$a/arun is not fresh! ; \
			touch ../.notfresh ; \
		fi ; \
	done

sun4 solaris1 solaris2 hp: objarch fromsrc

objarch:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test `cat .arch` != $(ARCH); then \
	    echo Removing objects for `cat .arch`, building for $(ARCH)... ; \
	    rm $(OBJECTS) version.o; \
	    echo $(ARCH) > .arch; \
	fi

arun : $(OBJECTS) .version version.o Makefile
	$(LD) -o arun $(OBJECTS) version.o $(LDFLAGS)

dumpacd: dumpacd.o spa.o
	$(CC) -o dumpacd $(CFLAGS) dumpacd.o spa.o

genalan: genalan.o spa.o
	$(CC) -o genalan $(CFLAGS) genalan.o spa.o

purify: $(OBJECTS) version.o $(MAINOBJ)
	purify $(LD) -o arun $(OBJECTS) version.o $(LDFLAGS)

malloc: $(OBJECTS) version.o
	$(CC) -c $(CFLAGS) -DMALLOC arun.c
	$(LD) -o arun $(MAINOBJ) version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)

src: version.h

fromsrc: $(OBJECTS) version.o
	$(LD) -o arun $(OBJECTS) version.o $(LDFLAGS)
	@-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp arun $(ALANDIR)/bin/`arch`

version.h version.c: ../version.sh
	-chdir ..; rm [0-9]*_[0-9]*_[0-9]*.*; sh version.sh
	-rm [0-9]*_[0-9]*_[0-9]*.*
	cp ../[0-9]*_[0-9]*_[0-9]*.* .
	venum -c

.version: $(SRCS)
	venum +t
	touch .version

spa.o: spa.h spa.c

spa.h: ../compiler/spa.h
	ln -s ../compiler/spa.h spa.h

spa.c: ../compiler/spa.c
	ln -s ../compiler/spa.c spa.c

#--
amiga: src
	lha u interpreter.lha *.c *.h Makefile Makefile.amiga dependencies.mk

archives: amiga interpreter.zip.

pc:	src
	ramp iso2pc sysdep.c SYSDEP.C
	zip -k -u interpreter.zip *.[ch] Makefile.dos makefile.gpp
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
depend: dependencies.mk. Makefile.amiga makefile.gpp Makefile.mgw

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

makefile.gpp: dependencies.mk Makefile
	@echo Updating makefile.gpp!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' makefile.gpp > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cp "#Makefile.tmp#" makefile.gpp
	@sed -e s/\\.o/\\.obj/ dependencies.mk >> makefile.gpp

Makefile.mgw: dependencies.mk Makefile
	@echo Updating Makefile.mgw!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.mgw > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cp "#Makefile.tmp#" Makefile.mgw
	@sed $(DOSSUBS) -e s/\\.o/\\.obj/ dependencies.mk >> Makefile.mgw


mac:
	-mkdir macsrc
	refreshMac macsrc
	-cd macsrc; sit *.[ch]
	mv macsrc/archive.sit interpreter.sit


include dependencies.mk

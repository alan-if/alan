# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

%: RCS/%,v
	@co -q $<

SHELL	= /bin/sh
CPP	= gcc -MM

#CC	= acc -Xc -vc
CC	= gcc -ansi -pedantic -Wall -I/usr/local/include/ansi
CFLAGS	= $(DEBUG)

LD	= $(CC) $(DEBUG)
LDFLAGS	= -lm $(LQ)

.KEEP_STATE:

#--
SRCS = debug.o exe.o inter.o parse.o rules.o stack.o decode.o term.o params.o

MAINSRC = arun.o

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}

LIB = arun.a

.PRECIOUS: $(LIB)


#-- MAIN TARGET
debug all: arun

debug:=	DEBUG= -g
all:=	DEBUG= -O

arun: $(LIB)($(OBJECTS)) sysdep.o version.o $(MAINOBJ) Makefile
	ranlib $(LIB)
	$(LD) -o $@ $(MAINOBJ) sysdep.o version.o $(LIB) $(LDFLAGS)
	cp arun ../`arch`

purify: $LIB($(OBJECTS)) sysdep.o version.o $(MAINOBJ)
	ranlib $(LIB)
	purify $(LD) -o arun $(CFLAGS) $(MAINOBJ) sysdep.o version.o $(LIB) $(LDFLAGS)

malloc	: $(OBJECTS) sysdep.o version.o
	$(CC) -c $(CFLAGS) -DMALLOC arun.c
	$(LD) -o $@ $(MAINOBJ) sysdep.o version.o $(LIB) /usr/lib/debug/malloc.o $(LDFLAGS)
	cp arun ../`arch`

version.c: $(LIB)($(OBJECTS)) ../[0-9]*
	@co -l -f -q version.c
	@-rm [0-9]*
	@cp ../[0-9]* .
	venum +t
	@ci -u -q -m"." version.c


#--
archives:
	-\rm Makefile*~
	lha c interpreter.lha *.c *.h Makefile* dependencies.mk
	zip -k interpreter.zip *.[ch] Makefile.cmd

#-- release
release:
	-ci -u -q -m"New Release" RCS/*,v
	rcsfreeze v`basename ../[0-9]* | sed s/\\\.//`


#--
CLEAN:
	-rm [0-9]* *.o core *.tml *.sml *~ *.zoo *.bak
	-rcsclean RCS/*,v
	-co -q Makefile
#--
clean:
	rm core *.tml *.sml *~ *.zoo *.bak


#--
#FILETER = | sed -e "\,/usr/include, d" -e "s,./,," | sort -u 
FILETER = 
depend:
	@echo "make: Updating dependencies.mk, Makefile.amiga"
	@-co -l -f -q dependencies.mk Makefile.amiga
	@rm dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER)>> "dependencies.mk"; \
	done
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga
	@-ci -m"." -u dependencies.mk Makefile.amiga

#include dependencies.mk

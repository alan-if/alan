# Makefile for arun			      Date: 1990-10-16/thoni@rabbit
#

SHELL	= /bin/sh
CPP	= gcc -MM

purify:=		CC = acc -Xc -vc
debug all release:=	CC = gcc -ansi -pedantic -Wall -I/usr/local/include/ansi
CFLAGS	= $(DEBUG)

LD	= $(CC) $(DEBUG)
LDFLAGS	= -lm $(LQ)

#.KEEP_STATE:

#--
SRCS = debug.c \
	exe.c \
	inter.c \
	parse.c \
	reverse.c \
	rules.c \
	stack.c \
	decode.c \
	term.c \
	params.c \
	sysdep.c

MAINSRC = arun.c

OBJECTS = ${SRCS:.c=.o}
MAINOBJ = ${MAINSRC:.c=.o}

LIB = arun.a

.PRECIOUS: $(LIB)


#-- MAIN TARGET
debug all: arun

debug:=	DEBUG= -g
all:=	DEBUG= -O

arun: $(OBJECTS) version.o $(MAINOBJ) Makefile
	$(LD) -o $@ $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)
	cp arun ../`arch`

purify: $(OBJECTS) version.o $(MAINOBJ)
	purify $(LD) -o arun $(CFLAGS) $(MAINOBJ) version.o $(OBJECTS) $(LDFLAGS)

malloc: $(OBJECTS) version.o
	$(CC) -c $(CFLAGS) -DMALLOC arun.c
	$(LD) -o $@ $(MAINOBJ) version.o $(OBJECTS) /usr/lib/debug/malloc.o $(LDFLAGS)
	cp arun ../`arch`

version.c: $(OBJECTS) ../[0-9]*
	@-rm [0-9]*
	@cp ../[0-9]* .
	venum +t

#--
archives:
	-\rm Makefile*~
	lha u interpreter.lha *.c *.h Makefile* dependencies.mk
	zip -k -f interpreter.zip *.[ch] Makefile.cmd

#--
CLEAN: clean
	-rm interpreter.a

#--
clean:
	rm core *.tml *.sml *~ *.zoo *.bak


#--
#FILETER = | sed -e "\,/usr/include, d" -e "s,./,," | sort -u 
FILETER = 
depend:
	@echo "make: Updating dependencies.mk, Makefile.amiga"
	@rm dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER)>> "dependencies.mk"; \
	done
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga

include dependencies.mk

# Makefile for alan converter
#
# a2a3 is an Alan v2 to v3 converter that works on source code.
# It is built using the same parser generator tools that the
# Alan compiler is using, which are only available on ThoNi's
# machines. To build using those use the Makefile.thoni.
# This makefile instead assumes all sources are generated and
# only builds from C source.

OPTIMIZE = -g

win:	PLATFORM = -DWINGUI -mwindows -mno-cygwin
release:	OPTIMIZE = -O4
gcov:	GCOV = -fprofile-arcs -ftest-coverage

CFLAGS 	= $(OPTIMIZE) -Wall $(GCOV) -funsigned-char $(INCLUDE) $(PLATFORM)
LDFLAGS	= $(OPTIMIZE) $(PLATFORM)

CPP	= gcc -MM -ansi
LD	= gcc

TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c\
	lmList.c


# Sources required for both builds
BUILDSRCS = \
	a2a3.c \
	str.c \
	lst.c \
	util.c

MAINSRCS = $(TMCSRCS) $(BUILDSRCS)
MAINOBJECTS = ${MAINSRCS:.c=.o} a2a3.version.o

VERSIONSRCS = $(ALANSRCS) $(BUILDSRCS) $(TMSRCS)


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main targets
#

all : test a2a3 Makefile

build : a2a3

win: checkTarget $(MAINOBJECTS)
	$(LD) -o a2a3 $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp a2a3 ../bin

a2a3: checkTarget $(MAINOBJECTS)
	$(LD) -o a2a3 $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS) 
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp a2a3 ../bin

release: win

clean:
	-rm *.o

test : 
	cd testing ; ../../bin/regr

.PHONY: checkTarget
ARCH	= `echo $(PLATFORM) $(OPTIMIZE)`
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for \"`cat .arch`\", building for \"$(ARCH)\"... ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi


#
# Common sysdep
#
sysdep.c : ../interpreter/sysdep.c
	cp -f ../interpreter/sysdep.c .
	chmod a-w sysdep.c

sysdep.h : ../interpreter/sysdep.h
	cp -f ../interpreter/sysdep.h .
	chmod a-w sysdep.h
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

#
# Version number generation
#
a2a3.version.h a2a3.version.c:
	venum -write always -all a2a3

version.h :
	venum -write always -all a2a3

a2a3.version: $(VERSIONSRCS) 
	venum alan time


#
include dependencies.mk

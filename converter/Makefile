# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

PLATFORM = -mno-cygwin
OPTIMIZE = -g

win:	PLATFORM = -DWINGUI -mwindows
release:	OPTIMIZE = -O4
gcov:	GCOV = -fprofile-arcs -ftest-coverage

CFLAGS 	= $(OPTIMIZE) -Wall $(GCOV) -funsigned-char $(INCLUDE) $(PLATFORM)
LDFLAGS	= $(OPTIMIZE) $(PLATFORM)

ARCH	= $(PLATFORM) $(OPTIMIZE)

CPP	= gcc -MM -ansi
LD	= gcc

TMSRCS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

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

win: checkTarget tm $(MAINOBJECTS)
	$(LD) -o a2a3 $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp a2a3.exe ../bin

a2a3: checkTarget tm $(MAINOBJECTS)
	$(LD) -o a2a3 $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS) 
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp a2a3.exe ../bin

release: win

all : .tmstamp test alan Makefile

clean:
	-rm *.o

.tmstamp: $(TMSRCS)
	$(MAKE) -f Makefile.tm

tm:
	make -f Makefile.tm

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for `cat .arch`, building for $(ARCH)... ; \
		rm *.o; \
		echo $(ARCH) > .arch; \
	fi


#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
depend:
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

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

# Makefile for Alan interpreter to be built with CygWin32/Mingw32
# Note: uses a lot of special stuff, if you are porting, start from
# one of the other makefiles, such as Makefile.unix

VERSION = `cd ..; venum alan -print "\\$$v.\\$$r.\\$$s\\$$c"`

OPTIMIZE = -g
#CYGWIN	= -mno-cygwin
CYGWIN	= 
EXE	= arun.exe

COMPILEFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall -funsigned-char $(CRCLOG)
CFLAGS	= $(COMPILEFLAGS)
ARCH	= $(CYGWIN)$(OPTIMIZE)$(GLKDEFS)
LIBS	=

#Settings for Windows GLK variant
WINGLKROOT = ../../WindowsGLK
WINGLKDEFS = -DHAVE_WINGLK
WINGLKINCLUDE = -I$(WINGLKROOT)/Include
WINGLKLIB = $(WINGLKROOT)/Glk.lib

# Settings for TermGLK
TERMGLKROOT = ../../glkterm
TERMGLKDEFS = -DHAVE_GLK
TERMGLKINCLUDE = -I$(TERMGLKROOT)
TERMGLKLIB = -L$(TERMGLKROOT) -lglkterm -lncurses


#Set up for specific target
winarun: GLKDEFS = $(WINGLKDEFS)
winarun: CFLAGS = $(COMPILEFLAGS) $(WINGLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mno-cygwin -mwindows
winarun: LIBS = $(WINGLKLIB)
winarun: EXE = winarun.exe

termarun: GLKDEFS = $(TERMGLKDEFS)
termarun: CFLAGS = $(COMPILEFLAGS) $(TERMGLKINCLUDE) $(GLKDEFS)
termarun: CYGWIN = 
termarun: LIBS = $(TERMGLKLIB)
termarun: EXE = termarun.exe

dmalloc: CFLAGS = $(COMPILEFLAGS) -DDMALLOC
dmalloc: LIBS = /usr/local/lib/libdmalloc.a
release: OPTIMIZE = -O4

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


CPP	= gcc -MM -ansi

LDFLAGS = $(CYGWIN) $(VERBOSE)

GLKSRCS = glkstart.c glkio.c

# Build sources are in main build and in unit test
BUILDSRCS = \
	rules.c \
	debug.c \
	args.c \
	decode.c \
	term.c \
	readline.c \
	params.c \
	act.c

# Main sources are only in main build because unit #includes them
MAINSRCS = \
	exe.c \
	sysdep.c \
	stack.c \
	inter.c \
	reverse.c \
	syserr.c \
	parse.c \
	main.c \
	set.c \
	arun.c

VERSIONSRCS = $(MAINSRCS) $(BUILDSRCS)
VERSIONOBJECTS = ${VERSIONSRCS:.c=.o}
ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}
TERMARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}

all arun: checkTarget $(ARUNOBJECTS) unitTest dumpacd Makefile dependencies.mk
	gcc -o arun $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp arun.exe ../bin

release:
	$(MAKE) "OPTIMIZE=-O4" arun
	$(MAKE) "OPTIMIZE=-O4" winarun

winarun: checkTarget $(WINARUNOBJECTS) winglk.o arun.res
	gcc -o $(EXE) $(OPTIMIZE) $(WINARUNOBJECTS) winglk.o arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $(EXE) ../bin

termarun: checkTarget $(TERMARUNOBJECTS) arun.res
	gcc -o $(EXE) $(OPTIMIZE) $(TERMARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $(EXE) ../bin

dmalloc: checkTarget $(ARUNOBJECTS)
	gcc -o arun $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp arun.exe ../bin

arun.res: arun.rc arun.ico resources.h alan.version.c
	echo $(VERSION)
	sed "s/WINARUNVERSION/$(VERSION)/" arun.rc > arun.rc2
	windres arun.rc2 -O coff -o $@

clean:
	-rm *.o

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for \'`cat .arch`\', re-building for \'$(ARCH)\'... ; \
		rm *.o; \
		echo $(ARCH) > .arch; \
	fi


#
# Version number generation
#
alan.version.h :
	cd ..; venum -write always -all alan
	cp -p ../alan.version.h .

version.h :
	cd ..; venum -write always -all alan
	cp -p ../version.h .

../alan.version: $(VERSIONSRCS) 
	cd ..; venum alan time

alan.version.c:  ../alan.version
	cp -p ../alan.version.c .

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#

# Sources for the test framework
UNITSRCS = unit.c

TESTSRCS = $(UNITSRCS) $(BUILDSRCS)
TESTOBJECTS = ${TESTSRCS:.c=.o} alan.version.o

unit : checkTarget $(TESTOBJECTS)
	gcc -o unit $(TESTOBJECTS) $(LDFLAGS) $(LIBS)

unitTest: unit
	@./unit

# Run all tests!
test: unitTest


#
#
#
#
dumpacd: dumpacd.c reverse.o ../compiler/spa.o acode.h sysdep.o
	gcc -Wall -o dumpacd -g -mno-cygwin -I../compiler dumpacd.c ../compiler/spa.o reverse.o sysdep.o



#
# Dependencies
#
depend:
	@-for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS) $(GLKSRCS) dumpacd.c reverse.c; \
	  do \
	  $(CPP) $(CFLAGS) $(WINGLKINCLUDE) $$f >> dependencies.new; \
	  $(CPP) $(WINGLKDEFS) $(CFLAGS) $(WINGLKINCLUDE) -I../compiler $$f >> dependencies.new; \
	  $(CPP) $(TERMGLKDEFS) $(CFLAGS) $(TERMGLKINCLUDE) -I../compiler $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

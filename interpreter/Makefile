# Makefile for Alan interpreter to be built with CygWin32/Mingw32
# Note: uses a lot of special stuff, if you are porting, start from
# one of the other makefiles, such as Makefile.unix

VERSION = `cd ..; venum alan -print "\\$$v.\\$$r.\\$$c(\\$$s)"`

OPTIMIZE = -g
#CYGWIN	= -mno-cygwin
CYGWIN	= 
EXE	= arun.exe

COMPILEFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall -funsigned-char $(CRCLOG)
CFLAGS	= $(COMPILEFLAGS)
ARCH	= $(CYGWIN)$(OPTIMIZE)

#Settings for Windows GLK variant
GLKROOT = ../../WindowsGLK
GLKDEFS = -DHAVE_GLK -DHAVE_WINGLK
GLKINCLUDE = -I$(GLKROOT)/Include
GLKLIB = $(GLKROOT)/Glk.lib
winarun: CFLAGS = $(COMPILEFLAGS) $(GLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mwindows -mno-cygwin
winarun: LIB = $(LIB) $(GLKLIB)
winarun: EXE = winarun.exe
dmalloc: CFLAGS = $(COMPILEFLAGS) -DDMALLOC
dmalloc: LIB = /usr/local/lib/libdmalloc.a
release: OPTIMIZE = -O4

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


CPP	= gcc -MM -ansi

LDFLAGS = $(CYGWIN) $(VERBOSE)

GLKSRCS = glkstart.c glkio.c

# Build sources are in main buid an in unit test
BUILDSRCS = \
	rules.c \
	debug.c \
	args.c \
	parse.c \
	decode.c \
	term.c \
	readline.c \
	params.c \
	act.c \
	main.c

# Main sources are only in main build because unit #includes them
MAINSRCS = \
	exe.c \
	sysdep.c \
	stack.c \
	inter.c \
	reverse.c \
	syserr.c \
	arun.c

VERSIONSRCS= $(MAINSRCS) $(BUILDSRCS)
VERSIONOBJECTS= ${VERSIONSRCS:.c=.o}
ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}

all arun: checkTarget $(ARUNOBJECTS) unitTest dumpacd Makefile dependencies.mk
	gcc -o arun $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIB)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp arun.exe ../bin

release:
	$(MAKE) "OPTIMIZE=-O4" arun
	$(MAKE) "OPTIMIZE=-O4" winarun

winarun: checkTarget $(WINARUNOBJECTS) winglk.o arun.res
	gcc -o winarun $(OPTIMIZE) $(WINARUNOBJECTS) winglk.o arun.res $(LDFLAGS) $(GLKLIB)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp winarun.exe ../bin

dmalloc: checkTarget $(ARUNOBJECTS)
	gcc -o arun $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIB)
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
	gcc -o unit $(TESTOBJECTS) $(LDFLAGS) $(LIB)

unitTest: unit
	@./unit

# Run all tests!
test: unitTest


#
#
#
#
dumpacd: dumpacd.c reverse.o ../compiler/spa.o acode.h
	gcc -o dumpacd -g -mno-cygwin -I../compiler dumpacd.c ../compiler/spa.o reverse.o



#
# Dependencies
#
depend:
	@-for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS) $(GLKSRCS) dumpacd.c reverse.c; \
	  do \
	  $(CPP) $(CFLAGS) $(GLKINCLUDE) $$f >> dependencies.new; \
	  $(CPP) $(GLKDEFS) $(CFLAGS) $(GLKINCLUDE) -I../compiler $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

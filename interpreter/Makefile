# Makefile for Alan interpreter to be built with CygWin32/Mingw32

CYGWIN = #-DCOMPILER=\"[GCC/CygWin32]\"
OPTIMIZE = -g

GLKROOT = ../../WindowsGLK
GLKDEFS = -DGLK -DWINGLK
GLKINCLUDE = -I$(GLKROOT)/Include
GLKLIB = $(GLKROOT)/Glk.lib

CFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall -funsigned-char $(CRCLOG)

#Settings for Windows GLK variant
winarun: CFLAGS = $(CFLAGS) $(GLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mwindows -mno-cygwin
winarun: LIB = $(LIB) $(GLKLIB)

#-DCOMPILER=\"[GCC/Mingw32]\" 

release: OPTIMIZE = -O4

ARCH	= $(CYGWIN)$(OPTIMIZE)

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


CPP	= gcc -MM -ansi

LDFLAGS = $(CYGWIN) $(VERBOSE)

GLKSRCS = glkstart.c glkio.c

BUILDSRCS = \
	sysdep.c \
	rules.c \
	debug.c \
	args.c \
	parse.c \
	decode.c \
	term.c \
	reverse.c \
	readline.c \
	params.c \
	act.c \
	main.c

MAINSRCS = exe.c \
	stack.c \
	inter.c \
	arun.c

VERSIONSRCS= $(MAINSRCS) $(BUILDSRCS)
VERSIONOBJECTS= ${VERSIONSRCS:.c=.o}
ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}

all: checkTarget unitTest arun

arun : $(ARUNOBJECTS) Makefile dependencies.mk
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

arun.res: arun.rc arun.ico
	windres $< -O coff -o $@

clean:
	-rm *.o

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects, re-building for $(ARCH)... ; \
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
dumpacd: dumpacd.c reverse.o ../compiler/spa.o
	gcc -o dumpacd -g -mno-cygwin -I../compiler dumpacd.c ../compiler/spa.o reverse.o



#
# Dependencies
#
depend:
	@-for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS) $(GLKSRCS); \
	  do \
	  $(CPP) $(CFLAGS) $(GLKINCLUDE) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

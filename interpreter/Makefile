# Makefile for Alan interpreter to be built with CygWin32/Mingw32

CYGWIN =  -DCOMPILER=\"[GCC/CygWin32]\"

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG

CFLAGS = -g -Wall -funsigned-char $(CYGWIN) $(CRCLOG) $(GLKINCLUDE) $(GLK)

CPP	= gcc -MM -ansi

LDFLAGS = -g $(CYGWIN) $(VERBOSE)
LIB = $(GLKLIB)

GLKSRCS = glkstart.c glkio.c

BUILDSRCS = \
	sysdep.c \
	rules.c \
	debug.c \
	args.c \
	inter.c \
	parse.c \
	stack.c \
	decode.c \
	term.c \
	reverse.c \
	readline.c \
	params.c \
	main.c

MAINSRCS = exe.c \
	arun.c

VERSIONSRCS= $(MAINSRCS) $(BUILDSRCS)
VERSIONOBJECTS= ${VERSIONSRCS:.c=.o}
ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}

arun : $(ARUNOBJECTS) Makefile dependencies.mk
	gcc -o arun $(ARUNOBJECTS) $(LDFLAGS) $(LIB)
	@-mkdir ../bin 2> /dev/null
	cp arun.exe ../bin

winglk: GLK = -DGLK
winglk: GLKINCLUDE = -I../../WindowsGLK/Include
winglk: GLKLIB = ../../WindowsGlk/Glk.lib
winglk: CYGWIN = -DCOMPILER=\"[GCC/Mingw32]\" -mno-cygwin

winglk: $(GLKOBJECTS) $(ARUNOBJECTS) winglk.o arun.res
	gcc -o arun -mwindows $^ $(LDFLAGS) $(GLKLIB)
	@-mkdir ../bin 2> /dev/null
	cp arun.exe ../bin

arun.res: arun.rc arun.ico
	windres $< -O coff -o $@

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

strip:
	strip arun
	coff2exe arun

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
TESTOBJS = ${TESTSRCS:.c=.o} alan.version.o

unit : $(TESTOBJS)

# Run all unit tests!
test: unit
	@unit


#
# Dependencies
#
depend:
	@for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS); \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

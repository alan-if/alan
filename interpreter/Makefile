# Makefile for Alan interpreter to be built with CygWin32/Mingw32

CYGWIN = #-DCOMPILER=\"[GCC/CygWin32]\"
OPTIMIZE = -g
GLKROOT = ../../WindowsGLK

#Settings for Windows GLK variant
winglk: GLK = -DGLK -DWINGLK
winglk: GLKINCLUDE = -I$(GLKROOT)/Include
winglk: GLKLIB = $(GLKROOT)/Glk.lib
winglk: CYGWIN = #-mno-cygwin
#-DCOMPILER=\"[GCC/Mingw32]\" 

release: OPTIMIZE = -O4

ARCH	= $(CYGWIN)

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


CFLAGS = $(OPTIMIZE) -Wall -funsigned-char $(CYGWIN) $(CRCLOG) $(GLKINCLUDE) $(GLK)

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

arun : checkTarget $(ARUNOBJECTS) Makefile dependencies.mk unit
	gcc -o arun $(ARUNOBJECTS) $(LDFLAGS) $(LIB)
	@-mkdir ../bin 2> /dev/null
	cp arun.exe ../bin

release: winglk

winglk: checkTarget $(GLKOBJECTS) $(ARUNOBJECTS) winglk.o arun.res
	gcc -o arun -mwindows $(GLKOBJECTS) $(ARUNOBJECTS) winglk.o arun.res $(LDFLAGS) $(GLKLIB)
	@-mkdir ../bin 2> /dev/null
	cp arun.exe ../bin

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

unit : $(TESTOBJECTS)
	gcc -o unit $(TESTOBJECTS) $(LDFLAGS) $(LIB)
	@./unit

# Run all tests!
test: unit


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
	@for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS); \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

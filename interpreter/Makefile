# Makefile for Alan interpreter to be built with CygWin

CYGWIN = #-DCOMPILER=\"[GCC/CygWin32]\"
OPTIMIZE = -g

GLKROOT = /home/thoni/Alan/WindowsGLK
GLKDEFS = -DGLK -DWINGLK
GLKINCLUDE = -I$(GLKROOT)/Include
GLKLIB = $(GLKROOT)/Glk.lib

CFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall -funsigned-char $(CRCLOG)

#Settings for Windows GLK variant
winarun: CFLAGS = $(OPTIMIZE) $(CYGWIN) $(GLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mwindows -mno-cygwin
winarun: LIB = $(LIB) $(GLKLIB)

#-DCOMPILER=\"[GCC/Mingw32]\"

release: OPTIMIZE = -O4

ARCH	= $(CYGWIN)$(OPTIMIZE)

# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG

CFLAGS = -g -Wall -funsigned-char $(CYGWIN) $(CRCLOG)

CPP	= gcc -MM -ansi

LDFLAGS = $(CYGWIN) $(VERBOSE)

GLKSRCS = glkstart.c glkio.c

VERSIONOBJECTS = arun.o \
	main.o \
	debug.o \
	args.o \
	exe.o \
	inter.o \
	parse.o \
	rules.o \
	stack.o \
	decode.o \
	term.o \
	reverse.o \
	readline.o \
	params.o \
	sysdep.o

ARUNOBJECTS = $(VERSIONOBJECTS) alan.version.o
GLKOBJECTS = ${GLKSRCS:.c=.o}
WINARUNOBJECTS = ${GLKOBJECTS} ${ARUNOBJECTS}


all: checkTarget arun

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

arun.res: arun.rc arun.ico resources.h
	windres $< -O coff -o $@

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

clean:
	-rm *.o

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects, re-building for $(ARCH)... ; \
		rm *.o; \
		echo $(ARCH) > .arch; \
	fi



dumpacd: dumpacd.o reverse.o spa.o
	gcc -o dumpacd -g dumpacd.o spa.o reverse.o


#
# Dependencies
#
depend:
	@-for f in $(UNITSRCS) $(BUILDSRCS) $(MAINSRCS) $(GLKSRCS); \
	  do \
	  $(CPP) $(CFLAGS) $(GLKINCLUDE) $$f >> dependencies.new; \
	  $(CPP) $(GLKDEFS) $(CFLAGS) $(GLKINCLUDE) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

include dependencies.mk

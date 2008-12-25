# Makefile for Alan interpreter to be built with CygWin32/Mingw32
# Note: uses a lot of special stuff, if you are porting, start from
# one of the other makefiles, such as Makefile.unix

VERSION = `cd ..; venum alan -print "\\$$v.\\$$r\\$$s\\$$c"`
SYSTEM = `uname -s`

CYGWIN	= #-mno-cygwin

OPTIMIZE = -ggdb
CC = gcc

EXE	= arun.exe

COMPILEFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall -funsigned-char $(CRCLOG)
CFLAGS	= $(COMPILEFLAGS)
ARCH	= $(CYGWIN)$(OPTIMIZE)$(GLKDEFS)$(DMALLOC)
LIBS	=

#######################################################################
#
#Set up for specific targets
#
#Settings for Windows GLK variant
WINGLKROOT = ../../WindowsGLK
WINGLKDEFS = -DHAVE_WINGLK
WINGLKINCLUDE = -I$(WINGLKROOT)/Include
WINGLKLIB = $(WINGLKROOT)/Glk.lib

winarun: GLKDEFS = $(WINGLKDEFS)
winarun: CFLAGS = $(COMPILEFLAGS) $(WINGLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mwindows -mno-cygwin 
winarun: LIBS = $(WINGLKLIB)
winarun: EXE = winarun.exe


# Settings for TermGLK
TERMGLKROOT = ../../glkterm
TERMGLKDEFS = -DHAVE_GLK
TERMGLKINCLUDE = -I$(TERMGLKROOT)
TERMGLKLIB = -L$(TERMGLKROOT) -lglkterm -lncurses

glkarun: GLKDEFS = $(TERMGLKDEFS)
glkarun: CFLAGS = $(COMPILEFLAGS) $(TERMGLKINCLUDE) $(GLKDEFS)
glkarun: CYGWIN = 
glkarun: LIBS = $(TERMGLKLIB)
glkarun: EXE = glkarun.exe


#Settings for Gargoyle GLK variant (experimental!)
GARGLKROOT = ../../Gargoyle-2006-09-17/garglk
GARGLKDEFS = -DHAVE_GARGLK -DHAVE_GLK -mno-cygwin -mwindows
GARGLKINCLUDE = -I$(GARGLKROOT)
GARGLKLIB = -L$(GARGLKROOT)/../build/cygwin.release/garglk -lgarglkmain -lpng12 -lgtk -lgdk `pkg-config --libs freetype2` -ljpeg /home/thoni/fmodapi374win/api/lib/libfmod.a -lgarglk -mno-cygwin

gararun: GLKDEFS = $(GARGLKDEFS)
gararun: CFLAGS = $(COMPILEFLAGS) $(GARGLKINCLUDE) $(GLKDEFS)
gararun: CYGWIN = 
gararun: LIBS = $(GARGLKLIB)
gararun: EXE = gararun.exe


#Settings for memory debugging
dmalloc: DMALLOC = -DDMALLOC
dmalloc: CFLAGS = $(COMPILEFLAGS) $(DMALLOC)
dmalloc: LIBS = /usr/local/lib/libdmalloc.a


# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


CPP	= gcc -MM -ansi

LDFLAGS = $(CYGWIN) $(VERBOSE)

GLKSRCS = glkstart.c glkio.c


# Include common source lists 
include sources.mk


###############################################################
all:
	$(MAKE) $(SYSTEM)

Darwin: arun #dumpacd


Cygwin: arun winarun glkarun gararun dumpacd


release:
	$(MAKE) "OPTIMIZE=-O4" $(SYSTEM)



arun: checkTarget $(ARUNOBJECTS) Makefile dependencies.mk #unitTests
	gcc -o $@ $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	-@cp $@ ../bin


winarun: checkTarget $(WINARUNOBJECTS) winglk.o arun.res
	gcc -o $@ $(OPTIMIZE) $(WINARUNOBJECTS) winglk.o arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $@ ../bin

glkarun: checkTarget $(TERMARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(TERMARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $@ ../bin

gararun: checkTarget $(GARARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(GARARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $@ ../bin

dmalloc: checkTarget $(ARUNOBJECTS) unitTest
	gcc -o arun $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp arun.exe ../bin

arun.res: arun.rc arun.ico resources.h alan.version.c
	echo $(VERSION)
	sed "s/WINARUNVERSION/$(VERSION)/" arun.rc > arun.rc2
	windres arun.rc2 -O coff -o $@


about: about.c arun.res
	gcc -o $@ -ggdb -mno-cygwin -mwindows about.c arun.res

headerGenerator: headerGenerator.c

clean:
	-rm *.o

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for \'`cat .arch`\', re-building for \'$(ARCH)\'... ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi


#
# Version number generation
#
alan.version.h :
	-@if test "`which venum`" != "" ; then \
		cd ..; venum -write always -all alan ; \
		cp -p ../alan.version.h . ; \
	fi

version.h :
	-@if test "`which venum`" != "" ; then \
		cd ..; venum -write always -all alan ; \
		cp -p ../version.h . ; \
	fi

../alan.version: $(VERSIONSRCS) 
	-@if test "`which venum`" != "" ; then \
		cd ..; venum alan time ; \
	fi

alan.version.c:  ../alan.version
#	cp -p ../alan.version.c .

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# CGreen unit tests
#
CGREENROOT = ../Cgreen
CGREENINCLUDE = -I$(CGREENROOT)/include
unit2: CYGWIN = #-mno-cygwin
unit2: CFLAGS = $(COMPILEFLAGS) $(CGREENINCLUDE) $(CYGWIN)
unit2: LIBS = $(CGREENROOT)/libcgreen.a

unit2: checkTarget $(UNIT2OBJECTS) Makefile $(LIBS)
	gcc -o unit2 $(UNIT2OBJECTS) $(LDFLAGS) $(LIBS)
	@./unit2

unitTest:
	-@if test -d $(CGREENROOT) ; then \
		make unit2 ; \
	fi

#+++++++++++++++++++++++++++++++++
#
# Run all tests!
#
test: unitTest


#
#
#
#
dumpacd: dumpacd.c reverse.o ../compiler/spa.c acode.h sysdep.c
	gcc -Wall -o dumpacd -ggdb -DSTDIONONCONST dumpacd.c ../compiler/spa.c reverse.c sysdep.c
	cp $@ ../bin




#
# Dependencies are generated only for command line case
# Dependencies on libraries are only in the way for other cases,
# so special dependencies that we need are set explicitly below
#
DEPENDINCLUDES = $(WINGLKINCLUDE) $(CGREENINCLUDE)
depend:
	@-for f in $(UNIT2SRC) $(MAINSRCS) $(GLKSRCS) dumpacd.c reverse.c ; \
	  do \
	  $(CPP) $(CFLAGS) $(DEPENDINCLUDES) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

# Extra dependencies for WinGLK case
readline.o : resources.h

include dependencies.mk

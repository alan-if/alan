# Makefile for Alan interpreters, arun, winarun et al.
# Windows: CygWin32/Mingw32
# Darwin: GCC
# Note: uses a lot of special stuff, if you are porting, start from
# one of the other makefiles, such as Makefile.unix (which are not up-to-date yet)

VERSION = `cd ..; venum alan -print "\\$$v.\\$$r\\$$s\\$$c"`

# The SYSTEM is used to find out which applications can be built
# On cygwin the flag -o gives a better name
SYSTEM = `uname -s | sed 's/_.*$$//'`

# On cygwin we try to build without cygwin1.dll if we can, applications can unset this if they need to
PLATFORMFLAG = -mno-cygwin

OPTIMIZE = -ggdb
STDCOMPILEFLAGS = $(OPTIMIZE) $(PLATFORMFLAG) $(CRCLOG) $(SMARTALLOC) -funsigned-char
COMPILEFLAGS = $(STDCOMPILEFLAGS)
DEPENDFLAGS = -MM
CFLAGS	= $(COMPILEFLAGS) -Wall
CC = gcc
LDFLAGS = $(PLATFORMFLAG)


#Settings for memory debugging which is used unless building RELEASE
SMARTALLOC = -DSMARTALLOC


# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


# Include common source lists 
include sources.mk


###############################################################
all:
	$(MAKE) $(SYSTEM)

fromsrc: arun

Darwin: PLATFORMFLAG =
Darwin:
	$(MAKE) unit
	$(MAKE) arun
	$(MAKE) dumpacd


CYGWIN: PLATFORMFLAG = -mno-cygwin
CYGWIN:
	$(MAKE) unit
	$(MAKE) arun
	$(MAKE) winarun
	$(MAKE) glkarun
	$(MAKE) gararun
	$(MAKE) dumpacd


clean:
	-rm *.o



#######################################################################
# Settings for Release
release: SMARTALLOC =
release: OPTIMIZE=-O4
release:
	$(MAKE) $(SYSTEM)



#######################################################################
# Settings for Standard Arun
arun: checkTarget $(ARUNOBJECTS) Makefile dependencies.mk
	gcc -o $@ $(OPTIMIZE) $(ARUNOBJECTS) $(LDFLAGS) $(LIBS)
	cp $@ ../bin


#######################################################################
# Settings for Windows GLK variant
WINGLKROOT = ../../WindowsGLK
WINGLKDEFS = -DHAVE_WINGLK
WINGLKINCLUDE = -I$(WINGLKROOT)/Include
WINGLKLIB = $(WINGLKROOT)/Glk.lib

winarun: COMPILEFLAGS = $(STDCOMPILEFLAGS) $(WINGLKINCLUDE) $(WINGLKDEFS)
winarun: PLATFORMFLAG = -mno-cygwin 
winarun: LIBS = $(WINGLKLIB) -mwindows

winarun: checkTarget $(WINARUNOBJECTS)
	gcc -o $@ $(OPTIMIZE) $(WINARUNOBJECTS) $(LDFLAGS) $(LIBS)
	cp $@ ../bin

arun.res: arun.rc arun.ico resources.h alan.version.c
	echo $(VERSION)
	sed "s/WINARUNVERSION/$(VERSION)/" arun.rc > arun.rc2
	windres arun.rc2 -O coff -o $@


#######################################################################
# Settings for TermGLK => glkarun
TERMGLKROOT = ../../glkterm
TERMGLKDEFS = -DHAVE_GLK
TERMGLKINCLUDE = -I$(TERMGLKROOT)
TERMGLKLIB = -L$(TERMGLKROOT) -lglkterm -lncurses

glkarun: PLATFORMFLAG = # Cannot be build with -mno-cygwin since glkterm cannot be build with out cygwin since I have no curses library for windows
glkarun: COMPILEFLAGS = $(STDCOMPILEFLAGS) $(TERMGLKINCLUDE) $(TERMGLKDEFS)
glkarun: LIBS = $(TERMGLKLIB)

glkarun: checkTarget $(TERMARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(TERMARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	cp $@ ../bin


#######################################################################
# Settings for Gargoyle GLK variant (experimental!)
GARGLKROOT = ../../Gargoyle-2006-09-17/garglk
GARGLKDEFS = -DHAVE_GARGLK
GARGLKINCLUDE = -I$(GARGLKROOT)
GARGLKLIB = -L$(GARGLKROOT)/../build/cygwin.release/garglk -lgarglkmain -lpng12 -lgtk -lgdk `pkg-config --libs freetype2` -ljpeg /home/thoni/fmodapi374win/api/lib/libfmod.a -lgarglk

gararun: COMPILEFLAGS = $(STDCOMPILEFLAGS) $(GARGLKINCLUDE) $(GARGLKDEFS)
gararun: LIBS = $(GARGLKLIB)

gararun: checkTarget $(GARARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(GARARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	cp $@ ../bin


#######################################################################
ARCH	= `echo $(COMPILEFLAGS)`
.PHONY: checkTarget
checkTarget:
	@echo "ARCH  = $(ARCH)"
	@echo ".arch = `cat .arch`"
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
	-@if test "`which venum`" != "" ; then \
		cp -p ../alan.version.c . ; \
	fi


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# CGreen unit tests
#
CGREENROOT = ../Cgreen
CGREENINCLUDE = -I$(CGREENROOT)/include
unittests: PLATFORMFLAG = # Cannot run without -mno-cygwin since cgreen cannot be compiled with it yet
unittests: CFLAGS = $(COMPILEFLAGS) $(CGREENINCLUDE)
unittests: LIBS = $(CGREENROOT)/libcgreen.a

unittests: checkTarget $(UNITTESTSOBJECTS) Makefile $(LIBS)
	gcc -o unittests $(UNITTESTSOBJECTS) $(LDFLAGS) $(LIBS)

.PHONY: unit
unit:
	-@if test -d $(CGREENROOT) ; then \
		$(MAKE) unittests ; \
		./unittests ; \
	fi

#+++++++++++++++++++++++++++++++++
#
# Run all tests!
#
test: unit


#######################################################################
# Misc programs
#
# Dump Acode file into text format
dumpacd: checkTarget dumpacd.o reverse.o lists.o sysdep.o  ../compiler/spa.c smartall.o
	gcc -Wall -o dumpacd -ggdb $(PLATFORMFLAG) dumpacd.o reverse.o lists.o sysdep.o smartall.o -DSTDIONONCONST ../compiler/spa.c
	cp $@ ../bin

# Test of about dialog
about: about.c arun.res
	gcc -o $@ -ggdb -mwindows about.c arun.res

# Generate a header of a particular version
headerGenerator: headerGenerator.c



###################################################################
# Dependencies are generated only for command line case
# Dependencies on libraries are only in the way for other cases,
# so special dependencies that we need are set explicitly below
#
CPP	= gcc -MM -ansi
DEPENDINCLUDES = $(WINGLKINCLUDE) $(CGREENINCLUDE)
depend:
	@-for f in $(UNITTESTSSRC) $(MAINSRCS) $(GLKSRCS) dumpacd.c reverse.c ; \
	  do \
	  $(CPP) $(DEPENDFLAGS) $(DEPENDINCLUDES) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

# Extra dependencies for WinGLK case
readline.o : resources.h

include dependencies.mk

# Makefile for Alan interpreters, arun, winarun et al.
# Windows: CygWin32/Mingw32
# Darwin: GCC
# Note: uses a lot of special stuff, if you are porting, start from
# one of the other makefiles, such as Makefile.unix (which are not up-to-date)

VERSION = `cd ..; venum alan -print "\\$$v.\\$$r\\$$s\\$$c"`
SYSTEM = `uname -s | sed 's/_.*$$//'`

CYGWIN	= -mno-cygwin
OPTIMIZE = -ggdb
COMPILEFLAGS = $(OPTIMIZE) $(CYGWIN) -Wall $(CRCLOG) $(SMARTALLOC) -MMD
CFLAGS	= $(COMPILEFLAGS) 
CC = gcc
LDFLAGS = $(CYGWIN)


#Settings for memory debugging
SMARTALLOC = -DSMARTALLOC


# Define this to make interpreter log each crc calculation step
#CRCLOG = -DCRCLOG


# Include common source lists 
include sources.mk


###############################################################
all:
	$(MAKE) $(SYSTEM)

Darwin: CYGWIN =
Darwin: unitTest arun #dumpacd


CYGWIN: unitTest arun dumpacd winarun glkarun gararun


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
	-@if ! test -d ../bin; then mkdir ../bin ; fi
	cp $@ ../bin


GLKSRCS = glkstart.c glkio.c
#######################################################################
# Settings for Windows GLK variant
WINGLKROOT = ../../WindowsGLK
WINGLKDEFS = -DHAVE_WINGLK
WINGLKINCLUDE = -I$(WINGLKROOT)/Include
WINGLKLIB = $(WINGLKROOT)/Glk.lib

winarun: GLKDEFS = $(WINGLKDEFS)
winarun: CFLAGS = $(COMPILEFLAGS) $(WINGLKINCLUDE) $(GLKDEFS)
winarun: CYGWIN = -mwindows -mno-cygwin 
winarun: LIBS = $(WINGLKLIB)

winarun: checkTarget $(WINARUNOBJECTS) winglk.o arun.res
	gcc -o $@ $(OPTIMIZE) $(WINARUNOBJECTS) winglk.o arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
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

glkarun: GLKDEFS = $(TERMGLKDEFS)
glkarun: CFLAGS = $(COMPILEFLAGS) $(TERMGLKINCLUDE) $(GLKDEFS)
glkarun: CYGWIN = 
glkarun: LIBS = $(TERMGLKLIB)

glkarun: checkTarget $(TERMARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(TERMARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $@ ../bin


#######################################################################
#Settings for Gargoyle GLK variant (experimental!)
GARGLKROOT = ../../Gargoyle-2006-09-17/garglk
GARGLKDEFS = -DHAVE_GARGLK -mno-cygwin -mwindows
GARGLKINCLUDE = -I$(GARGLKROOT)
GARGLKLIB = -L$(GARGLKROOT)/../build/cygwin.release/garglk -lgarglkmain -lpng12 -lgtk -lgdk `pkg-config --libs freetype2` -ljpeg /home/thoni/fmodapi374win/api/lib/libfmod.a -lgarglk -mno-cygwin

gararun: GLKDEFS = $(GARGLKDEFS)
gararun: CFLAGS = $(COMPILEFLAGS) $(GARGLKINCLUDE) $(GLKDEFS)
gararun: CYGWIN = 
gararun: LIBS = $(GARGLKLIB)

gararun: checkTarget $(GARARUNOBJECTS) arun.res
	gcc -o $@ $(OPTIMIZE) $(GARARUNOBJECTS) arun.res $(LDFLAGS) $(LIBS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp $@ ../bin


#######################################################################
ARCH	= $(CYGWIN)$(OPTIMIZE)$(GLKDEFS)$(DMALLOC)$(SMARTALLOC)
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
	-@if test "`which venum`" != "" ; then \
		cp -p ../alan.version.c . ; \
	fi


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

unitTest:
	-@if test -d $(CGREENROOT) ; then \
		$(MAKE) unit2 ; \
		./unit2 ; \
	fi

#+++++++++++++++++++++++++++++++++
#
# Run all tests!
#
test: unitTest


#######################################################################
# Misc programs
dumpacd: dumpacd.o reverse.o lists.o sysdep.o  ../compiler/spa.c
	gcc -Wall -o dumpacd -ggdb $(CYGWIN) dumpacd.o reverse.o lists.o sysdep.o -DSTDIONONCONST ../compiler/spa.c
	cp $@ ../bin

about: about.c arun.res
	gcc -o $@ -ggdb -mno-cygwin -mwindows about.c arun.res

headerGenerator: headerGenerator.c



#
# Dependencies are generated only for command line case
# Dependencies on libraries are only in the way for other cases,
# so special dependencies that we need are set explicitly below
#
CPP	= gcc -MM -ansi
DEPENDINCLUDES = $(WINGLKINCLUDE) $(CGREENINCLUDE)
depend: CYGWIN =
depend:
	@-for f in $(UNIT2SRC) $(MAINSRCS) $(GLKSRCS) dumpacd.c reverse.c ; \
	  do \
	  $(CPP) $(CFLAGS) $(DEPENDINCLUDES) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

# Extra dependencies for WinGLK case
readline.o : resources.h

include dependencies.mk

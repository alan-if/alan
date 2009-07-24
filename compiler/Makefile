# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

# Cygwin or MingW?
PLATFORM = -mno-cygwin
#PLATFORM = 
OPTIMIZE = -ggdb

winalan:	PLATFORM = -DWINGUI -mno-cygwin -mwindows
release:	OPTIMIZE = -O4
gcov:	GCOV = -fprofile-arcs -ftest-coverage

dmalloc: DMALLOC = -DDMALLOC
dmalloc: CFLAGS = $(COMPILEFLAGS) $(DMALLOC)
dmalloc: LIBS = /usr/local/lib/libdmalloc.a
dmalloc: PLATFORM =

CFLAGS 	= $(OPTIMIZE) -Wall $(GCOV) -funsigned-char $(INCLUDE) $(PLATFORM)
LDFLAGS	= $(PLATFORM)

CC	= gcc
CPP	= gcc -MM -ansi
LD	= gcc


include sources.mk

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main targets
#

all : alan ../bin/rndalan

alan: checkTarget tm $(MAINOBJECTS) unit
	$(LD) -o alan $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

alanmain: $(MAINOBJECTS)
	$(LD) -o alan $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

rndalan.exe: rndalan.c

../bin/rndalan.exe : rndalan.exe
	cp rndalan.exe ../bin

release:
	$(MAKE) "OPTIMIZE=-O4" alan
	$(MAKE) "OPTIMIZE=-O4" winalan

winalan: checkTarget $(MAINOBJECTS) alan.res unit
	$(LD) -o winalan $(MAINOBJECTS) alan.res $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp winalan.exe ../bin


alan.res: alan.rc alan.ico
	windres $< -O coff -o $@

gcov :
	$(MAKE) "GCOV=-fprofile-arcs -ftest-coverage"

clean:
	-rm *.o

.tmstamp: $(TMSRCS)
	$(MAKE) -f Makefile.tm

src:
	$(MAKE) -f Makefile.src

fromsrc: unit $(MAINOBJECTS)
	$(LD) -o alan $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

tm:
	-@if test "`uname -n`" = "ThoNi"; then \
		make -f Makefile.tm ; \
	fi


ARCH	= `echo $(PLATFORM) $(OPTIMIZE)`
.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for `cat .arch`, building for $(ARCH)... ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#

unit: unittests
	@./unittests

.PHONY: unit
unittests : $(TESTOBJS)
	$(LD) -o unittests $(CFLAGS) $(TESTOBJS) $(LDFLAGS)

.PHONY:test
# Run all tests!
test : unittests
	-cd testing; regr

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

spa.o: spa.c spa.h
	$(CC) $(CFLAGS) -c spa.c -o spa.o

smScanx.o: smScanx.c

#
# Common sysdep
#
#sysdep.c : ../interpreter/sysdep.c
#	cp -f ../interpreter/sysdep.c .
#	chmod a-w sysdep.c
#
#sysdep.h : ../interpreter/sysdep.h
#	cp -f ../interpreter/sysdep.h .
#	chmod a-w sysdep.h

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
depend:
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Version number generation
#
alan.version.h : ../alan.version.h
	cp ../alan.version.h .

alan.version.c:  ../alan.version
	cp ../alan.version.c .

version.h : ../version.h
	cp ../version.h .

../alan.version.c: ../alan.version
../alan.version.h: ../alan.version
../version.h: ../alan.version

../alan.version: $(VERSIONSRCS) 
	cd ..; venum alan time

#
include dependencies.mk

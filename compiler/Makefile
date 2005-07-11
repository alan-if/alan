# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

# Cygwin or MingW?
PLATFORM = -mno-cygwin
#PLATFORM = 
OPTIMIZE = -ggdb

winalan:	PLATFORM = -DWINGUI -mno-cygwin -mwindows
release:	OPTIMIZE = -O4
gcov:	GCOV = -fprofile-arcs -ftest-coverage

CFLAGS 	= $(OPTIMIZE) -Wall $(GCOV) -funsigned-char $(INCLUDE) $(PLATFORM)
LDFLAGS	= $(OPTIMIZE) $(PLATFORM) 

ARCH	= $(PLATFORM) $(OPTIMIZE)

CPP	= gcc -MM -ansi
LD	= gcc


include sources.mk

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main targets
#

alan: checkTarget tm $(MAINOBJECTS) unittests
	$(LD) -o alan $(CFLAGS) $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

release:
	$(MAKE) "OPTIMIZE=-O4" alan
	$(MAKE) "OPTIMIZE=-O4" winalan

winalan: checkTarget $(MAINOBJECTS) alan.res unittests
	$(LD) -o winalan $(MAINOBJECTS) alan.res $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp winalan.exe ../bin

alan.res: alan.rc alan.ico
	windres $< -O coff -o $@

all : .tmstamp test alan Makefile

gcov :
	$(MAKE) "GCOV=-fprofile-arcs -ftest-coverage"

clean:
	-rm *.o

.tmstamp: $(TMSRCS)
	$(MAKE) -f Makefile.tm

src:
	$(MAKE) -f Makefile.src

fromsrc: unittests $(MAINOBJECTS)
	$(LD) -o alan $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

tm:
	-@if test "`uname -n`" = "ThoNi"; then \
		make -f Makefile.tm ; \
	fi

.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for `cat .arch`, building for $(ARCH)... ; \
		rm *.o; \
		echo $(ARCH) > .arch; \
	fi

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Unit testing
#

unittests: unit
	@./unit

.PHONY: unit
unit : $(TESTOBJS)
	$(LD) -o unit $(CFLAGS) $(TESTOBJS) $(LDFLAGS)

.PHONY:test
# Run all tests!
test : unittests
	-cd testing; regr

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o

smScanx.o: smScanx.c

#
# Common sysdep
#
sysdep.c : ../interpreter/sysdep.c
	cp -f ../interpreter/sysdep.c .
	chmod a-w sysdep.c

sysdep.h : ../interpreter/sysdep.h
	cp -f ../interpreter/sysdep.h .
	chmod a-w sysdep.h

#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
depend:
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

#
# Version number generation
#
alan.version.h :
	-@if test "`uname -n`" = "ThoNi"; then \
		cd ..; venum -write always -all alan ; \
		cp -p ../alan.version.h . ; \
	fi

version.h :
	-@if test "`uname -n`" = "ThoNi"; then \
		cd ..; venum -write always -all alan ; \
		cp -p ../version.h . ; \
	fi

../alan.version: $(VERSIONSRCS) 
	-@if test "`uname -n`" = "ThoNi"; then \
		cd ..; venum alan time ; \
	fi

alan.version.c:  ../alan.version
	-@if test "`uname -n`" = "ThoNi"; then \
		cp -p ../alan.version.c . ; \
	fi

#
include dependencies.mk

# Makefile for alan compiler on Windows using Cygwin32/Mingw32
#

# Cygwin or MingW?
PLATFORM = -mno-cygwin
#PLATFORM = 
OPTIMIZE = -g

winalan:	PLATFORM = -DWINGUI -mno-cygwin -mwindows
release:	OPTIMIZE = -O4
gcov:	GCOV = -fprofile-arcs -ftest-coverage

CFLAGS 	= $(OPTIMIZE) -Wall $(GCOV) -funsigned-char $(INCLUDE) $(PLATFORM)
LDFLAGS	= $(OPTIMIZE) $(PLATFORM) 

ARCH	= $(PLATFORM) $(OPTIMIZE)

CPP	= gcc -MM -ansi
LD	= gcc

TMSRCS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

# Sources which are in the unit tests
TESTSRCS = \
	ins.c \
	cla.c \
	adv.c \
	sym.c \
	whr.c \


# Sources required for both builds
BUILDSRCS = \
	alan.c \
	alt.c \
	chk.c \
	cnt.c \
	context.c \
	dump.c \
	elm.c \
	encode.c \
	evt.c \
	form.c \
	id.c \
	lim.c \
	msg.c \
	nam.c \
	opt.c \
	rul.c \
	set.c \
	sco.c \
	scr.c \
	srcp.c \
	stm.c \
	stp.c \
	str.c \
	syn.c \
	type.c \
	wht.c \
	timing.c \
	util.c \
	spa.c \
	sysdep.c \
	options.c


# More Sources required for Alan program build
# Excluded from BUILDSRCS because there are unit tests for these
ALANSRCS = \
	main.c \
	adv.c \
	add.c \
	atr.c \
	cla.c \
	res.c \
	emit.c \
	exp.c \
	ext.c \
	ins.c \
	lst.c \
	images.c \
	sym.c \
	stx.c \
	prop.c \
	vrb.c \
	whr.c \
	wrd.c \

MAINSRCS = $(TMCSRCS) $(ALANSRCS) $(BUILDSRCS)
MAINOBJECTS = ${MAINSRCS:.c=.o} alan.version.o

VERSIONSRCS = $(ALANSRCS) $(BUILDSRCS) $(TMSRCS)


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

fromsrc: $(MAINOBJECTS)
	$(LD) -o alan $(MAINOBJECTS) $(LDFLAGS)
	-@if test -f ../bin; then :; else mkdir ../bin 2> /dev/null ; fi
	cp alan.exe ../bin

tm:
	make -f Makefile.tm

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

# Sources for the test framework
UNITSRCS = unit.c \
	unitList.c \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c

TESTSRCS = $(UNITSRCS) $(BUILDSRCS)
TESTOBJS = ${TESTSRCS:.c=.o} alan.version.o

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
	cd ..; venum -write always -all alan
	cp -p ../alan.version.h .

version.h :
	cd ..; venum -write always -all alan
	cp -p ../version.h .

../alan.version: $(VERSIONSRCS) 
	cd ..; venum alan time

alan.version.c:  ../alan.version
	cp -p ../alan.version.c .

#
include dependencies.mk

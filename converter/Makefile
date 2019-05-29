# Makefile for alan converter
#
# a2a3 is an Alan v2 to v3 converter that works on source code.
# It is built using the same parser generator tools that the
# Alan compiler is using, which are only available on ThoNi's
# machines. To build using those use the Makefile.thoni.
# This makefile instead assumes all sources are generated and
# only builds from the C source.

include ../mk/common.mk

CFLAGS += -g -Wall -MMD -DBUILD=$(BUILDNUMBER)

CPP	= gcc -MM -ansi
LD	= gcc

TMCSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c\
	lmList.c


# Sources required for both builds
BUILDSRCS = \
	a2a3.c \
	str.c \
	lst.c \
	util.c

MAINSRCS = $(TMCSRCS) $(BUILDSRCS)
MAINOBJECTS = ${MAINSRCS:.c=.o} a2a3.version.o

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main targets
#

all : unit build Makefile

build : a2a3

a2a3: checkTarget $(MAINOBJECTS)
	$(LINK) -o a2a3$(EXEEXT) $(MAINOBJECTS)
	cp a2a3$(EXEEXT) ../bin

release: a2a3

clean:
	@-rm -f *.o ../bin/a2a3

################
# If Cgreen is installed correctly and completely theses things should just work
# Otherwise you can try to set them to something that does work
CGREENROOT = /usr/local
#CGREENINCLUDE = -I$(CGREENROOT)/include
CGREENLIBDIR=$(CGREENROOT)/lib
#CGREENBINDIR=$(CGREENROOT)/bin/
#CGREENLIB = $(CGREENLIBDIR)/libcgreen.dll.a
#CGREENLOADLIB = LD_LIBRARY_PATH=$(CGREENLIBDIR)
CGREENLINKDIR = -L $(CGREENLIBDIR)
CGREENLINKLIB = $(CGREENLINKDIR) -lcgreen -lm

ifneq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
unit: CFLAGS += -fPIC
endif

.PHONY: unit
ifneq ($(CGREEN),yes)
unit:
	echo "No unit tests run, cgreen not available"
else
unit: unittests
endif

# Here we try to build a runnable DLL for each module where it can be
# tested in total isolation (with everything else mocked away,
# except lists.c and memory.c)

MODULES_WITH_UNITTESTS = util

# A test .dll for a module is built from its .o and the _test.o (and some extras)
%_tests.dll: %.o %_tests.o
	$(LINK) -shared -o $@ $^ $(LIBS)

UNITTESTS_DLLS = $(patsubst %,%_tests.dll,$(MODULES_WITH_UNITTESTS))

# Then run all _tests.dll's with the cgreen-runner
unittests: CFLAGS += $(CGREENINCLUDE)
unittests: LIBS = $(CGREENLINKLIB)
unittests: $(UNITTESTS_DLLS)
	@for f in $(UNITTESTS_DLLS) ; do \
		$(CGREENLOADLIB) $(CGREENBINDIR)cgreen-runner ./$$f --suite Converter $(UNITOUTPUT) ; \
	done


test :
	@cd ..; bin/jregr -bin bin -dir converter/testing $(JREGROUTPUT)


.PHONY: checkTarget
ARCH	= `echo $(PLATFORM) $(OPTIMIZE)`
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for \"`cat .arch`\", building for \"$(ARCH)\"... ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi


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

spa.o: spa.c spa.h
	$(COMPILE) -DSTDIONONCONST -c spa.c -o spa.o

#
# Version number generation
#
a2a3.version.h a2a3.version.c:
	venum -write always -all a2a3

version.h :
	venum -write always -all a2a3

a2a3.version: $(MAINSRCS)
	venum alan time

cross: CC=i686-w64-mingw32-gcc
cross: EXEEXT=.exe
cross: clean build

#
include dependencies.mk

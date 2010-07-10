#######################################################################
# This is a set of common rules for building Alan Interpreters
# It is included from Makefile to complement rules and settings
# specific to the host it is run on, set in such files as
# Makefile.thoni, Makefile.cygwin, Makefile.Darwin, also included from
# Makefile

#######################################################################
# Standard console Arun

arun: checkTarget $(ARUNOBJECTS) dependencies.mk
	$(LINK) -o $@ $(OPTIMIZE) $(ARUNOBJECTS) $(LINKFLAGS) $(LIBS)
	cp $@ ../bin/

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# CGreen unit tests
#
# Define
#	CGREEN to something to build with cgreen unit tests
#	CGREENINCLUDE so that cgreen/cgreen.h works if needed
#	CGREENLIB to something to link with (e.g. -lcgreen)

unittests: CFLAGS := $(COMPILEFLAGS) $(CGREENINCLUDE)
unittests: checkTarget $(UNITTESTSOBJECTS)
	$(LINK) -o unittests $(UNITTESTSOBJECTS) $(LINKFLAGS) $(CGREENLIB)

.PHONY: unit
unit:
	echo OBJDIR='$(OBJDIR)'
	-@if test -d $(CGREENDIR) ; then \
		$(MAKE) unittests ; \
		echo ; \
		echo "        Unit Tests" ; \
		echo ; \
		./unittests ; \
		echo ; \
		echo ; \
	else \
		echo "No unit tests run, cgreen not available" ; \
	fi

#######################################################################
.PHONY: clean
clean:
	-rm *.o


#######################################################################
ARCH	= `echo $(COMPILEFLAGS) $(PLATFORM)`
.PHONY: checkTarget
checkTarget:
	-@if test -f .arch; then :; else echo "none" > .arch; fi
	-@if test "`cat .arch`" != "$(ARCH)"; then \
		echo Removing objects for \'`cat .arch`\' ; \
		echo Re-building      for \'$(ARCH)\'... ; \
		rm *.o; \
		/bin/echo -n $(ARCH) > .arch; \
	fi

###################################################################
#
# Version number file generation
#
alan.version.h : ../alan.version.h
	cp ../alan.version.h .

alan.version.c:  ../alan.version.c
	cp ../alan.version.c .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h: ../alan.version

../alan.version:
	cd ..; venum alan time


###################################################################
#
# Run all tests!
# No tests except unit tests are available
#
.PHONY: test
test: unit


###################################################################
# Dependencies are generated only for command line case
# Dependencies on libraries are only in the way for other cases,
# so special dependencies that we need are set explicitly below
#
CPP	= gcc
DEPENDFLAGS = -MM
DEPENDINCLUDES = $(WINGLKINCLUDE) $(CGREENINCLUDE)
depend:
	@-for f in $(UNITTESTSSRCS) $(MAINSRCS) dumpacd.c reverse.c ; \
	  do \
	  $(CPP) $(DEPENDFLAGS) $(DEPENDINCLUDES) $$f >> dependencies.new; \
	done
	mv dependencies.new dependencies.mk

# Extra dependencies for WinGLK case
readline.o : resources.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h main.h \
  glkstart.h glkio.h resources.h utils.h
glkio.o: glkio.c glkio.h

# Makefile for Alan v2 to v3 converter
#
# a2a3 is an Alan v2 to v3 converter that works on source code.  It is
# built using the same parser generator tools that the Alan compiler
# is using. To build using those use the Makefile.thoni.  This
# makefile instead assumes all those sources are generated and only
# builds from the C source.
#
# Note that there currently is no Windows GUI for this...
INCLUDED = true

include ../mk/common.mk

SOEXTENSION = dll

# Jenkins sets BUILD_NUMBER so use that if available
ifneq ($(BUILD_NUMBER),)
  BUILDNUMBER := $(BUILD_NUMBER)
endif
CFLAGS += -g -Wall -MMD -DBUILD=$(BUILDNUMBER)

######################################################################
#
# Sources
#
TMSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScan.c smScSema.c\
	lmList.c

# Sources required for both main and unit builds
BUILDSRCS = \
	a2a3.c \
	str.c \
	lst.c \
	util.c

######################################################################
#
# Main targets
#

all: unit build Makefile

build: a2a3$(EXEEXT)

release: a2a3

clean:
	-rm -rf $(OBJDIRS) a2a3$(EXEEXT) ../bin/a2a3$(EXEEXT)

######################################################################
#
# a2a3
#
A2A3_SRCS = $(TMSRCS) $(BUILDSRCS)

A2A3_OBJDIR = .a2a3
OBJDIRS += $(A2A3_OBJDIR)

A2A3_OBJECTS = $(addprefix $(A2A3_OBJDIR)/,${A2A3_SRCS:.c=.o}) $(A2A3_OBJDIR)/a2a3.version.o

-include $(A2A3_OBJECTS:.o=.d)

$(A2A3_OBJECTS): $(A2A3_OBJDIR)/%.o: %.c
	$(COMPILE) -o $@ -c $<

a2a3: ../bin $(A2A3_OBJECTS)
	$(LINK) -o $@$(EXEEXT) $(A2A3_OBJECTS)
	cp $@$(EXEEXT) ../bin

../bin:
	-mkdir ../bin

######################################################################
#
# Cgreen unit tests
#
# Here we try to build a runnable DLL for each module where it can be
# tested in total isolation (with everything else mocked away if required)

UNITTESTS_OBJDIR = .unittests
OBJDIRS += $(UNITTESTS_OBJDIR)

MODULES_WITH_ISOLATED_UNITTESTS = util
SUITE = Converter

include ../mk/isolated_unittests.mk

# We might need to locate -lcgreen
isolated_unittests: LDLIBS = -L/usr/local/lib
unittests: isolated_unittests

.PHONY: unit
ifeq ($(CGREEN),)
unit:
	echo "No unit tests run, cgreen not available"
else
unit: unittests
endif

# We also need to add -fPIC if not compiling for DLL-platforms
# More to be added here...
ifneq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
unit: CFLAGS += -fPIC
endif

######################################################################
test :
	@cd ..; bin/jregr -bin bin -dir converter/testing $(JREGROUTPUT)

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

version.h:
	venum -write always -all a2a3

a2a3.version: $(A2A3_SRCS)
	venum alan time

cross: cross_clean
	$(MAKE) cross-build

cross-build: CC=x86_64-w64-mingw32-gcc
cross-build: CFLAGS += -I$(WINICONV_ROOT)
cross-build: LDLIBS= -L$(WINICONV_ROOT) -l:libwiniconv.a
cross-build: EXEEXT=.exe
cross-build: build

# We need to remove all *.o for the targets that may be cross built
# but are not by default
.PHONY: cross_clean
cross_clean:
	-rm -f $(A2A3_OBJDIR)/*.o a2a3$(EXEEXT)

# Create all objdirs if necessary (at parse time)
$(info $(shell mkdir -p $(OBJDIRS)))

######################################################################
#
# Some common definitions for the Alan Makefiles
#

# Where the top of the tree is, relative to whoever included us. This
# file is included from the root and from each component directory, so
# work it out from our own path rather than assuming either.
ALAN_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))..

# The version components, and the release string composed from them.
# The file holds the components; the VERSION variable is the string,
# e.g. "3.0beta8". See VERSION for the composition rule.
include $(ALAN_ROOT)/VERSION
VERSION := $(strip $(ALAN_VERSION)).$(strip $(ALAN_REVISION))$(or $(strip $(ALAN_STATE)),.)$(strip $(ALAN_CORRECTION))

# TODO: Msys have three variants - MSYS, MINGW64, MINGW64
# And we need both -o and -s to figure out which...

# Discover OS and ARCH
UNAME_O_WORKS=$(shell uname -o &>/dev/null; echo $$?)
ifeq ($(UNAME_O_WORKS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif
ifeq ($(OS),GNU/Linux)
  OS=Linux
endif
ARCH = $(shell uname -m)
ifdef TRACE
  $(info OS=$(OS), ARCH=$(ARCH))
endif

# Extension for shared libraries on this platform. Mainly used for the
# Cgreen unit test libraries, which are loaded by the cgreen-runner.
ifeq ($(OS),Darwin)
  SOEXTENSION ?= dylib
else ifneq ($(filter Cygwin Msys,$(OS)),)
  SOEXTENSION ?= dll
else
  SOEXTENSION ?= so
endif

# Unit tests are built as shared libraries, which need position
# independent code everywhere except on the DLL platforms, where it is
# the default and the flag is ignored anyway.
ifeq ($(filter Cygwin Msys,$(OS)),)
unit: CFLAGS += -fPIC
unit: LDFLAGS += -fPIC
endif

######################################################################
#
# Cgreen
#
# Do we even have Cgreen available?
CGREEN := $(shell which cgreen-runner)

# How to link a unit test library against Cgreen. A hand built Cgreen
# ends up under /usr/local by default; point CGREENROOT at yours if it
# lives somewhere else, or override CGREENLINKLIB outright (from a
# platform Makefile, Makefile.local or the command line).
#
# Note that this has to name the library, not just add its directory:
# on Cygwin and MinGW every symbol of a DLL must resolve at link time,
# so a missing -lcgreen breaks the build there even though ELF
# platforms happily leave those symbols for the runner to supply.
CGREENROOT ?= /usr/local
CGREENLIBDIR ?= $(CGREENROOT)/lib
CGREENLINKLIB ?= -L$(CGREENLIBDIR) -lcgreen -lm

# Shorthands
COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

# Running inside Emacs? Ensure pretty output
ifneq ($(INSIDE_EMACS),)
  JREGROUTPUT = -noansi
else
  UNITOUTPUT ?= -c
endif

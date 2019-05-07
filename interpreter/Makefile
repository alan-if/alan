#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
#
# By default this will build the command line interpreter arun
#
# To handle local dependencies and enable some extra targets, copy
# Makefile.local.template to Makefile.local and modify settings there

# Defer resolution of EXTRA_TARGETS to end of this file
all: everything

# Discover OS and ARCH
UNAME_O_WORKS=$(shell uname -o &>/dev/null; echo $$?)
ifeq ($(UNAME_O_WORKS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif
ARCH = $(shell uname -m)
$(info OS=$(OS), ARCH=$(ARCH))

# Include list of all sources and symbols for various groups of them
include sources.mk

# Allow subordinate Makefiles to exectute
INCLUDED = true

# Read possible local dependencies and EXTRA_TARGETS to enable
-include Makefile.local

# Include correct subordinate Makefile depending on OS and possibly system name
ifeq ($(OS),cygwin)
	ifeq ($(shell uname -n), thoni64)
		include Makefile.thoni
	else
		include Makefile.cygwin
	endif
else
	include Makefile.$(OS)
endif

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Include common build rules
include Makefile.common

everything: unit arun $(EXTRA_TARGETS)
build: arun $(EXTRA_TARGETS)

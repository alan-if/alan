#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
#
# By default this will build the command line interpreter arun
#
# To handle local dependencies and enable some extra targets, copy
# Makefile.local.template to Makefile.local and modify settings there

all: everything

-include Makefile.local

# Note that the following must not have spaces in it lest the variable will include it
OS := ${if ${findstring CYGWIN,${shell uname}},CYGWIN,${shell uname}}
ARCH := `uname -m`

# Include list of all sources
include sources.mk

# Include correct main Makefile depending on OS and possibly system name
#
INCLUDED = true
ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
  ifeq ($(shell uname -n), thoni64)
	include Makefile.thoni
  else
	include Makefile.cygwin
  endif
else
  ifeq ($(findstring Msys, $(shell uname -o)), Msys)
	include Makefile.Msys
  else
	include Makefile.$(shell uname)
  endif
endif

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Include common build rules
include Makefile.common

def:
	@echo OS=$(OS), ARCH=$(ARCH)
	@echo CGREEN=$(CGREEN)

everything: arun $(EXTRA_TARGETS)

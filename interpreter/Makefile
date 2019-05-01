#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
# In those files you should handle all target specific
# compile and link commands
#

all: arun $(EXTRA_TARGETS)

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Note that this must not have spaces in it lest the variable will include it
OS := ${if ${findstring CYGWIN,${shell uname}},CYGWIN,${shell uname}}
ARCH := `uname -m`

# Include list of all sources
include sources.mk

# Include correct main Makefile depending on OS and possibly system name
#
$(info $(OS))
ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
  ifeq ($(shell uname -n), thoni64)
	include Makefile.thoni
  else
	include Makefile.cygwin
  endif
else
  ifeq ($(findstring MINGW, $(shell uname)), MINGW)
	include Makefile.Mingw
  else
	include Makefile.$(shell uname)
  endif
endif

# Include common build rules
include Makefile.common

def:
	@echo OS=$(OS), ARCH=$(ARCH)
	@echo CGREEN=$(CGREEN)

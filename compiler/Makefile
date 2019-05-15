#######################################################################
# This is the general Makefile for building the Alan Compiler
# It figures out on which host platform (and possibly on which computer)
# it is running an will include specific Makefiles to build on that
# particular host.
#
# The included platform specific Makefiles should handle all
# target specific compile and link settings and possible extra
# targets available on that platform.
#
# To handle local dependencies and enable some extra targets, copy
# Makefile.local.template to Makefile.local and modify settings there

# Defer to end of this Makefile
all: everything

# Running inside Emacs?
ifneq ($(EMACS),)
  JREGROUTPUT = -noansi
else
  UNITOUTPUT ?= -c
endif

# Discover OS and ARCH
UNAME_O_WORKS=$(shell uname -o &>/dev/null; echo $$?)
ifeq ($(UNAME_O_WORKS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif
ARCH = $(shell uname -m)
$(info OS=$(OS), ARCH=$(ARCH))

# Include the common source lists, need to be before the included rules
include sources.mk

# Allow subordinate Makefiles to exectute
INCLUDED = true

# Read possible local dependencies and EXTRA_TARGETS to enable
-include Makefile.local

# Include correct subordinate Makefile depending on OS and possibly system name
ifeq ($(OS),Cygwin)
	ifeq ($(shell uname -n), thoni64)
		-include Makefile.thoni
	else
		-include Makefile.cygwin
	endif
else
	-include Makefile.$(OS)
endif

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Include common build rules
include Makefile.common

everything: unit alan $(EXTRA_TARGETS)
build: alan $(EXTRA_TARGETS)

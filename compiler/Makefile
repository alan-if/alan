#######################################################################
# This is the general Makefile for building the Alan Compiler
# It figures out on which host platform (and possibly on which computer)
# it is running and will include specific Makefiles to build on that
# particular host.
#
# The included platform specific Makefiles should handle all
# target specific compile and link settings and possible extra
# targets available on that platform.
#
# To handle local dependencies and enable some extra targets, copy
# Makefile.local.template to Makefile.local and modify settings there

include ../mk/common.mk

# Defer resolution of EXTRA_TARGETS to end of this Makefile
all: everything

# Silent running if not VERBOSE set to something
#$(VERBOSE).SILENT:

# Allow subordinate Makefiles to execute
INCLUDED = true

# Include the common source lists and symbols for various groups of them
include sources.mk

# Read possible local dependencies and EXTRA_TARGETS to enable
ifneq ($(wildcard Makefile.local),)
	include Makefile.local
else
# Read the standard template for local settings
	include Makefile.local.template
endif

-include Makefile.$(OS)

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Include common build rules
include Makefile.common

everything: unit alan $(EXTRA_TARGETS)
build: alan $(EXTRA_TARGETS)

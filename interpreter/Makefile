#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
#
# By default this will build the command line interpreter arun
#
# To handle local dependencies and enable some extra targets, copy
# Makefile.local.template to Makefile.local and modify settings there
include ../mk/common.mk

# Defer resolution of EXTRA_TARGETS to end of this file
all: everything

# Silent running if not VERBOSE set to something
#$(VERBOSE).SILENT:

# Allow subordinate Makefiles to execute
INCLUDED = true

# Include list of all sources and symbols for various groups of them
include sources.mk

# Read possible local dependencies and EXTRA_TARGETS to enable
ifneq ($(wildcard Makefile.local),)
	include Makefile.local
else
# Read the standard template for local settings
	include Makefile.local.template
endif

-include Makefile.$(OS)

# Include common build rules
include Makefile.common

.PHONY: everything
everything: unit arun $(EXTRA_TARGETS)

.PHONY: build
build: arun $(EXTRA_TARGETS)

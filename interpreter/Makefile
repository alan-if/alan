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

# Allow subordinate Makefiles to execute
INCLUDED = true

# Include list of all sources and symbols for various groups of them
include sources.mk

# Read possible local dependencies and EXTRA_TARGETS to enable
-include Makefile.local

-include Makefile.$(OS)

# Do we have Cgreen available?
CGREEN:=$(if $(shell which cgreen-runner 2> /dev/null),yes,no)

# Include common build rules
include Makefile.common

everything: unit arun $(EXTRA_TARGETS)
build: arun $(EXTRA_TARGETS)

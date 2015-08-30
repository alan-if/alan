#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
# In those files you should handle all target specific
# compile and link commands
#

# Note that this must not have spaces in it lest the variable will include it
OS := ${if ${findstring CYGWIN,${shell uname}},CYGWIN,${shell uname}}

# Include list of all sources
include sources.mk

# Include correct main Makefile depending on OS and possibly system name
#
ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
ifeq ($(shell uname -n), thoni)
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

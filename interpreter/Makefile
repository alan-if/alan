#######################################################################
# This is the general Makefile for building the Alan Interpreter
# It will include specific Makefiles to build on the target
# In those files you should handle all target specific
# compile and link commands
#

# Include list of all sources
include sources.mk

# Include the correct main Makefile depending on the system name
#
ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
ifeq ($(shell uname -n), thoni)
include Makefile.thoni
else
include Makefile.cygwin
endif
else
include Makefile.$(shell uname)
endif

# Include common build rules
include rules.mk
# and all the dependencies
include dependencies.mk

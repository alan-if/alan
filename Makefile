#######################################################################
# This is the general Makefile for building the Alan System
# It figures out on which host platform (and possibly on which computer)
# it is running and will include specific Makefiles to build on that
# particular host
#
#
include mk/common.mk

all: unit build test

# Include the correct main Makefile depending on the OS/environment
INCLUDED = true
#
-include Makefile.$(OS)

include Makefile.common

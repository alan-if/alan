#######################################################################
# This is the general Makefile for building the Alan System
# It figures out on which host platform (and possibly on which computer)
# it is running and will include specific Makefiles to build on that
# particular host
#
# Development builds identify themselves as <release>-dev<count>, where
# the count comes from 'git describe' - see mk/common.mk. Nothing needs
# to be set for that, but the release tags do have to be fetched, so a
# CI checkout must not be shallow.
#
include mk/common.mk

all: everything

# Include the correct main Makefile depending on the OS/environment
INCLUDED = true
#
-include Makefile.$(OS)

include Makefile.common

everything: unit build test $(EXTRA_TARGETS)

test: build				# Ensure everything is built before testing

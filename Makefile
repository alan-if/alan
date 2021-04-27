#######################################################################
# This is the general Makefile for building the Alan System
# It figures out on which host platform (and possibly on which computer)
# it is running and will include specific Makefiles to build on that
# particular host
#
# If you are running this in a CI environment that should create
# distributions, set BUILDNUMBER to the build number as in
#
#   make BUILDNUMBER=$BUILD_NUMBER <target>
#
	include mk/common.mk

all: everything

# Include the correct main Makefile depending on the OS/environment
INCLUDED = true
#
-include Makefile.$(OS)

include Makefile.common

everything: unit build test $(EXTRA_TARGETS)

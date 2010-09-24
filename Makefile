#######################################################################
# This is the general Makefile for building the Alan System
# It figures out on which host platform (and possibly on which computer)
# it is running an will include specific Makefiles to build on that
# particular host
#
# In the included platform specific Makefiles you should handle all
# target specific compile and link settings
#

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
#######################################################################
# This is the general Makefile for building the Alan System
# It figures out on which host platform (and possibly on which computer)
# it is running and will include specific Makefiles to build on that
# particular host
#
# In the included platform specific Makefiles you should handle all
# target specific compile and link settings
#
include mk/common.mk

# Include the correct main Makefile depending on the OS/environment
INCLUDED = true
#
include Makefile.$(OS)

#ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
#	include Makefile.cygwin
#else
#	ifeq ($(findstring MINGW, $(shell uname)), MINGW)
#		include Makefile.Mingw
#	else
#		include Makefile.$(shell uname)
#	endif
#endif

######################################################################
#
# Some common definitions for the Alan Makefiles
#

# TODO: Msys have three variants - MSYS, MINGW64, MINGW64
# And we need both -o and -s to figure out which...

# Discover OS and ARCH
UNAME_O_WORKS=$(shell uname -o &>/dev/null; echo $$?)
ifeq ($(UNAME_O_WORKS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif
ifeq ($(OS),GNU/Linux)
  OS=Linux
endif
ARCH = $(shell uname -m)
ifdef TRACE
  $(info OS=$(OS), ARCH=$(ARCH))
endif

# Shorthands
COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

# Running inside Emacs? Ensure pretty output
ifneq ($(INSIDE_EMACS),)
  JREGROUTPUT = -noansi
else
  UNITOUTPUT ?= -c
endif

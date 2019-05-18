# Some common definitions for the Alan Makefiles

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
$(info OS=$(OS), ARCH=$(ARCH))

# Shorthands
COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

# Running inside Emacs? Ensure pretty output
ifneq ($(EMACS),)
  JREGROUTPUT = -noansi
else
  UNITOUTPUT ?= -c
endif

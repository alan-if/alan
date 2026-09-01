######################################################################
#
# Some common definitions for the Alan Makefiles
#

# Where the top of the tree is, relative to whoever included us. This
# file is included from the root and from each component directory, so
# work it out from our own path rather than assuming either.
ALAN_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))..

# The version components, and the release string composed from them.
# The file holds the components; the VERSION variable is the string,
# e.g. "3.0beta9". See VERSION for the composition rule.
include $(ALAN_ROOT)/VERSION
VERSION := $(strip $(ALAN_VERSION)).$(strip $(ALAN_REVISION))$(or $(strip $(ALAN_STATE)),.)$(strip $(ALAN_CORRECTION))

######################################################################
#
# Build designation - identifying a development build
#
# A development build names the release it is working *towards*, plus
# how far past the last release tag it has come:
#
#     3.0beta9-dev98
#      |        |
#      |        `- commits since the v3.0beta8 tag
#      `- VERSION, bumped to the next release right after tagging
#
# So between releases VERSION and the nearest tag deliberately disagree,
# and that disagreement is the whole mechanism. See RELEASE.md.
#
# The count is monotonic because master is linear: one commit, one
# increment. It restarts at each release, which is what makes it
# readable - it is a distance from a known point, not a global serial
# like the Jenkins BUILD_NUMBER it replaces. Nothing compares build
# numbers across releases, so the restart costs us nothing.
#
#   BUILDNUMBER   the count alone, empty on a release tag
#   BUILDVERSION  "-dev<count>", so it appends to $(VERSION)
#   BUILDNAME     "dev<count>", for naming snapshot directories
#
# On the release commit itself there is no count, all three are empty,
# and packages are named plainly: alan-3.0beta9-linux.
#
# This needs the release tags to be present. A shallow checkout has
# none, so CI must fetch them - the workflows pass fetch-depth: 0.
GIT_DESCRIBE := $(shell git -C $(ALAN_ROOT) describe --tags --match 'v[0-9]*.[0-9]*' 2>/dev/null)

ifeq ($(GIT_DESCRIBE),)
  # No git, no tags, or a shallow checkout - building from a source
  # package, most likely. We cannot tell a release from a development
  # build, so say so, rather than quietly naming this as if it were a
  # release. Matches the "unknown" that mk/git-revision.sh emits.
  BUILDVERSION := -devunknown
  BUILDNAME := devunknown
else
  # 'v3.0beta8-98-g7142db50' -> '98', and nothing at all when describe
  # returned the bare tag, which is precisely the release case. The
  # leading .* is greedy, so this still picks the right field if a tag
  # ever contains a dash itself.
  BUILDNUMBER := $(shell echo '$(GIT_DESCRIBE)' | sed -n 's/.*-\([0-9][0-9]*\)-g[0-9a-f]*$$/\1/p')
  ifneq ($(BUILDNUMBER),)
    BUILDVERSION := -dev$(BUILDNUMBER)
    BUILDNAME := dev$(BUILDNUMBER)
  endif
endif

######################################################################
#
# MinGW-w64 cross toolchains
#
# Two are needed, not one. The SDK (alan, arun, a2a3) and winalan are
# 64-bit. winarun is not, and cannot be: it links against David
# Kinder's WindowsGlk, which is built for Win32 only -- its
# GlkDll/Glk.vcxproj carries no x64 configuration, and the Glk.lib in
# every release including 1.55 is an i386 import library exporting
# _glk_* with the 32-bit cdecl underscore. Linking it with the 64-bit
# toolchain fails with an undefined reference for every glk_ symbol.
#
# Override either from the environment or the command line if your
# toolchain is named differently.
MINGW64_PREFIX ?= x86_64-w64-mingw32
MINGW32_PREFIX ?= i686-w64-mingw32

# win-iconv, one build per architecture, since both would otherwise be
# called libwiniconv.a. WINICONV_ROOT is the 64-bit one and is what the
# SDK cross build uses; WINICONV32_ROOT is winarun's.
WINICONV32_ROOT ?= $(WINICONV_ROOT)-i686

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

# Extension for shared libraries on this platform. Mainly used for the
# Cgreen unit test libraries, which are loaded by the cgreen-runner.
ifeq ($(OS),Darwin)
  SOEXTENSION ?= dylib
else ifneq ($(filter Cygwin Msys,$(OS)),)
  SOEXTENSION ?= dll
else
  SOEXTENSION ?= so
endif

# Unit tests are built as shared libraries, which need position
# independent code everywhere except on the DLL platforms, where it is
# the default and the flag is ignored anyway.
ifeq ($(filter Cygwin Msys,$(OS)),)
unit: CFLAGS += -fPIC
unit: LDFLAGS += -fPIC
endif

######################################################################
#
# Cgreen
#
# Do we even have Cgreen available?
CGREEN := $(shell which cgreen-runner)

# How to link a unit test library against Cgreen. A hand built Cgreen
# ends up under /usr/local by default; point CGREENROOT at yours if it
# lives somewhere else, or override CGREENLINKLIB outright (from a
# platform Makefile, Makefile.local or the command line).
#
# Note that this has to name the library, not just add its directory:
# on Cygwin and MinGW every symbol of a DLL must resolve at link time,
# so a missing -lcgreen breaks the build there even though ELF
# platforms happily leave those symbols for the runner to supply.
CGREENROOT ?= /usr/local
CGREENLIBDIR ?= $(CGREENROOT)/lib
CGREENLINKLIB ?= -L$(CGREENLIBDIR) -lcgreen -lm

# Shorthands
COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

# Running inside Emacs? Ensure pretty output
ifneq ($(INSIDE_EMACS),)
  JREGROUTPUT = -noansi
else
  UNITOUTPUT ?= -c
endif

[![Build Status](https://ci.alanif.se/jenkins/job/Alan/badge/icon)](https://ci.alanif.se/jenkins/job/Alan/)[![Build Status](https://travis-ci.org/alan-if/alan.svg?branch=master)](https://travis-ci.org/alan-if/alan)[![Coverage Status](https://coveralls.io/repos/github/alan-if/alan/badge.svg?branch=master)](https://coveralls.io/github/alan-if/alan?branch=master)

# Alan - the Adventure Language

Alan is a language for developing and running interactive fiction,
a.k.a adventure games, i.e. the games are text-based, in the best of
worlds reads like interactive literature, and gives you "the best
graphics device available - the human brain".

## What is this?

This repository is part of the larger ALAN-IF set of repositories with
tools, examples and documentation for the Alan system.


This particular repository contains the source code and build
environment for the Alan compiler, interpreters and an Alan v2 to Alan
v3 source converter.

## How to build?

The build system is fairly self-configuring, you should be able to
just clone and 'make' on the following platform:

- Cygwin
- Msys2, both native and 32/64-bit Windows-compatible
- Linux, most flavours including WSL (Windows Subsystem for Linux)
- MacOS/Darwin, both native and with Homebrew GCC

To run unittests you need Cgreen, and to run all the regression tests
you need a Java runtime.

## What's in the box?

From the repo you can build

- command line compiler and interpreter for you OS
- GLK-based interpreter, you need a GLK-library
- Gargoyle plug-in, which can be used instead of the plug-in that comes with Gargoyle

And if your environment allows cross-compilation to Windows (Cygwin,
MSYS2 and most Linuxen can be made to do this, but you have to install
the cross-compilation tools)

- WinArun, a WindowsGLK based GUI-interpreter
- WinAlan, a Windows "GUI" compiler (but a better choice is probably the AlanIDE)

## What's missing?

A lot probably, but I wanted this Readme to exist as quickly as
possible. Please, join, fork, pull request or file issues if there is
something you want included here.

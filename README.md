[![Jenkins Status][jenkins badge]][jenkins link]&nbsp;
[![Travis CI Status][travis badge]][travis link]&nbsp;
[![Coverage Status][coveralls badge]][coveralls link]&nbsp;
[![License][license badge]][license link]

# Alan — the Adventure Language

Alan is a language for developing and running interactive fiction,
a.k.a adventure games, i.e. the games are text-based, in the best of
worlds reads like interactive literature, and gives you "the best
graphics device available — the human brain".

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

- command line compiler and interpreter for your OS
- GLK-based interpreter, you need a GLK-library
- Gargoyle plug-in, which can be used instead of the plug-in that comes with Gargoyle

And if your environment allows cross-compilation to Windows you can build

- WinArun, a WindowsGLK based GUI-interpreter
- WinAlan, a Windows "GUI" compiler (but a better choice is probably the AlanIDE)

To cross-compile (Cygwin, MSYS2 and most Linuxen can be made to do
this) you have to install the cross-compilation tools (`sudo apt
install mingw-w64` or equivalent) and a matching iconv (sometimes a
separate package like `win-iconv-mingw-w64-dev`).

## What's missing?

A lot probably, but I wanted this Readme to exist as quickly as
possible. Please, join, fork, pull request or file issues if there is
something you want included here.

# Useful Links

- [Alan website]
- [Alan Wiki]
- [Alan Google Group] — for community help, announcements and discussions.
- [Alan IF Development team] — GitHub profile and public projects.

<!-----------------------------------------------------------------------------
                               REFERENCE LINKS
------------------------------------------------------------------------------>

[Artistic License 2.0]: https://opensource.org/licenses/Artistic-2.0

<!-- Useful Links -->

[Alan website]: https://www.alanif.se/ "Visit the Alan website"
[Alan Google Group]: https://groups.google.com/g/alan-if/ "Visit the Alan-IF discussions group at Google"
[Alan Wiki]: https://github.com/alan-if/alan/wiki "Visit Alan Wiki"

<!-- badges -->

[jenkins badge]: https://jenkins.responsive.se/job/Alan/badge/icon "Jenkins CI build status"
[jenkins link]: https://jenkins.responsive.se/job/Alan/
[travis badge]: https://travis-ci.com/alan-if/alan.svg?branch=master "Travis CI build status"
[travis link]: https://travis-ci.com/alan-if/alan
[coveralls badge]: https://coveralls.io/repos/github/alan-if/alan/badge.svg?branch=master "Coveralls status"
[coveralls link]: https://coveralls.io/github/alan-if/alan?branch=master
[license badge]: https://img.shields.io/badge/license-Artistic%20License%202.0-blue
[license link]: ./COPYING

<!-- people and organizations -->

[Alan IF Development team]: https://github.com/alan-if "Visit the Alan Interactive Fiction Development team organization on GitHub"

[Thomas Nilefalk]: https://github.com/thoni56 "View Thomas Nilefalk's GitHub profile"

<!-- EOF -->

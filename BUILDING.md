Building Alan
=============

Intro
-----

It is always difficult to create build scripts that work on a multitude
of operating systems and with different tools.

Some parts of Alan is also build using some proprietary compiler
generation tools and uses some non-standard libraries for unit testing,
Windows GUI etc.

You could go for the autotools that are available on *ix platforms but I
have always felt that they are too powerful and complex for my projects.
I might be wrong. But also they are not available on non-*ix platforms,
so historically it has not been an option for the Alan build suite.

The goal with the current Makefile approach is that it should be
possible to build the command line versions without any changes to the
Makefiles, particularly not the ones in the repository.

File Strategy
-------------

The Alan sources are divided into one directory for the compiler, one
for the interpreter(s) and one for the Alan2-to-Alan3 converter.

In each of the directories, as well as on the top level, a 'Makefile'
exists which will conditionally include other, more specific Makefiles.
They are either an extracted, common "module", or a platform-specific
Makefile.

The main Makefile will investigate which OS it is being run on and
include a platform-specific Makefile, if there is one for the current
OS.

You can trace which files are included by doing

    make TRACE=1 ...

This will log all included files.

By just doing

    make

you should see a successful build of commmand line versions of the
compiler and interpreter.

If you have the unittest framework Cgreen
[Cgreen](https://github.com/cgreen-devs/cgreen) installed, the Makefiles
will normally detect this and first build and run all unittests.

As a last step a large number of tests are run, which requires a working
Java RTE.

Enabling Specific Targets
-------------------------

On some platforms/OSes you can enable extra targets. Extra targets are
versions of the interpreter and compiler that are not the native command
line compiler and interpreter.

The Makefiles will present the possible extra targets during builds with
something like

    NOT ENABLED: glkarun - Arun with curses text Glk

This means that on this platform/OS it might be possible to build a
Curses GLK version of the interpreter.You can try to build it by
explicitly doing

    make glkarun

in the relevant directory (`interpreter` in this case).

The file `Makefile.local.template` includes command and settings to
build these extra targets. This file is included by default. If you find
that an extra target that you want to build does not work, you can copy
`Makefile.local.template` to `Makefile.local`. This file will override
the template file, and changes here will not affect the sources, they
are strictly local.

Of course, many of the extra targets requires extra libraries and you
need to download and possibly compile them and the adjust settings in
`Makefile.local` to make that work.

To enable an extra target so that it is included in every build
uncomment the relevant line in your `Makefile.local`. It looks something
like

    # Enable glkarun:
    #GLKARUN_ENABLED = true

Cross Compilation
-----------------

For platforms that are Windows cross compilation capable (Cygwin, Msys,
Linux) you can do

    make cross

This will create Windows 32-bit compatible versions of the command line
versions of the compiler and the interpreter.

Obviously you need to install the cross-compilation suite for your
platform (usually something like `i686-w64-mingw32-gcc`).

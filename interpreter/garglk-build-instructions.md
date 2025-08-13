# Build instructions for Gargoyle alan3 interpreter plugin on Windows

## Normal build

In the subdirectory `garglk` there are some files that should allow a
compatible `alan3.exe` using a Mingw32-toolchain:

- `libgarglk.dll`
- `libgarglkmain.a`
- `icons.o`

Build using

    $ make gararun

## Test

Copy the resulting `alan3.exe` into the directory of a Gargoyle
installation, probably `C:\\Program Files (x86)\\Gargoyle` or
something similar.

Start Gargoyle and lookup an `.ac3` file to run.  A good file has some
graphics, e.g. `regression/saviour.a3c`.  Look at the graphics and the
menues.  It should have the same icons as other Gargoyle plugins.
Open the about box.

If all that looks ok, it worked and the produced `alan3.exe` can be
distributed as a Gargoyle upgrade plugin.  Those are built by the
Makefile.

## Failed?

Then you probably have to re-build the `libgarglk.dll`,
`libgarglkmain.a` and/or the resource/icon file `icons.o`.

Here are what has worked on Cygwin to make that happen in some
previous versions when Gargoyle builds used Jam.  They have since
moved to CMake, for which I have no instructions. Sorry.

Ansure that the body of the Alan3 build instructions in `terps/Jamfile`
looks like this:

```
    SubDir TOP terps alan3 ;
    ObjDir alan3 ;

    SubDirCcFlags -funsigned-char -DGLK -DHAVE_GARGLK -DBUILD=0 ;

    Main $(GARGLKPRE)alan3 :
        alan.version.c act.c actor.c args.c arun.c attribute.c
        checkentry.c class.c current.c debug.c decode.c
        dictionary.c event.c exe.c glkio.c glkstart.c instance.c
        inter.c lists.c literal.c main.c memory.c msg.c options.c
        output.c params.c parse.c readline.c reverse.c rules.c
        save.c scan.c score.c set.c stack.c state.c syntax.c
        sysdep.c syserr.c term.c utils.c word.c compatibility.c
        AltInfo.c Container.c Location.c ParameterPosition.c StateStack.c
        types.c converter.c
        ;

    APPS += $(GARGLKPRE)alan3 ;
```

Introduce a section for CYGWIN in the switch for `$(OS)`, it is mostly the same as for MINGW with the addition of compilers being `x86_64-w64-mingw32` versions:

```
    case CYGWIN :
        Echo "OS is CYGWIN (win32) $(CROSS)" ;
        SUFDLL  = .dll ;
        CC = x86_64-w64-mingw32-gcc ;
        C++ = x86_64-w64-mingw32-gcc ;
        CXX = x86_64-w64-mingw32-gcc ;
        LINK = x86_64-w64-mingw32-gcc -mwindows ;
        LINKLIBS = -lcomdlg32 -lgdi32 -lwinmm ; # -lm ;
        SHRLINKLIBS = -lcomdlg32 -lgdi32 -lwinmm ; # -lm ;
        SHRLINKLIBS += $(TOP)/support/freetype/freetype6.dll ;
        SHRLINKLIBS += $(TOP)/support/libjpeg/libjpeg-8.dll ;
        SHRLINKLIBS += $(TOP)/support/libpng/libpng14-14.dll ;
        SHRLINKLIBS += $(TOP)/support/zlib/zlib1.dll ;

        if $(BUILD) = RELEASE || $(BUILD) = TURBO
        {
            SHRLINKLIBS += -mwindows ;
            LINKLIBS += -mwindows ;
        }

        if $(USESDL) = yes
        {
            SHRLINKLIBS += $(TOP)/support/sdl/SDL_mixer.dll ;
            SHRLINKLIBS += $(TOP)/support/sdl/SDL.dll ;
        }
```

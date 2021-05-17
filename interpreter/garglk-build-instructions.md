# Build instructions for Gargoyle alan3 interpreter plugin

To cleanly build garglk ("Gargoyle") with this version of the Alan interpreter
replace the body of the Alan3 build instructions in `terps/Jamfile` with

    SubDir TOP terps alan3 ;
    ObjDir alan3 ;

    SubDirCcFlags -funsigned-char -DGLK -DHAVE_GARGLK -DBUILD=0 ;

    SHRLINKLIBS += -liconv ;
    LINKLIBS += -liconv ;

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

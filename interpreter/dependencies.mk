args.o: args.c args.h arun.h types.h sysdep.h acode.h
arun.o: arun.c sysdep.h types.h acode.h arun.h readline.h version.h args.h parse.h \
  inter.h rules.h debug.h stack.h exe.h term.h c25to26.h
c25to26.o: c25to26.c types.h sysdep.h acode.h arun.h c25to26.h
debug.o: debug.c types.h sysdep.h acode.h version.h readline.h inter.h arun.h \
  parse.h exe.h debug.h
decode.o: decode.c arun.h types.h sysdep.h acode.h decode.h

arun.o : arun.c sysdep.h types.h acode.h arun.h version.h parse.h inter.h rules.h \
  reverse.h debug.h stack.h exe.h term.h 
debug.o : debug.c types.h sysdep.h acode.h inter.h arun.h parse.h exe.h 
decode.o : decode.c arun.h types.h sysdep.h acode.h decode.h 
exe.o : exe.c types.h sysdep.h acode.h arun.h parse.h inter.h stack.h decode.h \
  exe.h 
inter.o : inter.c types.h sysdep.h acode.h arun.h parse.h exe.h stack.h 
params.o : params.c types.h sysdep.h acode.h 
parse.o : parse.c types.h sysdep.h acode.h arun.h inter.h exe.h term.h debug.h \
  params.h parse.h 
reverse.o : reverse.c types.h sysdep.h acode.h arun.h 
rules.o : rules.c types.h sysdep.h acode.h arun.h inter.h stack.h 
stack.o : stack.c types.h sysdep.h acode.h arun.h 
sysdep.o : sysdep.c sysdep.h 
term.o : term.c arun.h types.h sysdep.h acode.h 
version.o : version.c version.h 

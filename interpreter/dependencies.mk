unit.o: unit.c sysdep.h acode.h unitTest.h exeTest.c exe.c types.h \
 readline.h main.h parse.h inter.h stack.h decode.h exe.h stackTest.c \
 stack.c interTest.c inter.c
sysdep.o: sysdep.c sysdep.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h debug.h exe.h \
 stack.h rules.h
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
 readline.h inter.h main.h parse.h exe.h debug.h
args.o: args.c args.h main.h types.h sysdep.h acode.h
parse.o: parse.c types.h sysdep.h acode.h readline.h main.h inter.h \
 exe.h term.h debug.h params.h parse.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
readline.o: readline.c sysdep.h readline.h types.h acode.h main.h
params.o: params.c types.h sysdep.h acode.h params.h
main.o: main.c sysdep.h types.h acode.h main.h readline.h \
 alan.version.h version.h args.h parse.h inter.h rules.h reverse.h \
 debug.h stack.h exe.h term.h
exe.o: exe.c types.h sysdep.h acode.h readline.h main.h parse.h \
 inter.h stack.h decode.h exe.h
stack.o: stack.c types.h sysdep.h acode.h main.h stack.h
inter.o: inter.c types.h sysdep.h acode.h main.h parse.h exe.h stack.h \
 inter.h
arun.o: arun.c main.h types.h sysdep.h acode.h term.h alan.version.h \
 version.h args.h

unit.o: unit.c sysdep.h acode.h reverse.h types.h unitTest.h exeTest.c \
  exe.c act.h debug.h readline.h main.h parse.h inter.h stack.h decode.h \
  exe.h stackTest.c stack.c interTest.c inter.c
unit.o: unit.c sysdep.h acode.h reverse.h types.h unitTest.h exeTest.c \
  exe.c act.h debug.h readline.h glkio.h ../../WindowsGLK/Include/glk.h \
  main.h parse.h inter.h stack.h decode.h exe.h stackTest.c stack.c \
  interTest.c inter.c
sysdep.o: sysdep.c sysdep.h
sysdep.o: sysdep.c sysdep.h ../../WindowsGLK/Include/glk.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h debug.h exe.h \
  stack.h rules.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h debug.h exe.h \
  stack.h rules.h glkio.h ../../WindowsGLK/Include/glk.h
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h main.h parse.h exe.h debug.h
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h main.h parse.h exe.h debug.h glkio.h \
  ../../WindowsGLK/Include/glk.h
args.o: args.c args.h main.h types.h sysdep.h acode.h
args.o: args.c args.h main.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h glkio.h
parse.o: parse.c types.h sysdep.h acode.h readline.h main.h inter.h exe.h \
  act.h term.h debug.h params.h parse.h
parse.o: parse.c types.h sysdep.h acode.h readline.h main.h inter.h exe.h \
  act.h term.h debug.h params.h parse.h glkio.h \
  ../../WindowsGLK/Include/glk.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
readline.o: readline.c sysdep.h readline.h types.h acode.h main.h
readline.o: readline.c readline.h types.h sysdep.h acode.h exe.h main.h \
  ../../WindowsGLK/Include/glk.h glkio.h resources.h \
  ../../WindowsGLK/Include/WinGlk.h
params.o: params.c types.h sysdep.h acode.h params.h
params.o: params.c types.h sysdep.h acode.h params.h
act.o: act.c act.h types.h sysdep.h acode.h main.h inter.h exe.h stack.h \
  parse.h debug.h
act.o: act.c act.h types.h sysdep.h acode.h main.h inter.h exe.h stack.h \
  parse.h debug.h
main.o: main.c sysdep.h types.h acode.h main.h readline.h alan.version.h \
  version.h args.h parse.h inter.h rules.h reverse.h debug.h stack.h \
  exe.h term.h
main.o: main.c sysdep.h types.h acode.h main.h readline.h alan.version.h \
  version.h args.h parse.h inter.h rules.h reverse.h debug.h stack.h \
  exe.h term.h ../../WindowsGLK/Include/glk.h glkio.h
exe.o: exe.c types.h sysdep.h acode.h act.h debug.h readline.h main.h \
  parse.h inter.h stack.h decode.h exe.h
exe.o: exe.c types.h sysdep.h acode.h act.h debug.h readline.h glkio.h \
  ../../WindowsGLK/Include/glk.h main.h parse.h inter.h stack.h decode.h \
  exe.h
stack.o: stack.c types.h sysdep.h acode.h main.h stack.h
stack.o: stack.c types.h sysdep.h acode.h main.h stack.h
inter.o: inter.c types.h sysdep.h acode.h main.h parse.h exe.h stack.h \
  inter.h
inter.o: inter.c types.h sysdep.h acode.h main.h parse.h exe.h stack.h \
  inter.h glkio.h ../../WindowsGLK/Include/glk.h
arun.o: arun.c main.h types.h sysdep.h acode.h term.h alan.version.h \
  version.h args.h
arun.o: arun.c main.h types.h sysdep.h acode.h term.h alan.version.h \
  version.h args.h glkio.h ../../WindowsGLK/Include/glk.h \
  ../../WindowsGLK/Include/WinGlk.h
glkstart.o: glkstart.c ../../WindowsGLK/Include/glk.h glkstart.h glkio.h \
  args.h resources.h
glkstart.o: glkstart.c ../../WindowsGLK/Include/glk.h glkstart.h glkio.h \
  args.h resources.h ../../WindowsGLK/Include/WinGlk.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
dumpacd.o: dumpacd.c types.h sysdep.h acode.h reverse.h ../compiler/spa.h

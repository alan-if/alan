unit.o: unit.c sysdep.h acode.h reverse.h types.h syserr.h unitTest.h \
  exeTest.c exe.c act.h set.h debug.h params.h parse.h readline.h main.h \
  inter.h stack.h decode.h exe.h parseTest.c parse.c term.h stackTest.c \
  stack.c interTest.c inter.c reverseTest.c reverse.c sysdepTest.c \
  sysdep.c setTest.c set.c mainTest.c main.c alan.version.h version.h \
  args.h rules.h
unit.o: unit.c sysdep.h acode.h reverse.h types.h syserr.h unitTest.h \
  exeTest.c exe.c act.h set.h debug.h params.h parse.h readline.h glkio.h \
  ../../WindowsGLK/Include/glk.h main.h inter.h stack.h decode.h exe.h \
  parseTest.c parse.c term.h stackTest.c stack.c interTest.c inter.c \
  reverseTest.c reverse.c sysdepTest.c sysdep.c setTest.c set.c \
  mainTest.c main.c alan.version.h version.h args.h rules.h \
  ../../WindowsGLK/Include/winglk.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h debug.h exe.h \
  stack.h rules.h
rules.o: rules.c types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h inter.h debug.h exe.h stack.h rules.h \
  glkio.h
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h main.h parse.h stack.h exe.h debug.h
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h main.h ../../WindowsGLK/Include/glk.h parse.h \
  stack.h exe.h debug.h glkio.h
args.o: args.c sysdep.h args.h types.h acode.h main.h winargs.c
args.o: args.c sysdep.h args.h types.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h glkio.h winargs.c
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h syserr.h
decode.o: decode.c main.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h decode.h syserr.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
term.o: term.c main.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h term.h
readline.o: readline.c sysdep.h readline.h types.h acode.h main.h
readline.o: readline.c sysdep.h readline.h types.h acode.h exe.h main.h \
  ../../WindowsGLK/Include/glk.h glkio.h resources.h \
  ../../WindowsGLK/Include/WinGlk.h
params.o: params.c types.h sysdep.h acode.h params.h main.h
params.o: params.c types.h sysdep.h acode.h params.h main.h \
  ../../WindowsGLK/Include/glk.h
act.o: act.c sysdep.h act.h types.h acode.h main.h inter.h exe.h stack.h \
  parse.h debug.h syserr.h
act.o: act.c sysdep.h act.h types.h acode.h glkio.h \
  ../../WindowsGLK/Include/glk.h main.h inter.h exe.h stack.h parse.h \
  debug.h syserr.h
exe.o: exe.c sysdep.h types.h acode.h act.h set.h debug.h params.h \
  parse.h syserr.h readline.h main.h inter.h stack.h decode.h exe.h
exe.o: exe.c sysdep.h types.h acode.h act.h set.h debug.h params.h \
  parse.h syserr.h readline.h glkio.h ../../WindowsGLK/Include/glk.h \
  main.h inter.h stack.h decode.h exe.h
sysdep.o: sysdep.c sysdep.h
sysdep.o: sysdep.c sysdep.h ../../WindowsGLK/Include/glk.h
stack.o: stack.c types.h sysdep.h acode.h main.h syserr.h stack.h
stack.o: stack.c types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h syserr.h stack.h
inter.o: inter.c types.h sysdep.h acode.h main.h parse.h exe.h stack.h \
  syserr.h debug.h set.h inter.h
inter.o: inter.c types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h parse.h exe.h stack.h syserr.h debug.h \
  set.h inter.h glkio.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h reverse.h
syserr.o: syserr.c main.h types.h sysdep.h acode.h inter.h debug.h
syserr.o: syserr.c main.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h inter.h debug.h
parse.o: parse.c types.h sysdep.h acode.h readline.h main.h inter.h exe.h \
  act.h term.h debug.h params.h syserr.h parse.h
parse.o: parse.c types.h sysdep.h acode.h readline.h main.h \
  ../../WindowsGLK/Include/glk.h inter.h exe.h act.h term.h debug.h \
  params.h syserr.h parse.h glkio.h
main.o: main.c sysdep.h acode.h types.h set.h main.h syserr.h readline.h \
  alan.version.h version.h args.h parse.h inter.h rules.h reverse.h \
  debug.h stack.h exe.h term.h
main.o: main.c sysdep.h acode.h types.h set.h main.h \
  ../../WindowsGLK/Include/glk.h syserr.h readline.h alan.version.h \
  version.h args.h parse.h inter.h rules.h reverse.h debug.h stack.h \
  exe.h term.h glkio.h ../../WindowsGLK/Include/winglk.h
set.o: set.c set.h acode.h types.h sysdep.h main.h syserr.h
set.o: set.c set.h acode.h types.h sysdep.h main.h \
  ../../WindowsGLK/Include/glk.h syserr.h
arun.o: arun.c sysdep.h main.h types.h acode.h term.h alan.version.h \
  version.h args.h
arun.o: arun.c sysdep.h main.h types.h acode.h \
  ../../WindowsGLK/Include/glk.h term.h alan.version.h version.h args.h \
  glkio.h ../../WindowsGLK/Include/WinGlk.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h glkstart.h glkio.h resources.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h \
  ../../WindowsGLK/Include/glk.h glkstart.h glkio.h resources.h \
  ../../WindowsGLK/Include/WinGlk.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
dumpacd.o: dumpacd.c types.h sysdep.h acode.h reverse.h ../compiler/spa.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h reverse.h

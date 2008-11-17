unit.o: unit.c sysdep.h acode.h reverse.h types.h syserr.h saveTest.c \
  set.h save.c save.h main.h exe.h readline.h options.h stateTest.c \
  state.c parse.h parseTest.c parse.c inter.h state.h act.h term.h \
  debug.h params.h stackTest.c stack.c stack.h interTest.c inter.c \
  reverseTest.c reverse.c setTest.c set.c mainTest.c main.c \
  alan.version.h version.h args.h rules.h
AltInfoArrayTests.o: AltInfoArrayTests.c \
  ../../../Development/cgreen/cgreen.h ../../../Development/cgreen/unit.h \
  ../../../Development/cgreen/reporter.h \
  ../../../Development/cgreen/mocks.h \
  ../../../Development/cgreen/constraint.h \
  ../../../Development/cgreen/text_reporter.h \
  ../../../Development/cgreen/assertions.h altInfoArray.c AltInfoArray.h \
  AltInfo.h types.h sysdep.h acode.h main.h
AltInfoTests.o: AltInfoTests.c ../../../Development/cgreen/cgreen.h \
  ../../../Development/cgreen/unit.h \
  ../../../Development/cgreen/reporter.h \
  ../../../Development/cgreen/mocks.h \
  ../../../Development/cgreen/constraint.h \
  ../../../Development/cgreen/text_reporter.h \
  ../../../Development/cgreen/assertions.h altInfo.c AltInfo.h types.h \
  sysdep.h acode.h options.h main.h inter.h
actTests.o: actTests.c ../../../Development/cgreen/cgreen.h \
  ../../../Development/cgreen/unit.h \
  ../../../Development/cgreen/reporter.h \
  ../../../Development/cgreen/mocks.h \
  ../../../Development/cgreen/constraint.h \
  ../../../Development/cgreen/text_reporter.h \
  ../../../Development/cgreen/assertions.h act.c act.h types.h sysdep.h \
  acode.h main.h inter.h exe.h stack.h parse.h debug.h syserr.h options.h \
  AltInfo.h AltInfoArray.h
sysdepTests.o: sysdepTests.c ../../../Development/cgreen/cgreen.h \
  ../../../Development/cgreen/unit.h \
  ../../../Development/cgreen/reporter.h \
  ../../../Development/cgreen/mocks.h \
  ../../../Development/cgreen/constraint.h \
  ../../../Development/cgreen/text_reporter.h \
  ../../../Development/cgreen/assertions.h sysdep.c sysdep.h
exeTests.o: exeTests.c ../../../Development/cgreen/cgreen.h \
  ../../../Development/cgreen/unit.h \
  ../../../Development/cgreen/reporter.h \
  ../../../Development/cgreen/mocks.h \
  ../../../Development/cgreen/constraint.h \
  ../../../Development/cgreen/text_reporter.h \
  ../../../Development/cgreen/assertions.h set.h acode.h types.h sysdep.h \
  exe.c exe.h act.h state.h debug.h params.h parse.h syserr.h options.h \
  readline.h main.h inter.h stack.h decode.h
AltInfo.o: AltInfo.c AltInfo.h types.h sysdep.h acode.h options.h main.h \
  inter.h
AltInfoArray.o: AltInfoArray.c AltInfoArray.h AltInfo.h types.h sysdep.h \
  acode.h main.h
act.o: act.c act.h types.h sysdep.h acode.h main.h inter.h exe.h stack.h \
  parse.h debug.h syserr.h options.h AltInfo.h AltInfoArray.h
args.o: args.c sysdep.h args.h types.h acode.h options.h main.h winargs.c
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h main.h parse.h stack.h exe.h options.h debug.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h syserr.h
exe.o: exe.c exe.h acode.h types.h sysdep.h act.h set.h state.h debug.h \
  params.h parse.h syserr.h options.h readline.h main.h inter.h stack.h \
  decode.h
inter.o: inter.c inter.h types.h sysdep.h acode.h main.h parse.h exe.h \
  stack.h syserr.h debug.h set.h options.h
main.o: main.c sysdep.h acode.h types.h set.h state.h main.h syserr.h \
  parse.h options.h readline.h alan.version.h version.h args.h inter.h \
  rules.h reverse.h debug.h stack.h exe.h term.h
options.o: options.c options.h types.h sysdep.h acode.h
params.o: params.c types.h sysdep.h acode.h params.h main.h
parse.o: parse.c parse.h types.h sysdep.h acode.h readline.h main.h \
  inter.h exe.h state.h act.h term.h debug.h params.h options.h syserr.h
readline.o: readline.c sysdep.h readline.h types.h acode.h main.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h debug.h exe.h \
  stack.h options.h rules.h
save.o: save.c save.h types.h sysdep.h acode.h main.h exe.h set.h \
  readline.h options.h syserr.h
set.o: set.c set.h acode.h types.h sysdep.h main.h syserr.h exe.h
stack.o: stack.c types.h sysdep.h acode.h main.h syserr.h stack.h
state.o: state.c sysdep.h types.h acode.h syserr.h main.h set.h exe.h \
  parse.h
sysdep.o: sysdep.c sysdep.h
syserr.o: syserr.c syserr.h main.h types.h sysdep.h acode.h inter.h \
  debug.h options.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h glkstart.h glkio.h resources.h \
  ../../WindowsGLK/Include/gi_blorb.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
dumpacd.o: dumpacd.c types.h sysdep.h acode.h reverse.h ../compiler/spa.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h

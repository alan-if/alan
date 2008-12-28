unit2.o: unit2.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h
AltInfoArrayTests.o: AltInfoArrayTests.c \
  ../Cgreen/include/cgreen/cgreen.h ../Cgreen/include/cgreen/unit.h \
  ../Cgreen/include/cgreen/reporter.h ../Cgreen/include/cgreen/mocks.h \
  ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h AltInfoArray.c AltInfoArray.h \
  AltInfo.h types.h sysdep.h acode.h main.h parse.h
AltInfoTests.o: AltInfoTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h AltInfo.c AltInfo.h types.h \
  sysdep.h acode.h options.h main.h inter.h Stack.h parse.h debug.h \
  CheckEntryArray.h AltInfoArray.h
StateStackTests.o: StateStackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h StateStack.c StateStack.h types.h \
  sysdep.h acode.h main.h syserr.h
actTests.o: actTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h act.c act.h types.h sysdep.h \
  acode.h main.h parse.h AltInfo.h AltInfoArray.h CheckEntryArray.h
exeTests.o: exeTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.h acode.h types.h sysdep.h \
  exe.c exe.h act.h state.h debug.h params.h parse.h syserr.h options.h \
  readline.h main.h inter.h Stack.h stack.h decode.h CheckEntryArray.h \
  Container.h Location.h
interTests.o: interTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h inter.c inter.h types.h sysdep.h \
  acode.h Stack.h main.h parse.h exe.h stack.h syserr.h debug.h set.h \
  options.h
mainTests.o: mainTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h main.c sysdep.h acode.h types.h \
  set.h state.h main.h syserr.h parse.h options.h readline.h \
  alan.version.h version.h args.h inter.h Stack.h rules.h reverse.h \
  debug.h stack.h exe.h term.h
parseTests.o: parseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h parse.c parse.h types.h sysdep.h \
  acode.h readline.h AltInfoArray.h AltInfo.h main.h inter.h Stack.h \
  exe.h act.h term.h debug.h params.h options.h syserr.h Location.h
reverseTests.o: reverseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h reverse.c types.h sysdep.h \
  acode.h main.h reverse.h
saveTests.o: saveTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h save.c save.h types.h sysdep.h \
  acode.h main.h exe.h set.h readline.h options.h syserr.h
setTests.o: setTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.c set.h acode.h types.h \
  sysdep.h main.h syserr.h exe.h
stackTests.o: stackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h stack.c stack.h acode.h types.h \
  sysdep.h main.h syserr.h
stateTests.o: stateTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h state.c sysdep.h types.h acode.h \
  syserr.h main.h set.h exe.h parse.h StateStack.h
sysdepTests.o: sysdepTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h sysdep.c sysdep.h
AltInfo.o: AltInfo.c AltInfo.h types.h sysdep.h acode.h options.h main.h \
  inter.h Stack.h parse.h debug.h CheckEntryArray.h
AltInfoArray.o: AltInfoArray.c AltInfoArray.h AltInfo.h types.h sysdep.h \
  acode.h main.h parse.h
AttributeEntryArray.o: AttributeEntryArray.c AttributeEntryArray.h \
  acode.h main.h types.h sysdep.h syserr.h state.h
CheckEntryArray.o: CheckEntryArray.c CheckEntryArray.h types.h sysdep.h \
  acode.h main.h inter.h Stack.h stack.h
Container.o: Container.c Container.h types.h sysdep.h acode.h main.h \
  exe.h syserr.h inter.h Stack.h
Location.o: Location.c Location.h types.h sysdep.h acode.h main.h \
  options.h parse.h inter.h Stack.h exe.h CheckEntryArray.h debug.h
StateStack.o: StateStack.c StateStack.h types.h sysdep.h acode.h main.h \
  syserr.h
act.o: act.c act.h types.h sysdep.h acode.h main.h parse.h AltInfo.h \
  AltInfoArray.h CheckEntryArray.h
args.o: args.c sysdep.h args.h types.h acode.h options.h main.h \
  unixargs.c
debug.o: debug.c types.h sysdep.h acode.h alan.version.h version.h \
  readline.h inter.h Stack.h main.h parse.h stack.h exe.h options.h \
  debug.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h syserr.h
exe.o: exe.c exe.h acode.h types.h sysdep.h act.h set.h state.h debug.h \
  params.h parse.h syserr.h options.h readline.h main.h inter.h Stack.h \
  stack.h decode.h CheckEntryArray.h Container.h Location.h
inter.o: inter.c inter.h types.h sysdep.h acode.h Stack.h main.h parse.h \
  exe.h stack.h syserr.h debug.h set.h options.h
main.o: main.c sysdep.h acode.h types.h set.h state.h main.h syserr.h \
  parse.h options.h readline.h alan.version.h version.h args.h inter.h \
  Stack.h rules.h reverse.h debug.h stack.h exe.h term.h
options.o: options.c options.h types.h sysdep.h acode.h
params.o: params.c types.h sysdep.h acode.h params.h main.h
parse.o: parse.c parse.h types.h sysdep.h acode.h readline.h \
  AltInfoArray.h AltInfo.h main.h inter.h Stack.h exe.h act.h term.h \
  debug.h params.h options.h syserr.h Location.h
readline.o: readline.c sysdep.h readline.h types.h acode.h main.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h
rules.o: rules.c types.h sysdep.h acode.h main.h inter.h Stack.h debug.h \
  exe.h stack.h options.h rules.h
save.o: save.c save.h types.h sysdep.h acode.h main.h exe.h set.h \
  readline.h options.h syserr.h
set.o: set.c set.h acode.h types.h sysdep.h main.h syserr.h exe.h
stack.o: stack.c stack.h acode.h types.h sysdep.h main.h syserr.h
state.o: state.c sysdep.h types.h acode.h syserr.h main.h set.h exe.h \
  parse.h StateStack.h
sysdep.o: sysdep.c sysdep.h
syserr.o: syserr.c syserr.h main.h types.h sysdep.h acode.h inter.h \
  Stack.h debug.h options.h
term.o: term.c main.h types.h sysdep.h acode.h term.h
reverse.o: reverse.c types.h sysdep.h acode.h main.h reverse.h

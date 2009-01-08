unit2.o: unit2.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h
AltInfoTests.o: AltInfoTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h AltInfo.c AltInfo.h types.h \
  sysdep.h acode.h checkentry.h debug.h inter.h Stack.h lists.h \
  instance.h options.h parse.h memory.h exe.h set.h class.h main.h
StateStackTests.o: StateStackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h StateStack.c StateStack.h types.h \
  sysdep.h acode.h main.h syserr.h instance.h
actTests.o: actTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h act.c act.h types.h sysdep.h \
  acode.h parse.h AltInfo.h checkentry.h params.h output.h main.h lists.h
exeTests.o: exeTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.h acode.h types.h sysdep.h \
  exe.c exe.h lists.h state.h params.h syserr.h options.h term.h utils.h \
  instance.h parse.h inter.h Stack.h decode.h checkentry.h Container.h \
  Location.h save.h debug.h AttributeEntry.h memory.h output.h \
  dictionary.h class.h main.h readline.h
interTests.o: interTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h inter.c inter.h types.h sysdep.h \
  acode.h Stack.h parse.h exe.h set.h stack.h syserr.h debug.h options.h \
  save.h memory.h output.h main.h
mainTests.o: mainTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h main.c main.h types.h sysdep.h \
  acode.h set.h state.h lists.h syserr.h parse.h params.h options.h \
  utils.h args.h inter.h Stack.h rules.h reverse.h debug.h stack.h exe.h \
  term.h instance.h memory.h output.h Container.h dictionary.h class.h \
  readline.h alan.version.h version.h
parseTests.o: parseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h parse.c parse.h types.h sysdep.h \
  acode.h readline.h AltInfo.h inter.h Stack.h exe.h set.h act.h term.h \
  lists.h debug.h params.h options.h syserr.h Location.h instance.h \
  memory.h output.h dictionary.h main.h
paramsTests.o: paramsTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h params.c types.h sysdep.h acode.h \
  lists.h params.h main.h
reverseTests.o: reverseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h reverse.c reverse.h types.h \
  sysdep.h acode.h lists.h
saveTests.o: saveTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h save.c save.h types.h sysdep.h \
  acode.h exe.h set.h lists.h readline.h options.h syserr.h instance.h \
  memory.h output.h main.h
setTests.o: setTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.c set.h acode.h types.h \
  sysdep.h lists.h main.h syserr.h exe.h memory.h
stackTests.o: stackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h stack.c stack.h acode.h types.h \
  sysdep.h main.h syserr.h memory.h
stateTests.o: stateTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h state.c state.h types.h sysdep.h \
  acode.h syserr.h exe.h set.h parse.h StateStack.h instance.h \
  AttributeEntry.h memory.h main.h
sysdepTests.o: sysdepTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h sysdep.c sysdep.h
outputTests.o: outputTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h output.c output.h types.h \
  sysdep.h acode.h options.h memory.h parse.h exe.h set.h lists.h term.h \
  syserr.h dictionary.h inter.h Stack.h params.h main.h
smartall.o: smartall.c
AltInfo.o: AltInfo.c AltInfo.h types.h sysdep.h acode.h checkentry.h \
  debug.h inter.h Stack.h lists.h instance.h options.h parse.h memory.h \
  exe.h set.h class.h main.h
AttributeEntry.o: AttributeEntry.c AttributeEntry.h acode.h main.h \
  types.h sysdep.h syserr.h state.h lists.h
checkentry.o: checkentry.c checkentry.h types.h sysdep.h acode.h main.h \
  inter.h Stack.h stack.h lists.h memory.h
Container.o: Container.c Container.h types.h sysdep.h acode.h instance.h \
  exe.h set.h syserr.h inter.h Stack.h lists.h memory.h
Location.o: Location.c Location.h types.h sysdep.h acode.h instance.h \
  options.h parse.h inter.h Stack.h exe.h set.h lists.h checkentry.h \
  debug.h memory.h dictionary.h main.h
StateStack.o: StateStack.c StateStack.h types.h sysdep.h acode.h main.h \
  syserr.h
act.o: act.c act.h types.h sysdep.h acode.h parse.h AltInfo.h \
  checkentry.h params.h output.h main.h
args.o: args.c sysdep.h args.h types.h acode.h options.h utils.h main.h \
  unixargs.c
debug.o: debug.c debug.h types.h sysdep.h acode.h alan.version.h \
  version.h readline.h lists.h inter.h Stack.h parse.h stack.h exe.h \
  set.h options.h utils.h instance.h memory.h output.h class.h main.h
decode.o: decode.c main.h types.h sysdep.h acode.h decode.h syserr.h
dictionary.o: dictionary.c dictionary.h acode.h types.h sysdep.h parse.h
exe.o: exe.c exe.h acode.h types.h sysdep.h set.h lists.h state.h \
  params.h syserr.h options.h term.h utils.h instance.h parse.h inter.h \
  Stack.h decode.h checkentry.h Container.h Location.h save.h debug.h \
  AttributeEntry.h memory.h output.h dictionary.h class.h main.h \
  readline.h
instance.o: instance.c instance.h acode.h types.h sysdep.h memory.h \
  parse.h exe.h set.h
inter.o: inter.c inter.h types.h sysdep.h acode.h Stack.h parse.h exe.h \
  set.h stack.h syserr.h debug.h options.h save.h memory.h output.h \
  main.h
lists.o: lists.c lists.h acode.h types.h sysdep.h
main.o: main.c main.h types.h sysdep.h acode.h set.h state.h lists.h \
  syserr.h parse.h params.h options.h utils.h args.h inter.h Stack.h \
  rules.h reverse.h debug.h stack.h exe.h term.h instance.h memory.h \
  output.h Container.h dictionary.h class.h readline.h alan.version.h \
  version.h
memory.o: memory.c memory.h acode.h types.h sysdep.h syserr.h
options.o: options.c options.h types.h sysdep.h acode.h
output.o: output.c output.h types.h sysdep.h acode.h options.h memory.h \
  parse.h exe.h set.h lists.h term.h syserr.h dictionary.h inter.h \
  Stack.h params.h main.h
params.o: params.c types.h sysdep.h acode.h lists.h params.h main.h
parse.o: parse.c parse.h types.h sysdep.h acode.h readline.h AltInfo.h \
  inter.h Stack.h exe.h set.h act.h term.h lists.h debug.h params.h \
  options.h syserr.h Location.h instance.h memory.h output.h dictionary.h \
  main.h
readline.o: readline.c sysdep.h output.h types.h acode.h readline.h \
  main.h
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h
rules.o: rules.c types.h sysdep.h acode.h lists.h main.h inter.h Stack.h \
  debug.h exe.h set.h stack.h options.h rules.h
save.o: save.c save.h types.h sysdep.h acode.h exe.h set.h lists.h \
  readline.h options.h syserr.h instance.h memory.h output.h main.h
set.o: set.c set.h acode.h types.h sysdep.h lists.h main.h syserr.h exe.h \
  memory.h
stack.o: stack.c stack.h acode.h types.h sysdep.h main.h syserr.h \
  memory.h
state.o: state.c state.h types.h sysdep.h acode.h syserr.h exe.h set.h \
  parse.h StateStack.h instance.h AttributeEntry.h memory.h main.h
sysdep.o: sysdep.c sysdep.h
syserr.o: syserr.c syserr.h inter.h types.h sysdep.h acode.h Stack.h \
  debug.h options.h utils.h exe.h set.h output.h
term.o: term.c term.h memory.h acode.h output.h types.h sysdep.h
utils.o: utils.c utils.h alan.version.h version.h options.h types.h \
  sysdep.h acode.h args.h memory.h output.h main.h
smartall.o: smartall.c
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h

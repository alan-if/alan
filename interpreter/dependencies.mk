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
  sysdep.h acode.h checkentry.h debug.h inter.h stack.h lists.h \
  instance.h set.h options.h memory.h current.h class.h params.h exe.h
StateStackTests.o: StateStackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h StateStack.c StateStack.h types.h \
  sysdep.h acode.h syserr.h memory.h instance.h set.h event.h
actTests.o: actTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h act.c act.h params.h types.h \
  sysdep.h acode.h AltInfo.h output.h msg.h lists.h
interTests.o: interTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h inter.c inter.h types.h sysdep.h \
  acode.h stack.h parse.h params.h current.h exe.h set.h syserr.h debug.h \
  options.h save.h memory.h output.h score.h instance.h
mainTests.o: mainTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h main.c main.h types.h sysdep.h \
  acode.h set.h state.h lists.h syserr.h parse.h params.h options.h \
  utils.h args.h inter.h stack.h rules.h reverse.h debug.h exe.h term.h \
  instance.h memory.h output.h Container.h dictionary.h class.h score.h \
  decode.h msg.h event.h syntax.h current.h readline.h alan.version.h \
  version.h
parseTests.o: parseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h parse.c parse.h types.h sysdep.h \
  acode.h params.h readline.h AltInfo.h inter.h stack.h current.h act.h \
  term.h lists.h debug.h options.h syserr.h Location.h instance.h set.h \
  memory.h output.h dictionary.h syntax.h word.h msg.h exe.h
paramsTests.o: paramsTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h params.c params.h types.h \
  sysdep.h acode.h lists.h memory.h
reverseTests.o: reverseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h reverse.c reverse.h types.h \
  sysdep.h acode.h lists.h checkentry.h rules.h
saveTests.o: saveTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h save.c save.h types.h sysdep.h \
  acode.h current.h set.h lists.h readline.h options.h syserr.h \
  instance.h memory.h output.h args.h score.h event.h msg.h params.h \
  exe.h
setTests.o: setTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.c set.h acode.h types.h \
  sysdep.h lists.h syserr.h memory.h instance.h
stackTests.o: stackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/cute_reporter.h \
  ../Cgreen/include/cgreen/assertions.h stack.c stack.h acode.h types.h \
  sysdep.h syserr.h memory.h
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
  sysdep.h acode.h options.h memory.h word.h lists.h term.h syserr.h \
  dictionary.h current.h inter.h stack.h params.h msg.h readline.h exe.h \
  set.h
smartall.o: smartall.c
AltInfo.o: AltInfo.c AltInfo.h types.h sysdep.h acode.h checkentry.h \
  debug.h inter.h stack.h lists.h instance.h set.h options.h memory.h \
  current.h class.h params.h exe.h
checkentry.o: checkentry.c checkentry.h types.h sysdep.h acode.h inter.h \
  stack.h lists.h memory.h
Container.o: Container.c Container.h types.h sysdep.h acode.h instance.h \
  set.h exe.h syserr.h inter.h stack.h lists.h memory.h
Location.o: Location.c Location.h types.h sysdep.h acode.h instance.h \
  set.h options.h word.h inter.h stack.h lists.h checkentry.h debug.h \
  memory.h dictionary.h output.h msg.h params.h exe.h
StateStack.o: StateStack.c StateStack.h types.h sysdep.h acode.h syserr.h \
  memory.h
act.o: act.c act.h params.h types.h sysdep.h acode.h AltInfo.h output.h \
  msg.h
args.o: args.c sysdep.h args.h types.h acode.h options.h utils.h \
  unixargs.c
class.o: class.c class.h acode.h
current.o: current.c current.h types.h sysdep.h acode.h
debug.o: debug.c debug.h types.h sysdep.h acode.h alan.version.h \
  version.h readline.h lists.h inter.h stack.h parse.h params.h current.h \
  options.h utils.h instance.h set.h memory.h output.h class.h event.h \
  exe.h
decode.o: decode.c decode.h types.h sysdep.h acode.h syserr.h exe.h set.h \
  memory.h
dictionary.o: dictionary.c dictionary.h acode.h types.h sysdep.h word.h
event.o: event.c event.h acode.h types.h sysdep.h
inter.o: inter.c inter.h types.h sysdep.h acode.h stack.h parse.h \
  params.h current.h exe.h set.h syserr.h debug.h options.h save.h \
  memory.h output.h score.h instance.h
lists.o: lists.c lists.h acode.h types.h sysdep.h
main.o: main.c main.h types.h sysdep.h acode.h set.h state.h lists.h \
  syserr.h parse.h params.h options.h utils.h args.h inter.h stack.h \
  rules.h reverse.h debug.h exe.h term.h instance.h memory.h output.h \
  Container.h dictionary.h class.h score.h decode.h msg.h event.h \
  syntax.h current.h readline.h alan.version.h version.h
memory.o: memory.c memory.h acode.h types.h sysdep.h syserr.h
msg.o: msg.c msg.h types.h sysdep.h acode.h params.h inter.h stack.h \
  exe.h set.h
options.o: options.c options.h types.h sysdep.h acode.h
output.o: output.c output.h types.h sysdep.h acode.h options.h memory.h \
  word.h lists.h term.h syserr.h dictionary.h current.h inter.h stack.h \
  params.h msg.h readline.h exe.h set.h
params.o: params.c params.h types.h sysdep.h acode.h lists.h memory.h
parse.o: parse.c parse.h types.h sysdep.h acode.h params.h readline.h \
  AltInfo.h inter.h stack.h current.h act.h term.h lists.h debug.h \
  options.h syserr.h Location.h instance.h set.h memory.h output.h \
  dictionary.h syntax.h word.h msg.h exe.h
readline.o: readline.c sysdep.h output.h types.h acode.h readline.h \
  memory.h
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h \
  checkentry.h rules.h
rules.o: rules.c rules.h acode.h lists.h types.h sysdep.h inter.h stack.h \
  debug.h current.h options.h
save.o: save.c save.h types.h sysdep.h acode.h current.h set.h lists.h \
  readline.h options.h syserr.h instance.h memory.h output.h args.h \
  score.h event.h msg.h params.h exe.h
score.o: score.c score.h acode.h
set.o: set.c set.h acode.h types.h sysdep.h lists.h syserr.h memory.h \
  instance.h
stack.o: stack.c stack.h acode.h types.h sysdep.h syserr.h memory.h
syntax.o: syntax.c syntax.h types.h sysdep.h acode.h
sysdep.o: sysdep.c sysdep.h
syserr.o: syserr.c syserr.h inter.h types.h sysdep.h acode.h stack.h \
  debug.h options.h utils.h current.h output.h
term.o: term.c term.h memory.h acode.h output.h types.h sysdep.h
utils.o: utils.c utils.h alan.version.h version.h options.h types.h \
  sysdep.h acode.h args.h memory.h output.h
word.o: word.c word.h types.h sysdep.h acode.h memory.h
smartall.o: smartall.c
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h \
  checkentry.h rules.h

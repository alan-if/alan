unittests.o: unittests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h
AltInfoTests.o: AltInfoTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h AltInfo.c AltInfo.h types.h \
  sysdep.h acode.h checkentry.h debug.h inter.h stack.h lists.h \
  instance.h set.h options.h memory.h current.h class.h params.h \
  literal.h
StateStackTests.o: StateStackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h StateStack.c StateStack.h types.h \
  sysdep.h acode.h syserr.h memory.h instance.h set.h event.h
actTests.o: actTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h act.c act.h params.h types.h \
  sysdep.h acode.h AltInfo.h output.h msg.h lists.h
exeTests.o: exeTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h exe.c exe.h sysdep.h acode.h \
  types.h set.h lists.h state.h syserr.h term.h utils.h instance.h \
  inter.h stack.h decode.h save.h memory.h output.h score.h event.h \
  current.h word.h msg.h params.h actor.h readline.h class.h container.h
interTests.o: interTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h inter.c inter.h types.h sysdep.h \
  acode.h stack.h current.h exe.h set.h syserr.h debug.h options.h save.h \
  memory.h output.h score.h params.h instance.h Container.h
mainTests.o: mainTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h main.c main.h types.h sysdep.h \
  acode.h state.h lists.h syserr.h options.h utils.h args.h parse.h \
  params.h inter.h stack.h rules.h reverse.h debug.h exe.h set.h term.h \
  instance.h memory.h output.h Container.h dictionary.h class.h score.h \
  decode.h msg.h event.h syntax.h current.h literal.h readline.h \
  alan.version.h version.h
parseTests.o: parseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h parse.c parse.h types.h sysdep.h \
  acode.h params.h readline.h AltInfo.h inter.h stack.h current.h act.h \
  term.h lists.h debug.h options.h syserr.h Location.h instance.h set.h \
  memory.h output.h dictionary.h syntax.h word.h msg.h literal.h exe.h
paramsTests.o: paramsTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h params.c params.h types.h \
  sysdep.h acode.h lists.h memory.h literal.h
reverseTests.o: reverseTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h reverse.c reverse.h types.h \
  sysdep.h acode.h lists.h checkentry.h rules.h msg.h params.h
saveTests.o: saveTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h save.c save.h current.h types.h \
  sysdep.h acode.h lists.h readline.h options.h syserr.h instance.h set.h \
  memory.h output.h args.h score.h event.h msg.h params.h
setTests.o: setTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h set.c set.h acode.h types.h \
  sysdep.h lists.h syserr.h memory.h instance.h
stackTests.o: stackTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h stack.c stack.h acode.h types.h \
  sysdep.h syserr.h memory.h
stateTests.o: stateTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h state.c state.h types.h sysdep.h \
  acode.h syserr.h current.h word.h StateStack.h instance.h set.h \
  attribute.h memory.h score.h event.h
sysdepTests.o: sysdepTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h sysdep.c sysdep.h syserr.h
outputTests.o: outputTests.c ../Cgreen/include/cgreen/cgreen.h \
  ../Cgreen/include/cgreen/unit.h ../Cgreen/include/cgreen/reporter.h \
  ../Cgreen/include/cgreen/mocks.h ../Cgreen/include/cgreen/constraint.h \
  ../Cgreen/include/cgreen/text_reporter.h \
  ../Cgreen/include/cgreen/assertions.h output.c output.h types.h \
  sysdep.h acode.h options.h memory.h word.h lists.h term.h syserr.h \
  dictionary.h current.h msg.h params.h readline.h instance.h set.h
smartall.o: smartall.c
actor.o: actor.c actor.h acode.h instance.h types.h sysdep.h set.h \
  memory.h lists.h inter.h stack.h msg.h params.h Container.h
AltInfo.o: AltInfo.c AltInfo.h types.h sysdep.h acode.h checkentry.h \
  debug.h inter.h stack.h lists.h instance.h set.h options.h memory.h \
  current.h class.h params.h literal.h
attribute.o: attribute.c attribute.h acode.h syserr.h current.h types.h \
  sysdep.h lists.h
checkentry.o: checkentry.c checkentry.h types.h sysdep.h acode.h inter.h \
  stack.h lists.h memory.h
Container.o: Container.c Container.h types.h sysdep.h acode.h instance.h \
  set.h syserr.h inter.h stack.h lists.h memory.h current.h msg.h \
  params.h output.h
Location.o: Location.c Location.h types.h sysdep.h acode.h instance.h \
  set.h options.h word.h inter.h stack.h lists.h checkentry.h debug.h \
  memory.h dictionary.h output.h msg.h params.h current.h
StateStack.o: StateStack.c StateStack.h types.h sysdep.h acode.h syserr.h \
  memory.h
act.o: act.c act.h params.h types.h sysdep.h acode.h AltInfo.h output.h \
  msg.h
args.o: args.c sysdep.h args.h types.h acode.h options.h memory.h utils.h \
  winargs.c
class.o: class.c class.h acode.h
current.o: current.c current.h types.h sysdep.h acode.h
debug.o: debug.c debug.h types.h sysdep.h acode.h alan.version.h \
  version.h readline.h lists.h inter.h stack.h current.h options.h \
  utils.h instance.h set.h memory.h output.h class.h event.h exe.h
decode.o: decode.c decode.h types.h sysdep.h acode.h syserr.h exe.h set.h \
  memory.h
dictionary.o: dictionary.c dictionary.h acode.h types.h sysdep.h word.h
event.o: event.c event.h acode.h types.h sysdep.h
exe.o: exe.c exe.h sysdep.h acode.h types.h set.h lists.h state.h \
  syserr.h term.h utils.h instance.h inter.h stack.h decode.h save.h \
  memory.h output.h score.h event.h current.h word.h msg.h params.h \
  actor.h readline.h
instance.o: instance.c instance.h acode.h types.h sysdep.h set.h memory.h \
  syserr.h attribute.h current.h Container.h options.h debug.h \
  checkentry.h inter.h stack.h output.h class.h msg.h params.h actor.h \
  literal.h dictionary.h Location.h
inter.o: inter.c inter.h types.h sysdep.h acode.h stack.h current.h exe.h \
  set.h syserr.h debug.h options.h save.h memory.h output.h score.h \
  params.h instance.h Container.h
lists.o: lists.c lists.h acode.h types.h sysdep.h
literal.o: literal.c literal.h acode.h types.h sysdep.h memory.h
main.o: main.c main.h types.h sysdep.h acode.h state.h lists.h syserr.h \
  options.h utils.h args.h parse.h params.h inter.h stack.h rules.h \
  reverse.h debug.h exe.h set.h term.h instance.h memory.h output.h \
  Container.h dictionary.h class.h score.h decode.h msg.h event.h \
  syntax.h current.h literal.h readline.h alan.version.h version.h
memory.o: memory.c memory.h acode.h types.h sysdep.h syserr.h
msg.o: msg.c msg.h acode.h types.h sysdep.h params.h inter.h stack.h \
  exe.h set.h lists.h
options.o: options.c options.h types.h sysdep.h acode.h
output.o: output.c output.h types.h sysdep.h acode.h options.h memory.h \
  word.h lists.h term.h syserr.h dictionary.h current.h msg.h params.h \
  readline.h instance.h set.h
params.o: params.c params.h types.h sysdep.h acode.h lists.h memory.h \
  literal.h
parse.o: parse.c parse.h types.h sysdep.h acode.h params.h readline.h \
  AltInfo.h inter.h stack.h current.h act.h term.h lists.h debug.h \
  options.h syserr.h Location.h instance.h set.h memory.h output.h \
  dictionary.h syntax.h word.h msg.h literal.h exe.h
readline.o: readline.c sysdep.h output.h types.h acode.h term.h exe.h \
  set.h save.h readline.h memory.h
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h \
  checkentry.h rules.h msg.h params.h
rules.o: rules.c rules.h acode.h lists.h types.h sysdep.h inter.h stack.h \
  debug.h current.h options.h
save.o: save.c save.h current.h types.h sysdep.h acode.h lists.h \
  readline.h options.h syserr.h instance.h set.h memory.h output.h args.h \
  score.h event.h msg.h params.h
score.o: score.c score.h acode.h
set.o: set.c set.h acode.h types.h sysdep.h lists.h syserr.h memory.h \
  instance.h
stack.o: stack.c stack.h acode.h types.h sysdep.h syserr.h memory.h
state.o: state.c state.h types.h sysdep.h acode.h syserr.h current.h \
  word.h StateStack.h instance.h set.h attribute.h memory.h score.h \
  event.h
syntax.o: syntax.c syntax.h types.h sysdep.h acode.h
sysdep.o: sysdep.c sysdep.h syserr.h
syserr.o: syserr.c syserr.h debug.h types.h sysdep.h acode.h utils.h \
  current.h output.h
term.o: term.c sysdep.h memory.h acode.h output.h types.h options.h \
  instance.h set.h current.h
utils.o: utils.c utils.h alan.version.h version.h options.h types.h \
  sysdep.h acode.h memory.h output.h
word.o: word.c word.h types.h sysdep.h acode.h memory.h
smartall.o: smartall.c
glkstart.o: glkstart.c args.h types.h sysdep.h acode.h main.h \
  ../../WindowsGLK/Include/glk.h glkstart.h glkio.h resources.h \
  ../../WindowsGLK/Include/gi_blorb.h utils.h
glkio.o: glkio.c ../../WindowsGLK/Include/glk.h glkio.h
dumpacd.o: dumpacd.c types.h sysdep.h acode.h checkentry.h reverse.h \
  msg.h params.h ../compiler/spa.h
reverse.o: reverse.c reverse.h types.h sysdep.h acode.h lists.h \
  checkentry.h rules.h msg.h params.h

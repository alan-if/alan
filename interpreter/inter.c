/*----------------------------------------------------------------------*\

  inter.c

  Interpreter unit for Alan interpreter Arun

\*----------------------------------------------------------------------*/


#include <stdio.h>

#include "types.h"
#include "main.h"
#include "parse.h"
#include "exe.h"
#include "stack.h"
#include "syserr.h"
#include "sysdep.h"
#include "debug.h"
#include "set.h"

#include "inter.h"

#ifdef HAVE_GLK
#define MAP_STDIO_TO_GLK
#include "glkio.h"
#endif


Bool stopAtNextLine = FALSE;


/* PRIVATE DATA */

static int pc;


/*----------------------------------------------------------------------*/
static void traceSkip() {
  printf("\n    : \t\t\t\t\t\t\t");
}


/*----------------------------------------------------------------------*/
static void interpretIf(Aword v)
{
  int lev = 1;
  Aword i;

  if (!v) {
    /* Skip to next ELSE or ENDIF on same level */
    if (singleStepOption) traceSkip();
    while (TRUE) {
      i = memory[pc++];
      if (I_CLASS(i) == (Aword)C_STMOP)
	switch (I_OP(i)) {
	case I_ELSE:
	  if (lev == 1) {
	    if (singleStepOption)
	      printf("\n%4x: ELSE\t\t\t\t\t\t", pc);
	    return;
	  }
	  break;
	case I_IF:
	  lev++;
	  break;
	case I_ENDIF:
	  lev--;
	  if (lev == 0) {
	    if (singleStepOption)
	      printf("\n%4x: ENDIF\t\t\t\t\t\t", pc);
	    return;
	  }
	  break;
	}
    }
  }
}


/*----------------------------------------------------------------------*/
static void interpretElse(void)
{
  int lev = 1;
  Aword i;

  if (singleStepOption) traceSkip();
  while (TRUE) {
    /* Skip to ENDIF on the same level */
    i = memory[pc++];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_ENDIF:
	lev--;
	if (lev == 0) return;
	break;
      case I_IF:
	lev++;
	break;
      }
  }
}


/*----------------------------------------------------------------------*/
static void goToAGREND(void)
{
  while (memory[pc] != INSTRUCTION(I_AGREND))
    pc++;
  if (singleStepOption) traceSkip();
}


/*----------------------------------------------------------------------*/
static void goToAGRSTART(void)
{
  while (memory[pc-1] != INSTRUCTION(I_AGRSTART))
    pc--;
  if (singleStepOption) traceSkip();
}

/*----------------------------------------------------------------------*/
static void goToENDEACH(void) {
  int level = 1;
  int i;

  if (singleStepOption) traceSkip();
  while (TRUE) {
    /* Skip past ENDEACH on the same level */
    i = memory[pc];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_ENDEACH:
	level--;
	if (level == 0)
	  return;
	break;
      case I_EACH:
	level++;
	break;
      }
    pc++;
  }
}


/*----------------------------------------------------------------------*/
static void goToEACH(void) {
  int level = 1;
  int i;

  if (singleStepOption) traceSkip();
  pc--;				/* Ignore the instruction we're on */
  while (TRUE) {
    /* Skip back past EACH on the same level */
    i = memory[--pc];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_ENDEACH:
	level++;
	break;
      case I_EACH:
	level--;
	if (level == 0) {
	  return;
	}
	break;
      }
  }
}


/*----------------------------------------------------------------------*/
static void nextEach(void)
{
  goToENDEACH();
}  


/*----------------------------------------------------------------------*/
static void endEach(void)
{
  Aint counter = getLocal(0, 1);
  Aint limit = top();

  if (counter < limit) {
    counter++;
    setLocal(0, 1, counter);
    goToEACH();
    if (singleStepOption)
      printf("\n%4x: EACH\t\t\t\t\t\t", pc);
    pc++;
  } else
    pop();
}


/*----------------------------------------------------------------------*/
static void stackDup(void)
{
  push(top());
}


/*----------------------------------------------------------------------*/
static void depexec(Aword v)
{
  int lev = 1;
  Aword i;
  char *instructionString = "DEPELSE";

  if (!v) {
    /* The expression was not true, skip to next CASE on the same
       level which could be a DEPCASE or DEPELSE */
    if (singleStepOption) printf("\n    : ");
    while (TRUE) {
      i = memory[pc++];
      if (I_CLASS(i) == (Aword)C_STMOP)
	switch (I_OP(i)) {
	case I_DEPEND:
	  lev++;
	  break;
	case I_ENDDEP:
	  if (lev == 1) {
	    pc--;
	    if (singleStepOption)
	      printf("\n%4x: ENDDEP", pc);
	    return;
	  } else
	    lev--;
	  break;
	case I_DEPCASE:
	  instructionString = "DEPCASE";
	case I_DEPELSE:
	  if (lev == 1) {
	    if (singleStepOption)
	      printf("\n%4x: %s", pc, instructionString);
	    return;
	  }
	  break;
	}
    }
  }
}


/*----------------------------------------------------------------------*/
static void depcase(void)
{
  int lev = 1;
  Aword i;

  /* 
     We have just executed a DEPCASE/DEPELSE clause as a result of a
     DEPCASE catching so skip to end of DEPENDING block (next DEPEND
     on same level) then return.
  */

  if (singleStepOption) printf("\n    : ");
  while (TRUE) {
    i = memory[pc++];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_DEPEND:
	lev++;
	break;
      case I_ENDDEP:
	lev--;
	if (lev == 0) {
	  pc--;
	  return;
	}
	break;
      }
  }
}


/*----------------------------------------------------------------------*/
static void traceBooleanTopValue() {
  if (singleStepOption) {
    if (top()) printf("\t=TRUE\t\t");
    else printf("\t=FALSE\t\t");
  }
}

/*----------------------------------------------------------------------*/
static void traceIntegerTopValue() {
  if (singleStepOption) {
    printf("\t=%ld\t", top());
    if (top() <= 99999)
      printf("\t");
  }
}

/*----------------------------------------------------------------------*/
static void traceInstanceTopValue() {
  if (singleStepOption) {
    printf("\t=%ld\t(", top());
    traceSay(top());
    printf(")");
  }
}

/*----------------------------------------------------------------------*/
static char *booleanValue(Abool bool) {
  if (bool) return "TRUE";
  else return "FALSE";
}

/*----------------------------------------------------------------------*/
static char *stringValue(Aword adress) {
  static char string[100];

  sprintf(string, "0x%lx (\"%s\")\t\t", adress, (char *)adress);
  return string;
}

/*----------------------------------------------------------------------*/
static void traceStringTopValue() {
  if (singleStepOption)
    printf("\t=%s", stringValue(top()));
}

/*----------------------------------------------------------------------*/
static char *printForm(SayForm form) {
  switch (form) {
  case SAY_SIMPLE: return "-";
  case SAY_INDEFINITE: return "An";
  case SAY_DEFINITE: return "The";
  case SAY_PRONOUN: return "It";
  }
  return "**Unknown!!***";
}


static Aaddr recursion[1000];
int recursions = 0;

/*----------------------------------------------------------------------*/
static void checkForRecursion(Aaddr adr) {
  int i;

  for (i = 0; i < recursions; i++)
    if (recursion[i] == adr)
      syserr("Interpreter recursion.");
  recursion[recursions++] = adr;
  if (recursions > 1000)
    syserr("Interpreter call stack too deep.");
}

/*======================================================================*/
void interpret(Aaddr adr)
{
  Aaddr oldpc;
  Aword i;

  /* Sanity checks: */
  if (adr == 0) syserr("Interpreting at address 0.");
  checkForRecursion(adr);
  
  if (singleStepOption)
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  oldpc = pc;
  pc = adr;
  while(TRUE) {
    if (pc > memTop)
      syserr("Interpreting outside program.");

    i = memory[pc++];

    switch (I_CLASS(i)) {
    case C_CONST:
      if (tracePushOption) printf("\n%4x: PUSH  \t%7ld\t\t\t\t\t", pc-1, I_OP(i));
      push(I_OP(i));
      if (tracePushOption && traceStackOption)
	dumpStack();
      break;
    case C_CURVAR:
      if (singleStepOption) printf("\n%4x: ", pc-1);
      switch (I_OP(i)) {
      case V_PARAM:
	if (singleStepOption) printf("PARAM \t%7ld\t\t\t=%ld\t", top(),
				     parameters[top()-1].instance);
	push(parameters[pop()-1].instance);
	break;
      case V_CURLOC:
	if (singleStepOption) printf("CURLOC \t\t\t\t=%d\t\t", current.location);
	push(current.location);
	break;
      case V_CURACT:
	if (singleStepOption) printf("CURACT \t\t\t\t=%d\t\t", current.actor);
	push(current.actor);
	break;
      case V_CURVRB:
	if (singleStepOption) printf("CURVRB \t\t\t\t=%d\t\t", current.verb);
	push(current.verb);
	break;
      case V_CURRENT_INSTANCE:
	if (singleStepOption) printf("CURINS \t\t\t\t=%d\t\t", current.instance);
	push(current.instance);
	break;
      case V_SCORE:
	if (singleStepOption) printf("CURSCORE \t\t\t\t=%d\t\t", current.score);
	push(current.score);
	break;
      case V_MAX_INSTANCE:
	if (singleStepOption) printf("MAXINSTANCE \t\t\t=%d\t\t", (int)header->instanceMax);
	push(header->instanceMax);
	break;
      default:
	syserr("Unknown CURVAR instruction.");
	break;
      }
      if (traceStackOption)
	dumpStack();
      break;

    case C_STMOP:
      if (singleStepOption) printf("\n%4x: ", pc-1);
      switch (I_OP(i)) {
      case I_POP: {
	Aword top;
	top = pop();
	if (singleStepOption)
	  printf("POP\t%7ld", top);
	break;
      }
      case I_LINE: {
	Aword file, line;
	line = pop();
	file = pop();
	if (singleStepOption)
	  printf("LINE\t%7ld, %7ld\t\t\t", file, line);
	if (line != 0) {
	  Bool atNext = stopAtNextLine && line != current.sourceLine;
	  Bool atBreakpoint =  breakpointIndex(line) != -1;
	  current.sourceLine = line;
	  current.sourceFile = file;
	  if (atNext || atBreakpoint) {
	    stopAtNextLine = FALSE;
	    debug(TRUE, line, file);
	  }
	}
	break;
      }
      case I_PRINT: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption) {
	  printf("PRINT \t%7ld, %7ld\t\"", fpos, len);
	  col = 41;		/* To break lines better! */
	}
	print(fpos, len);
	if (singleStepOption)
	  printf("\"\n\t\t\t\t\t\t\t");
	break;
      }
      case I_STYLE: {
	Aword style;
	style = pop();
	if (singleStepOption) {
	  printf("STYLE \t%7ld\t\t\"", style);
	}
	setStyle(style);
	break;
      }
      case I_SYSTEM: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption) {
	  printf("SYSTEM \t%7ld, %7ld\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	sys(fpos, len);
	if (singleStepOption)
	  printf("\"\t\t\t\t\t\t");
	break;
      }
      case I_GETSTR: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption)
	  printf("GETSTR\t%7ld, %7ld", fpos, len);
	push((Aword)getStringFromFile(fpos, len));
	traceStringTopValue();
	break;
      }
      case I_QUIT: {
	if (singleStepOption)
	  printf("QUIT\t\t\t\t\t\t");
	quitGame();
	break;
      }
      case I_LOOK: {
	if (singleStepOption)
	  printf("LOOK\t\t\t\t\t\t");
	look();
	break;
      }
      case I_SAVE: {
	if (singleStepOption)
	  printf("SAVE\t\t\t\t\t\t");
	save();
	break;
      }
      case I_RESTORE: {
	if (singleStepOption)
	  printf("RESTORE\t\t\t\t\t\t");
	restore();
	break;
      }
      case I_RESTART: {
	if (singleStepOption)
	  printf("RESTART\t\t\t\t\t\t");
	restartGame();
	break;
      }
      case I_LIST: {
	Aword cnt;
	cnt = pop();
	if (singleStepOption)
	  printf("LIST \t%7ld\t\t\t\t\t", cnt);
	list(cnt);
	break;
      }
      case I_EMPTY: {
	Aword cnt, whr;
	cnt = pop();
	whr = pop();
	if (singleStepOption)
	  printf("EMPTY \t%7ld, %7ld\t\t\t\t", cnt, whr);
	empty(cnt, whr);
	break;
      }
      case I_SCORE: {
	Aword sc;
	sc = pop();
	if (singleStepOption)
	  printf("SCORE \t%7ld\t\t=%ld\t\t\t", sc, scores[sc-1]);
	score(sc);
	break;
      }
      case I_VISITS: {
	Aword v;
	v = pop();
	if (singleStepOption)
	  printf("VISITS \t%7ld\t\t\t\t\t", v);
	visits(v);
	break;
      }
      case I_SCHEDULE: {
	Aword evt, whr, aft;
	evt = pop();
	whr = pop();
	aft = pop();
	if (singleStepOption)
	  printf("SCHEDULE \t%7ld, %7ld, %7ld\t\t\t\t", evt, whr, aft);
	schedule(evt, whr, aft);
	break;
      }
      case I_CANCEL: {
	Aword evt;
	evt = pop();
	if (singleStepOption)
	  printf("CANCEL \t%7ld\t\t\t\t", evt);
	cancelEvent(evt);
	break;
      }
      case I_MAKE: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (singleStepOption)
	  printf("MAKE \t%7ld, %7ld, %s\t\t\t", id, atr, booleanValue(val));
	setValue(id, atr, val);
	break;
      }
      case I_SET: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (singleStepOption) {
	  printf("SET \t%7ld, %7ld, %7ld\t\t\t\t", id, atr, val);
	}
	setValue(id, atr, val);
	break;
      }
      case I_SETSTR: {
	Aword id, atr, str;
	id = pop();
	atr = pop();
	str = pop();
	if (singleStepOption) {
	  printf("SETSTR\t%7ld, %7ld, %s\t\t\t\t", id, atr, stringValue(str));
	}
	setStringAttribute(id, atr, (char *)str);
	break;
      }
      case I_SETSET: {
	Aword id, atr, set;
	id = pop();
	atr = pop();
	set = pop();
	if (singleStepOption) {
	  printf("STRSET\t%7ld, %7ld, %7ld\t\t\t\t", id, atr, set);
	}
	setSetAttribute(id, atr, set);
	break;
      }
      case I_NEWSET: {
	Set *set = newSet(0);
	if (singleStepOption) {
	  printf("NEWSET\t\t\t\t\t\t\t");
	}
	push((Aword)set);
	traceIntegerTopValue();
	break;
      }
      case I_UNION: {
	Aword set1, set2;
	set2 = pop();
	set1 = top();
	if (singleStepOption) {
	  printf("UNION\t%7ld, %7ld\t\t\t\t", set1, set2);
	}
	setUnion((Set *)top(), (Set *)set2);
	traceIntegerTopValue();
	freeSet((Set *)set2);
	break;
      }
      case I_INCR: {
	Aint step;
	step = pop();
	if (singleStepOption) {
	  printf("INCR\t%7ld", step);
	}
	push(pop() + step);
	traceIntegerTopValue();
	break;
      }
      case I_DECR: {
	Aint step;
	step = pop();
	if (singleStepOption) {
	  printf("DECR\t%7ld\t\t\t\t\t", step);
	}
	push(pop() - step);
	traceIntegerTopValue();
	break;
      }
      case I_INCLUDE: {
	Aword member;
	member = pop();
	if (singleStepOption) {
	  printf("INCLUDE\t%7ld", member);
	}
	addToSet((Set *)top(), member);
	break;
      }
      case I_EXCLUDE: {
	Aword member;
	member = pop();
	if (singleStepOption) {
	  printf("EXCLUDE\t%7ld", member);
	}
	removeFromSet((Set *)top(), member);
	break;
      }
      case I_SETSIZE: {
	Set *set = (Set *)pop();
	if (singleStepOption)
	  printf("SETSIZE\t%7ld\t\t\t\t\t", (Aword)set);
	push(setSize(set));
	if (singleStepOption)
	  traceIntegerTopValue();
	break;
      }
      case I_SETMEMB: {
	Set *set = (Set *)pop();
	Aint index = pop();
	if (singleStepOption)
	  printf("SETMEMB\t%7ld, %7ld", (Aword)set, index);
	push(getSetMember(set, index));
	if (singleStepOption)
	  traceIntegerTopValue();
	break;
      }
      case I_CONTSIZE: {
	Abool directly = pop();
	Aint container = pop();
	if (singleStepOption)
	  printf("CONTSIZE\t%7ld, %7ld\t", container, directly);
	push(containerSize(container, directly));
	if (singleStepOption)
	  traceIntegerTopValue();
	break;
      }
      case I_CONTMEMB: {
	Abool directly = pop();
	Aint container = pop();
	Aint index = pop();
	if (singleStepOption)
	  printf("CONTMEMB\t%7ld, %7ld, %7ld", container, index, directly);
	push(getContainerMember(container, index, directly));
	if (singleStepOption)
	  traceIntegerTopValue();
	break;
      }
      case I_ATTRIBUTE: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (singleStepOption)
	  printf("ATTRIBUTE %7ld, %7ld\t", id, atr);
	push(attributeOf(id, atr));
	traceIntegerTopValue();
	break;
      }
      case I_ATTRSTR: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (singleStepOption)
	  printf("STRATTR \t%7ld, %7ld\t", id, atr);
	push(getStringAttribute(id, atr));
	traceStringTopValue();
	break;
      }
      case I_ATTRSET: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (singleStepOption)
	  printf("ATTRSET \t%7ld, %7ld", id, atr);
	push(getSetAttribute(id, atr));
	traceIntegerTopValue();
	break;
      }
      case I_SHOW: {
	Aword image, align;
	image = pop();
	align = pop();
	if (singleStepOption)
	  printf("SHOW \t%7ld, %7ld\t\t\t\t", image, align);
	showImage(image, align);
	break;
      }
      case I_PLAY: {
	Aword sound, align;
	sound = pop();
	align = pop();
	if (singleStepOption)
	  printf("PLAY \t%7ld\t\t\t\t", sound);
	playSound(sound);
	break;
      }
      case I_LOCATE: {
	Aword id, whr;
	id = pop();
	whr = pop();
	if (singleStepOption)
	  printf("LOCATE \t%7ld, %7ld\t\t\t\t", id, whr);
	locate(id, whr);
	break;
      }
      case I_WHERE: {
	Aword id;
	id = pop();
	if (singleStepOption)
	  printf("WHERE \t%7ld\t\t\t", id);
	push(where(id, TRUE));
	traceInstanceTopValue();
	break;
      }
      case I_LOCATION: {
	Aword id;
	id = pop();
	if (singleStepOption)
	  printf("LOCATION \t%7ld\t\t\t", id);
	push(location(id));
	traceInstanceTopValue();
	break;
      }
      case I_HERE: {
	Aword id;
	Abool directly;
	directly = pop();
	id = pop();
	if (singleStepOption)
	  printf("HERE \t%7ld, %s\t\t\t", id, booleanValue(directly));
	push(isHere(id, directly));
	traceBooleanTopValue();
	break;
      }
      case I_NEARBY: {
	Aword id;
	Abool directly;
	directly = pop();
	id = pop();
	if (singleStepOption)
	  printf("NEARBY \t%7ld, %s\t\t\t", id, booleanValue(directly));
	push(isNearby(id, directly));
	traceBooleanTopValue();
	break;
      }
      case I_NEAR: {
	Aword id, other;
	Abool directly;
	other = pop();
	directly = pop();
	id = pop();
	if (singleStepOption)
	  printf("NEAR \t%7ld, %7ld, %s\t\t\t", id, other, booleanValue(directly));
	push(isNear(id, other, directly));
	traceBooleanTopValue();
	break;
      }
      case I_AT: {
	Aword instance, other;
	Abool directly;
	other = pop();
	directly = pop();
	instance = pop();
	if (singleStepOption)
	  printf("AT \t%7ld, %7ld, %s", instance, other, booleanValue(directly));
	push(at(instance, other, directly));
	traceBooleanTopValue();
	break;
      }
      case I_IN: {
	Aword obj, cnt;
	Abool directly;
	directly = pop();
	cnt = pop();
	obj = pop();
	if (singleStepOption)
	  printf("IN \t%7ld, %7ld, %s", obj, cnt, booleanValue(directly));
	push(in(obj, cnt, directly));
	traceBooleanTopValue();
	break;
      }
      case I_INSET: {
	Aword element, set;
	set = pop();
	element = pop();
	if (singleStepOption)
	  printf("INSET \t%7ld, %7ld", element, set);
	push(inSet((Set*)set, element));
	traceBooleanTopValue();
	break;
      }
      case I_USE: {
	Aword act, scr;
	act = pop();
	scr = pop();
	if (singleStepOption)
	  printf("USE \t%7ld, %7ld\t\t\t\t", act, scr);
	use(act, scr);
	break;
      }
      case I_STOP: {
	Aword actor;
	actor = pop();
	if (singleStepOption)
	  printf("STOP \t%7ld\t\t\t\t\t", actor);
	stop(actor);
	break;
      }
      case I_DESCRIBE: {
	Aword id;
	id = pop();
	if (singleStepOption) {
	  printf("DESCRIBE \t%7ld\t\t\t", id);
	  col = 41;		/* To format it better! */
	}
	describe(id);
	if (singleStepOption)
	  printf("\n\t\t\t\t\t\t");
	break;
      }
      case I_SAY: {
	Aword form, id;
	form = pop();
	id = pop();
	if (singleStepOption)
	  printf("SAY\t%7s, %7ld\t\t\t", printForm(form), id);
	if (form == SAY_SIMPLE)
	  say(id);
	else
	  sayForm(id, form);
	if (singleStepOption)
	  printf("\t\t\t\t\t\t\t");
	break;
      }
      case I_SAYINT: {
	Aword val;
	val = pop();
	if (singleStepOption)
	  printf("SAYINT\t%7ld\t\t\t\"", val);
	sayInteger(val);
	if (singleStepOption)
	  printf("\"\n\t\t\t\t\t\t\t");
	break;
      }
      case I_SAYSTR: {
	Aword adr;
	adr = pop();
	if (singleStepOption)
	  printf("SAYSTR\t%7ld\t\ty\t", adr);
	sayString((char *)adr);
	if (singleStepOption)
	  printf("\n\t\t\t\t\t\t");
	break;
      }
      case I_IF: {
	Aword v;
	v = pop();
	if (singleStepOption) {
	  printf("IF \t");
	  if (v) printf("   TRUE"); else printf("  FALSE");
	  printf("\t\t\t\t\t");
	}
	interpretIf(v);
	break;
      }
      case I_ELSE: {
	if (singleStepOption)
	  printf("ELSE\t\t\t\t\t\t");
	interpretElse();
	break;
      }
      case I_ENDIF: {
	if (singleStepOption)
	  printf("ENDIF\t\t\t\t\t\t");
	break;
      }
      case I_AND: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("AND \t%s, %s", booleanValue(lh), booleanValue(rh));
	push(lh && rh);
	traceBooleanTopValue();
	break;
      }
      case I_OR: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("OR \t%s, %s", booleanValue(lh), booleanValue(rh));
	push(lh || rh);
	traceBooleanTopValue();
	break;
      }
      case I_NE: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("NE \t%7ld, %7ld", lh, rh);
	push(lh != rh);
	traceBooleanTopValue();
	break;
      }
      case I_EQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("EQ \t%7ld, %7ld", lh, rh);
	push(lh == rh);
	traceBooleanTopValue();
	break;
      }
      case I_STREQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("STREQ \t%7ld, %7ld", lh, rh);
	push(streq((char *)lh, (char *)rh));
	traceBooleanTopValue();
	break;
      }
      case I_STREXACT: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("STREXACT \t%7ld, %7ld", lh, rh);
	push(strcmp((char *)lh, (char *)rh) == 0);
	traceBooleanTopValue();
	free((void *)lh);
	free((void *)rh);
	break;
      }
      case I_LE: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("LE \t%7ld, %7ld", lh, rh);
	push(lh <= rh);
	traceBooleanTopValue();
	break;
      }
      case I_GE: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("GE \t%7ld, %7ld", lh, rh);
	push(lh >= rh);
	traceBooleanTopValue();
	break;
      }
      case I_LT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("LT \t%7ld, %7ld", lh, rh);
	push(lh < rh);
	traceBooleanTopValue();
	break;
      }
      case I_GT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("GT \t%7ld, %7ld", lh, rh);
	push(lh > rh);
	traceBooleanTopValue();
	break;
      }
      case I_PLUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("PLUS \t%7ld, %7ld", lh, rh);
	push(lh + rh);
	traceIntegerTopValue();
	break;
      }
      case I_MINUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("MINUS \t%7ld, %7ld", lh, rh);
	push(lh - rh);
	traceIntegerTopValue();
	break;
      }
      case I_MULT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("MULT \t%7ld, %7ld", lh, rh);
	push(lh * rh);
	traceIntegerTopValue();
	break;
      }
      case I_DIV: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("DIV \t%7ld, %7ld", lh, rh);
	push(lh / rh);
	traceIntegerTopValue();
	break;
      }
      case I_NOT: {
	Aword val;
	val = pop();
	if (singleStepOption)
	  printf("NOT \t%s\t\t", booleanValue(val));
	push(!val);
	traceBooleanTopValue();
	break;
      }
      case I_RND: {
	Aword  from, to;
	from = pop();
	to = pop();
	if (singleStepOption)
	  printf("RANDOM \t%7ld, %7ld", from, to);
	push(randomInteger(from, to));
	traceIntegerTopValue();
	break;
      }
      case I_BTW: {
	Aint low, high, val;
	high = pop();
	low = pop();
	val = pop();
	if (singleStepOption)
	  printf("BETWEEN \t%7ld, %7ld, %7ld", val, low, high);
	push(btw(val, low, high));
	traceIntegerTopValue();
	break;
      }

	/*------------------------------------------------------------*	\
	  String functions
	\*------------------------------------------------------------*/
      case I_CONCAT: {
	Aword s1, s2;
	s2 = pop();
	s1 = pop();
	if (singleStepOption)
	  printf("CONCAT \t%7ld, %7ld", s1, s2);
	push(concat(s1, s2));
	traceStringTopValue();
	break;
      }

      case I_CONTAINS: {
	Aword string, substring;
	substring = pop();
	string = pop();
	if (singleStepOption)
	  printf("CONTAINS \t%7ld, %7ld", string, substring);
	push(contains(string, substring));
	traceIntegerTopValue();
	break;
      }

      case I_STRIP: {
	Aword first, count, words, id, atr;
	id = pop();
	atr = pop();
	words = pop();
	count = pop();
	first = pop();
	if (singleStepOption)
	  printf("STRIP \t%7ld, %7ld, %7ld, %7ld, %7ld", first, count, words, id, atr);
	push(strip(first, count, words, id, atr));
	traceStringTopValue();
	break;
      }

      case I_AGRSTART: {
	Aint index = pop();
	Aint value = pop();
	Aint limit = pop();
	if (singleStepOption)
	  printf("AGRSTART\t\t\t\t\t\t");
	push(limit);
	push(value);
	push(index);
	if (index > limit) {
	  push(index);
	  goToAGREND();
	}
	break;
      }

      case I_AGRCHECK:
	if (singleStepOption)
	  printf("AGRCHECK\t\t\t\t\t\t");
	if (!pop())		/* This filter matched? */
	  goToAGREND();		/* If not, skip rest of filter and the aggregate itself */
	break;

      case I_MIN: {
	Aint attribute = pop();
	Aint loopValue = pop();
	Aint loopIndex = pop();
	Aint min;

	min = pop();
	if (singleStepOption)
	  printf("MIN \t%7ld\t\t", attribute);
	if (min > attributeOf(loopIndex, attribute))
	  push(attributeOf(loopIndex, attribute));
	else
	  push(min);
	traceIntegerTopValue();
	push(loopIndex);
	push(loopValue);
	break;
      }
      case I_MAX: {
	Aint attribute = pop();
	Aint loopValue = pop();
	Aint loopIndex = pop();
	Aint max;

	max = pop();
	if (singleStepOption)
	  printf("MAX \t%7ld\t\t", attribute);
	if (max < attributeOf(loopIndex, attribute))
	  push(attributeOf(loopIndex, attribute)); 
	else
	  push(max);
	traceIntegerTopValue();
	push(loopIndex);
	push(loopValue);
	break;
      }
      case I_SUM: {
	Aint attribute = pop();
	Aint loopValue = pop();
	Aint loopIndex = pop();
	Aint sum;

	sum = pop();
	if (singleStepOption)
	  printf("SUM \t%7ld\t\t", attribute);
	push(sum + attributeOf(loopIndex, attribute));
	traceIntegerTopValue();
	push(loopIndex);
	push(loopValue);
	break;
      }
      case I_COUNT: {
	Aint loopValue = pop();
	Aint loopIndex = pop();
	if (singleStepOption)
	  printf("COUNT\t\t\t");
	push(pop() + 1);
	traceIntegerTopValue();
	push(loopIndex);
	push(loopValue);
	break;
      }
      case I_AGREND: {
	Aint loopIndex;
	Aint value;
	Aint limit;

	pop();			/* Ignore loop value */
	loopIndex = pop();
	value = pop();
	limit = top();
	if (singleStepOption)
	  printf("AGREND\t%7ld\t\t\t=%ld\t\t", loopIndex, limit);
	if (loopIndex < limit) {
	  push(value);
	  push(loopIndex+1);
	  goToAGRSTART();
	} else {
	  pop();		/* remove the limit */
	  push(value);		/* and push the resulting aggregate value */
	}
	break;
      }

      case I_DUP:
	if (singleStepOption)
	  printf("DUP\t\t\t\t\t\t");
	stackDup();
	break;

      case I_DEPEND:
	if (singleStepOption)
	  printf("DEPEND\t\t\t\t\t\t");
	break;

      case I_DEPCASE:
	if (singleStepOption)
	  printf("DEPCASE\t\t\t\t\t\t");
	depcase();
	break;

      case I_DEPEXEC: {
	Aword v;
	v = pop();
	if (singleStepOption) {
	  printf("DEPEXEC \t\t\t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	  printf("\t\t\t\t\t");
	}
	depexec(v);
	break;
      }
	
      case I_DEPELSE:
	if (singleStepOption)
	  printf("DEPELSE\t\t\t\t\t\t");
	depcase();
	break;

      case I_ENDDEP:
	if (singleStepOption)
	  printf("ENDDEP\t\t\t\t\t\t");
	pop();
	break;

      case I_ISA: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("ISA \t%7ld, %7ld\t", lh, rh);
	push(isA(lh, rh));
	traceBooleanTopValue();
	break;
      }

      case I_FRAME: {
	Aint size;
	size = pop();
	if (singleStepOption)
	  printf("FRAME \t%7ld\t\t\t\t\t", size);
	newFrame(size);
	break;
      }

      case I_GETLOCAL: {
	Aint framesBelow, variableNumber;
	framesBelow = pop();
	variableNumber = pop();
	if (singleStepOption)
	  printf("GETLOCAL \t%7ld, %7ld", framesBelow, variableNumber);
	push(getLocal(framesBelow, variableNumber));
	traceIntegerTopValue();
	break;
      }

      case I_SETLOCAL: {
	Aint framesBelow, variableNumber, value;
	framesBelow = pop();
	variableNumber = pop();
	value = pop();
	if (singleStepOption)
	  printf("SETLOCAL \t%7ld, %7ld, %7ld\t\t\t\t", framesBelow, variableNumber, value);
	setLocal(framesBelow, variableNumber, value);
	break;
      }

      case I_ENDFRAME: {
	if (singleStepOption)
	  printf("ENDFRAME\t\t\t\t\t\t");
	endFrame();
	break;
      }

      case I_EACH: {
	Aint startValue = pop();
	if (singleStepOption)
	  printf("EACH \t%7ld\t\t\t\t\t", startValue);
	setLocal(0, 1, startValue);
	break;
      }

      case I_NEXTEACH: {
	if (singleStepOption)
	  printf("NEXTEACH\t\t\t\t\t\t");
	nextEach();
	break;
      }

      case I_ENDEACH: {
	if (singleStepOption)
	  printf("ENDEACH\t\t\t\t\t\t");
	endEach();
	break;
      }

      case I_RETURN:
	if (singleStepOption)
	  printf("RETURN\n--------------------------------------------------\n");
	pc = oldpc;
	goto exitInterpreter;

      default:
	syserr("Unknown STMOP instruction.");
	break;
      }
      if (fail) {
	pc = oldpc;
	goto exitInterpreter;
      }
      if (traceStackOption)
	dumpStack();
      break;

    default:
      syserr("Unknown instruction class.");
      break;
    }
  }
 exitInterpreter:
  recursions--;
}

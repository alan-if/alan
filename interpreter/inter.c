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

#include "inter.h"

#ifdef GLK
#include "glkio.h"
#endif

/* PRIVATE DATA */

static int pc;


/*----------------------------------------------------------------------*/
static void interpretIf(Aword v)
{
  int lev = 1;
  Aword i;

  if (!v) {
    /* Skip to next ELSE or ENDIF on same level */
    if (singleStepOption) printf("\n    : ");
    while (TRUE) {
      i = memory[pc++];
      if (I_CLASS(i) == (Aword)C_STMOP)
	switch (I_OP(i)) {
	case I_ELSE:
	  if (lev == 1) {
	    if (singleStepOption)
	      printf("\n%4x: ELSE", pc);
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
	      printf("\n%4x: ENDIF", pc);
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

  if (singleStepOption) printf("\n    : ");
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
static void interpretEach(Aint local)
{
  Aint counter = getLocal(0, local);
  int level = 1;
  int i;

  counter++;
  if (counter > header->instanceMax) {
    while (TRUE) {
      /* Skip past ENDEACHFOR on the same level */
      i = memory[pc++];
      if (I_CLASS(i) == (Aword)C_STMOP)
	switch (I_OP(i)) {
	case I_ENDEACH:
	  level--;
	  if (level == 0) {
	    if (singleStepOption)
	      printf("\n%4x: ENDEACH", pc);
	    return;
	  }
	  break;
	case I_EACH:
	  level++;
	  break;
	}
    }
  } else
    setLocal(0, local, counter);
}


/*----------------------------------------------------------------------*/
static void nextEach()
{
  int level = 1;
  int i;

  pc--;
  if (singleStepOption) printf("\n    : ");
  while (TRUE) {
    /* Skip back past FOR on the same level */
    i = memory[--pc];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_ENDEACH:
	level++;
	break;
      case I_EACH:
	level--;
	if (level == 0) {
	  pc--;
	  return;
	}
	break;
      }
  }
}  


/*----------------------------------------------------------------------*/
static void endEach()
{
  nextEach();
}

/*----------------------------------------------------------------------*/
static void dup(void)
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
static void goToENDAGR(void)
{
  while (memory[pc] != INSTRUCTION(I_ENDAGR))
    pc++;
}


/*----------------------------------------------------------------------*/
static void goToAGRSTART(void)
{
  while (memory[pc-1] != INSTRUCTION(I_AGRSTART))
    pc--;
}

static void traceBooleanTopValue() {
  if (singleStepOption) {
    if (top()) printf("\t=TRUE\t");
    else printf("\t=FALSE\t");
  }
}

static void traceIntegerTopValue() {
  if (singleStepOption)
    printf("\t=%ld\t", top());
}


static char *printForm(SayForm form) {
  switch (form) {
  case SAY_SIMPLE: return "";
  case SAY_INDEFINITE: return "An";
  case SAY_DEFINITE: return "The";
  }
  return "**Unknown!!***";
}

/*======================================================================*/
void interpret(Aaddr adr)
{
  Aaddr oldpc;
  Aword i;

  if (adr == 0) syserr("Interpreting at address 0.");
  
  if (singleStepOption) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  oldpc = pc;
  pc = adr;
  while(TRUE) {
    if (pc > memTop)
      syserr("Interpreting outside program.");

    i = memory[pc++];
    
    switch (I_CLASS(i)) {
    case C_CONST:
      if (tracePushOption) printf("\n%4x: PUSH  \t%5ld\t\t\t", pc-1, I_OP(i));
      push(I_OP(i));
      if (tracePushOption && traceStackOption)
	dumpStack();
      break;
    case C_CURVAR:
      if (singleStepOption) printf("\n%4x: ", pc-1);
      switch (I_OP(i)) {
      case V_PARAM:
	if (singleStepOption) printf("PARAM \t%5ld\t\t=%ld\t", top(), params[top()-1].code);
	push(params[pop()-1].code);
	break;
      case V_CURLOC:
	if (singleStepOption) printf("CURLOC \t\t\t=%d\t", current.location);
	push(current.location);
	break;
      case V_CURACT:
	if (singleStepOption) printf("CURACT \t\t\t=%d\t", current.actor);
	push(current.actor);
	break;
      case V_CURVRB:
	if (singleStepOption) printf("CURVRB \t\t\t=%d\t", current.verb);
	push(current.verb);
	break;
      case V_CURRENT_INSTANCE:
	if (singleStepOption) printf("CURINS \t\t\t=%d\t", current.instance);
	push(current.instance);
	break;
      case V_SCORE:
	if (singleStepOption) printf("CURSCORE \t\t\t=%d\t", current.score);
	push(current.score);
	break;
      default:
	syserr("Unknown CURVAR instruction.");
	break;
      }
      break;

    case C_STMOP:
      if (singleStepOption) printf("\n%4x: ", pc-1);
      switch (I_OP(i)) {
      case I_PRINT: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption) {
	  printf("PRINT \t%5ld, %5ld\t\t\"", fpos, len);
	  col = 41;		/* To break lines better! */
	}
	print(fpos, len);
	if (singleStepOption)
	  printf("\"\n\t\t\t\t\t");
	break;
      }
      case I_SYSTEM: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption) {
	  printf("SYSTEM \t%5ld, %5ld\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	sys(fpos, len);
	if (singleStepOption)
	  printf("\"\t\t\t\t");
	break;
      }
      case I_GETSTR: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (singleStepOption)
	  printf("GETSTR\t%5ld, %5ld", fpos, len);
	getstr(fpos, len);
	traceIntegerTopValue();
	break;
      }
      case I_QUIT: {
	if (singleStepOption)
	  printf("QUIT\t\t\t\t");
	quitGame();
	break;
      }
      case I_LOOK: {
	if (singleStepOption)
	  printf("LOOK\t\t\t\t");
	look();
	break;
      }
      case I_SAVE: {
	if (singleStepOption)
	  printf("SAVE\t\t\t\t");
	saveGame();
	break;
      }
      case I_RESTORE: {
	if (singleStepOption)
	  printf("RESTORE\t\t\t\t");
	restoreGame();
	break;
      }
      case I_RESTART: {
	if (singleStepOption)
	  printf("RESTART\t\t\t\t");
	restartGame();
	break;
      }
      case I_LIST: {
	Aword cnt;
	cnt = pop();
	if (singleStepOption)
	  printf("LIST \t%5ld\t\t\t", cnt);
	list(cnt);
	break;
      }
      case I_EMPTY: {
	Aword cnt, whr;
	cnt = pop();
	whr = pop();
	if (singleStepOption)
	  printf("EMPTY \t%5ld, %5ld\t\t", cnt, whr);
	empty(cnt, whr);
	break;
      }
      case I_SCORE: {
	Aword sc;
	sc = pop();
	if (singleStepOption)
	  printf("SCORE \t%5ld\t\t=%ld\t", sc, scores[sc-1]);
	score(sc);
	break;
      }
      case I_VISITS: {
	Aword v;
	v = pop();
	if (singleStepOption)
	  printf("VISITS \t%5ld\t\t\t", v);
	visits(v);
	break;
      }
      case I_SCHEDULE: {
	Aword evt, whr, aft;
	evt = pop();
	whr = pop();
	aft = pop();
	if (singleStepOption)
	  printf("SCHEDULE \t%5ld, %5ld, %5ld\t\t", evt, whr, aft);
	schedule(evt, whr, aft);
	break;
      }
      case I_CANCEL: {
	Aword evt;
	evt = pop();
	if (singleStepOption)
	  printf("CANCEL \t%5ld\t\t", evt);
	cancelEvent(evt);
	break;
      }
      case I_MAKE: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (singleStepOption) {
	  printf("MAKE \t%5ld, %5ld, ", id, atr);
	  if (val) printf("TRUE"); else printf("FALSE");
	  printf("\t");
	}
	make(id, atr, val);
	break;
      }
      case I_SET: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (singleStepOption) {
	  printf("SET \t%5ld, %5ld, %5ld\t\t", id, atr, val);
	}
	set(id, atr, val);
	break;
      }
      case I_STRSET: {
	Aword id, atr, str;
	id = pop();
	atr = pop();
	str = pop();
	if (singleStepOption) {
	  printf("STRSET\t%5ld, %5ld, %5ld\t\t", id, atr, str);
	}
	setStringAttribute(id, atr, str);
	break;
      }
      case I_INCR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (singleStepOption) {
	  printf("INCR\t%5ld, %5ld, %5ld", id, atr, step);
	}
	increase(id, atr, step);
	break;
      }
      case I_DECR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (singleStepOption) {
	  printf("DECR\t%5ld, %5ld, %5ld\t\t", id, atr, step);
	}
	decrease(id, atr, step);
	break;
      }
      case I_ATTRIBUTE: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (singleStepOption)
	  printf("ATTRIBUTE %5ld, %5ld", id, atr);
	push(attributeOf(id, atr));
	traceIntegerTopValue();
	break;
      }
      case I_STRATTR: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (singleStepOption)
	  printf("STRATTR \t%5ld, %5ld", id, atr);
	push(strattr(id, atr));
	traceIntegerTopValue();
	break;
      }
      case I_SHOW: {
	Aword image, align;
	image = pop();
	align = pop();
	if (singleStepOption)
	  printf("SHOW \t%5ld, %5ld\t\t", image, align);
	showImage(image, align);
	break;
      }
      case I_LOCATE: {
	Aword id, whr;
	id = pop();
	whr = pop();
	if (singleStepOption)
	  printf("LOCATE \t%5ld, %5ld\t\t", id, whr);
	locate(id, whr);
	break;
      }
      case I_WHERE: {
	Aword id;
	id = pop();
	if (singleStepOption)
	  printf("WHERE \t%5ld\t", id);
	push(where(id));
	traceIntegerTopValue();
	break;
      }
      case I_HERE: {
	Aword id;
	id = pop();
	if (singleStepOption)
	  printf("HERE \t%5ld", id);
	push(isHere(id));
	if (singleStepOption) {
	  if (top()) printf("\t=TRUE\t");
	  else printf("\t=FALSE\t");
	}
	break;
      }
      case I_NEAR: {
	Aword id;
	id = pop();
	if (singleStepOption)
	  printf("NEAR \t%5ld", id);
	push(isNear(id));
	if (singleStepOption) {
	  if (top()) printf("\t=TRUE\t");
	  else printf("\t=FALSE\t");
	}
	break;
      }
      case I_USE: {
	Aword act, scr;
	act = pop();
	scr = pop();
	if (singleStepOption)
	  printf("USE \t%5ld, %5ld\t\t", act, scr);
	use(act, scr);
	break;
      }
      case I_STOP: {
	Aword actor;
	actor = pop();
	if (singleStepOption)
	  printf("STOP \t%5ld\t\t\t", actor);
	stop(actor);
	break;
      }
      case I_IN: {
	Aword obj, cnt;
	obj = pop();
	cnt = pop();
	if (singleStepOption)
	  printf("IN \t%5ld, %5ld ", obj, cnt);
	push(in(obj, cnt));
	if (singleStepOption) {
	  if (top()) printf("\t=TRUE\t");
	  else printf("\t=FALSE\t");
	}
	break;
      }
      case I_DESCRIBE: {
	Aword id;
	id = pop();
	if (singleStepOption) {
	  printf("DESCRIBE \t%5ld\t", id);
	  col = 41;		/* To format it better! */
	}
	describe(id);
	if (singleStepOption)
	  printf("\n\t\t\t\t");
	break;
      }
      case I_SAY: {
	Aword form, id;
	form = pop();
	id = pop();
	if (singleStepOption)
	  printf("SAY %s\t%5ld\t\t\t\"", printForm(form), id);
	if (form == SAY_SIMPLE)
	  say(id);
	else
	  sayForm(id, form);
	if (singleStepOption)
	  printf("\"\n\t\t\t\t");
	break;
      }
      case I_SAYINT: {
	Aword val;
	val = pop();
	if (singleStepOption)
	  printf("SAYINT\t%5ld\t\t\t\"", val);
	sayint(val);
	if (singleStepOption)
	  printf("\"\n\t\t\t\t\t");
	break;
      }
      case I_SAYSTR: {
	Aword adr;
	adr = pop();
	if (singleStepOption)
	  printf("SAYSTR\t%5ld\t\t", adr);
	saystr((char *)adr);
	if (singleStepOption)
	  printf("\n\t\t\t\t");
	break;
      }
      case I_IF: {
	Aword v;
	v = pop();
	if (singleStepOption) {
	  printf("IF \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	  printf("\t\t");
	}
	interpretIf(v);
	break;
      }
      case I_ELSE: {
	if (singleStepOption)
	  printf("ELSE\t\t\t\t");
	interpretElse();
	break;
      }
      case I_ENDIF: {
	if (singleStepOption)
	  printf("ENDIF\t\t\t\t");
	break;
      }
      case I_AND: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption) {
	  printf("AND \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh && rh);
	traceBooleanTopValue();
	break;
      }
      case I_OR: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption) {
	  printf("OR \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh || rh);
	traceBooleanTopValue();
	break;
      }
      case I_NE: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("NE \t%5ld, %5ld", lh, rh);
	push(lh != rh);
	traceBooleanTopValue();
	break;
      }
      case I_EQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("EQ \t%5ld, %5ld", lh, rh);
	push(lh == rh);
	traceBooleanTopValue();
	break;
      }
      case I_STREQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("STREQ \t%5ld, %5ld", lh, rh);
	push(streq((char *)lh, (char *)rh));
	traceBooleanTopValue();
	break;
      }
      case I_STREXACT: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("STREXACT \t%5ld, %5ld", lh, rh);
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
	  printf("LE \t%5ld, %5ld", lh, rh);
	push(lh <= rh);
	traceBooleanTopValue();
	break;
      }
      case I_GE: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("GE \t%5ld, %5ld", lh, rh);
	push(lh >= rh);
	traceBooleanTopValue();
	break;
      }
      case I_LT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("LT \t%5ld, %5ld", lh, rh);
	push(lh < rh);
	traceBooleanTopValue();
	break;
      }
      case I_GT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("GT \t%5ld, %5ld", lh, rh);
	push(lh > rh);
	traceBooleanTopValue();
	break;
      }
      case I_PLUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("PLUS \t%5ld, %5ld", lh, rh);
	push(lh + rh);
	traceIntegerTopValue();
	break;
      }
      case I_MINUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("MINUS \t%5ld, %5ld", lh, rh);
	push(lh - rh);
	traceIntegerTopValue();
	break;
      }
      case I_MULT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("MULT \t%5ld, %5ld", lh, rh);
	push(lh * rh);
	traceIntegerTopValue();
	break;
      }
      case I_DIV: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("DIV \t%5ld, %5ld", lh, rh);
	push(lh / rh);
	traceIntegerTopValue();
	break;
      }
      case I_NOT: {
	Aword val;
	val = pop();
	if (singleStepOption) {
	  printf("NOT \t");
	  if (val) printf("TRUE"); else printf("FALSE");
	}
	push(!val);
	traceBooleanTopValue();
	break;
      }
      case I_RND: {
	Aword  from, to;
	from = pop();
	to = pop();
	if (singleStepOption)
	  printf("RANDOM \t%5ld, %5ld", from, to);
	push(rnd(from, to));
	traceIntegerTopValue();
	break;
      }
      case I_BTW: {
	Aint low, high, val;
	high = pop();
	low = pop();
	val = pop();
	if (singleStepOption)
	  printf("BETWEEN \t%5ld, %5ld, %5ld", val, low, high);
	push(btw(val, low, high));
	traceIntegerTopValue();
	break;
      }
      case I_CONTAINS: {
	Aword string, substring;
	substring = pop();
	string = pop();
	if (singleStepOption)
	  printf("CONTAINS \t%5ld, %5ld", string, substring);
	push(contains(string, substring));
	traceIntegerTopValue();
	break;
      }

      case I_AGRSTART:
	if (singleStepOption)
	  printf("AGRSTART\t\t\t\t");
	push(1);		/* Instance index */
	dup();			/* Twice! */
	break;
      case I_AGRCHECK:
	if (singleStepOption)
	  printf("AGRCHECK\t\t\t\t");
	if (!pop())		/* This filter matched? */
	  goToENDAGR();		/* If not, skip rest of filter and the aggregate itself */
	else
	  dup();
	break;
      case I_MIN: {
	Aint attribute = pop();
	Aint instanceIndex = pop();
	Aint min;

	pop();
	min = pop();
	if (singleStepOption)
	  printf("MIN \t%5ld\t", attribute);
	if (min > attributeOf(instanceIndex, attribute))
	  push(attributeOf(instanceIndex, attribute));
	else
	  push(min);
	traceIntegerTopValue();
	push(instanceIndex);
	break;
      }
      case I_MAX: {
	Aint attribute = pop();
	Aint instanceIndex = pop();
	Aint max;

	pop();
	max = pop();
	if (singleStepOption)
	  printf("MAX \t%5ld\t", attribute);
	if (max < attributeOf(instanceIndex, attribute))
	  push(attributeOf(instanceIndex, attribute)); 
	else
	  push(max);
	traceIntegerTopValue();
	push(instanceIndex);
	break;
      }
      case I_SUM: {
	Aint attribute = pop();
	Aint instanceIndex = pop();
	Aint sum;

	pop();
	sum = pop();
	if (singleStepOption)
	  printf("SUM \t%5ld\t", attribute);
	push(sum + attributeOf(instanceIndex, attribute));
	traceIntegerTopValue();
	push(instanceIndex);
	break;
      }
      case I_COUNT: {
	int instanceIndex = pop();
	pop();
	if (singleStepOption)
	  printf("COUNT\t\t");
	push(pop() + 1);
	traceIntegerTopValue();
	push(instanceIndex);
	break;
      }
      case I_ENDAGR: {
	int instance = pop();

	if (singleStepOption)
	  printf("ENDAGR\t%5d\t\t=%ld\t", instance, top());
	if (instance < MAXENTITY) {
	  push(instance+1);
	  dup();
	  goToAGRSTART();
	}
	break;
      }

      case I_DUP:
	if (singleStepOption)
	  printf("DUP\t\t\t\t");
	dup();
	break;

      case I_DEPEND:
	if (singleStepOption)
	  printf("DEPEND\t\t\t\t");
	break;

      case I_DEPCASE:
	if (singleStepOption)
	  printf("DEPCASE\t\t\t\t");
	depcase();
	break;

      case I_DEPEXEC: {
	Aword v;
	v = pop();
	if (singleStepOption) {
	  printf("DEPEXEC \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	  printf("\t\t\t");
	}
	depexec(v);
	break;
      }
	
      case I_DEPELSE:
	if (singleStepOption)
	  printf("DEPELSE\t\t\t\t");
	depcase();
	break;

      case I_ENDDEP:
	if (singleStepOption)
	  printf("ENDDEP\t\t\t\t");
	pop();
	break;

      case I_ISA: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (singleStepOption)
	  printf("ISA \t%5ld, %5ld", lh, rh);
	push(isA(lh, rh));
	traceBooleanTopValue();
	break;
      }

      case I_FRAME: {
	Aint size;
	size = pop();
	if (singleStepOption)
	  printf("FRAME \t%5ld\t\t\t", size);
	newFrame(size);
	break;
      }

      case I_GETLOCAL: {
	Aint framesBelow, variableNumber;
	framesBelow = pop();
	variableNumber = pop();
	if (singleStepOption)
	  printf("GETLOCAL \t%5ld, %5ld", framesBelow, variableNumber);
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
	  printf("SETLOCAL \t%5ld, %5ld, %5ld\t\t", framesBelow, variableNumber, value);
	setLocal(framesBelow, variableNumber, value);
	break;
      }

      case I_ENDFRAME: {
	if (singleStepOption)
	  printf("ENDFRAME\t\t\t\t");
	endFrame();
	break;
      }

      case I_EACH: {
	Aint local = pop();
	if (singleStepOption)
	  printf("EACH \t%5ld\t\t\t", local);
	interpretEach(local);
	break;
      }

      case I_NEXTEACH: {
	if (singleStepOption)
	  printf("NEXTEACH\t\t\t\t");
	nextEach();
	break;
      }

      case I_ENDEACH: {
	if (singleStepOption)
	  printf("ENDEACH\t\t\t\t");
	endEach();
	break;
      }

      case I_RETURN:
	if (singleStepOption)
	  printf("RETURN\n--------------------------------------------------\n");
	pc = oldpc;
	return;

      default:
	syserr("Unknown STMOP instruction.");
	break;
      }
      if (fail) {
	pc = oldpc;
	return;
      }
      if (traceStackOption)
	dumpStack();
      break;

    default:
      syserr("Unknown instruction class.");
      break;
    }
  }
}

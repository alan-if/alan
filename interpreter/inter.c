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
#include "sysdep.h"

#include "inter.h"

#ifdef GLK
#include "glkio.h"
#endif

/* PRIVATE DATA */

static int pc;


#ifdef _PROTOTYPES_
static void interpretIf(
     Aword v
)
#else
static void interpretIf(v)
     Aword v;
#endif
{
  int lev = 1;
  Aword i;

  if (!v) {
    /* Skip to next ELSE or ENDIF on same level */
    while (TRUE) {
      i = memory[pc++];
      if (I_CLASS(i) == (Aword)C_STMOP)
	switch (I_OP(i)) {
	case I_ELSE:
	  if (lev == 1) return;
	  break;
	case I_IF:
	  lev++;
	  break;
	case I_ENDIF:
	  lev--;
	  if (lev == 0) return;
	  break;
	}
    }
  }
}


#ifdef _PROTOTYPES_
static void interpretElse(void)
#else
static void interpretElse()
#endif
{
  int lev = 1;
  Aword i;

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
	  if (level == 0)
	    return;
	  break;
	case I_EACH:
	  level++;
	  break;
	}
    }
  } else
    setLocal(0, local, counter);
}


static void endEach()
{
  int level = 1;
  int i;

  pc--;
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


#ifdef _PROTOTYPES_
static void dup(void)
#else
static void dup()
#endif
{
  push(top());
}



#ifdef _PROTOTYPES_
static void depexec(
     Aword v
)
#else
static void depexec(v)
     Aword v;
#endif
{
  int lev = 1;
  Aword i;

  if (!v)
    /* The expression was not true, skip to next CASE on the same
       level which could be a DEPCASE or DEPELSE */
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
	    return;
	  } else
	    lev--;
	  break;
	case I_DEPCASE:
	case I_DEPELSE:
	  if (lev == 1) return;
	  break;
	}
    }
}


#ifdef _PROTOTYPES_
static void depcase(void)
#else
static void depcase()
#endif
{
  int lev = 1;
  Aword i;

  /* 
     We have just executed a DEPCASE/DEPELSE clause as a result of a
     DEPCASE catching so skip to end of DEPENDING block (next DEPEND
     on same level) then return.
  */

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


#ifdef _PROTOTYPES_
void interpret(Aaddr adr)
#else
void interpret(adr)
     Aaddr adr;
#endif
{
  Aaddr oldpc;
  Aword i;

  if (adr == 0) syserr("Interpreting at address 0.");

  
  if (stepFlag) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  oldpc = pc;
  pc = adr;
  while(TRUE) {
    if (stepFlag) printf("\n%4x: ", pc);
    if (pc > memTop)
      syserr("Interpreting outside program.");

    i = memory[pc++];
    
    switch (I_CLASS(i)) {
    case C_CONST:
      if (stepFlag) printf("PUSH  \t%5ld", I_OP(i));
      push(I_OP(i));
      break;
    case C_CURVAR:
      switch (I_OP(i)) {
      case V_PARAM:
	if (stepFlag) printf("PARAM \t%5ld\t\t(%ld)", top(), params[top()-1].code);
	push(params[pop()-1].code);
	break;
      case V_CURLOC:
	if (stepFlag) printf("CURLOC \t\t\t(%d)", current.location);
	push(current.location);
	break;
      case V_CURACT:
	if (stepFlag) printf("CURACT \t\t\t(%d)", current.actor);
	push(current.actor);
	break;
      case V_CURVRB:
	if (stepFlag) printf("CURVRB \t\t\t(%d)", current.verb);
	push(current.verb);
	break;
      case V_CURRENT_INSTANCE:
	if (stepFlag) printf("CURINS \t\t\t(%d)", current.instance);
	push(current.instance);
	break;
      case V_SCORE:
	if (stepFlag) printf("CURSCORE \t\t\t(%d)", current.score);
	push(current.score);
	break;
      default:
	syserr("Unknown CURVAR instruction.");
	break;
      }
      break;
      
    case C_STMOP: 
      switch (I_OP(i)) {
      case I_PRINT: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (stepFlag) {
	  printf("PRINT \t%5ld, %5ld\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	print(fpos, len);
	if (stepFlag)
	  printf("\"");
	break;
      }
      case I_SYSTEM: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (stepFlag) {
	  printf("SYSTEM \t%5ld, %5ld\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	sys(fpos, len);
	break;
      }
      case I_GETSTR: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (stepFlag)
	  printf("GETSTR\t%5ld, %5ld", fpos, len);
	getstr(fpos, len);
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_QUIT: {
	if (stepFlag)
	  printf("QUIT");
	quit();
	break;
      }
      case I_LOOK: {
	if (stepFlag)
	  printf("LOOK");
	look();
	break;
      }
      case I_SAVE: {
	if (stepFlag)
	  printf("SAVE");
	save();
	break;
      }
      case I_RESTORE: {
	if (stepFlag)
	  printf("RESTORE");
	restore();
	break;
      }
      case I_RESTART: {
	if (stepFlag)
	  printf("RESTART");
	restart();
	break;
      }
      case I_LIST: {
	Aword cnt;
	cnt = pop();
	if (stepFlag)
	  printf("LIST \t%5ld", cnt);
	list(cnt);
	break;
      }
      case I_EMPTY: {
	Aword cnt, whr;
	cnt = pop();
	whr = pop();
	if (stepFlag)
	  printf("EMPTY \t%5ld, %5ld", cnt, whr);
	empty(cnt, whr);
	break;
      }
      case I_SCORE: {
	Aword sc;
	sc = pop();
	if (stepFlag)
	  printf("SCORE \t%5ld\t\t(%ld)", sc, scores[sc-1]);
	score(sc);
	break;
      }
      case I_VISITS: {
	Aword v;
	v = pop();
	if (stepFlag)
	  printf("VISITS \t%5ld", v);
	visits(v);
	break;
      }
      case I_SCHEDULE: {
	Aword evt, whr, aft;
	evt = pop();
	whr = pop();
	aft = pop();
	if (stepFlag)
	  printf("SCHEDULE \t%5ld, %5ld, %5ld", evt, whr, aft);
	schedule(evt, whr, aft);
	break;
      }
      case I_CANCEL: {
	Aword evt;
	evt = pop();
	if (stepFlag)
	  printf("CANCEL \t%5ld", evt);
	cancl(evt);
	break;
      }
      case I_MAKE: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (stepFlag) {
	  printf("MAKE \t%5ld, %5ld, ", id, atr);
	  if (val) printf("TRUE"); else printf("FALSE");
	}
	make(id, atr, val);
	break;
      }
      case I_SET: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (stepFlag) {
	  printf("SET \t%5ld, %5ld, %5ld", id, atr, val);
	}
	set(id, atr, val);
	break;
      }
      case I_STRSET: {
	Aword id, atr, str;
	id = pop();
	atr = pop();
	str = pop();
	if (stepFlag) {
	  printf("STRSET\t%5ld, %5ld, %5ld", id, atr, str);
	}
	setstr(id, atr, str);
	break;
      }
      case I_INCR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (stepFlag) {
	  printf("INCR\t%5ld, %5ld, %5ld", id, atr, step);
	}
	incr(id, atr, step);
	break;
      }
      case I_DECR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (stepFlag) {
	  printf("DECR\t%5ld, %5ld, %5ld", id, atr, step);
	}
	decr(id, atr, step);
	break;
      }
      case I_ATTRIBUTE: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stepFlag)
	  printf("ATTRIBUTE %5ld, %5ld", id, atr);
	push(attribute(id, atr));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_STRATTR: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stepFlag)
	  printf("STRATTR \t%5ld, %5ld", id, atr);
	push(strattr(id, atr));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_LOCATE: {
	Aword id, whr;
	id = pop();
	whr = pop();
	if (stepFlag)
	  printf("LOCATE \t%5ld, %5ld", id, whr);
	locate(id, whr);
	break;
      }
      case I_WHERE: {
	Aword id;
	id = pop();
	if (stepFlag)
	  printf("WHERE \t%5ld", id);
	push(where(id));
	if (stepFlag)
	  printf("\t\t(%ld)", top());
	break;
      }
      case I_HERE: {
	Aword id;
	id = pop();
	if (stepFlag)
	  printf("HERE \t%5ld", id);
	push(isHere(id));
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_NEAR: {
	Aword id;
	id = pop();
	if (stepFlag)
	  printf("NEAR \t%5ld", id);
	push(isNear(id));
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_USE: {
	Aword act, scr;
	act = pop();
	scr = pop();
	if (stepFlag)
	  printf("USE \t%5ld, %5ld", act, scr);
	use(act, scr);
	break;
      }
      case I_IN: {
	Aword obj, cnt;
	obj = pop();
	cnt = pop();
	if (stepFlag)
	  printf("IN \t%5ld, %5ld ", obj, cnt);
	push(in(obj, cnt));
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_DESCRIBE: {
	Aword id;
	id = pop();
	if (stepFlag) {
	  printf("DESCRIBE \t%5ld\t", id);
	  col = 34;		/* To format it better! */
	}
	describe(id);
	break;
      }
      case I_SAY: {
	Aword id;
	id = pop();
	if (stepFlag)
	  printf("SAY \t%5ld\t\t\"", id);
	say(id);
	if (stepFlag)
	  printf("\"");
	break;
      }
      case I_SAYINT: {
	Aword val;
	val = pop();
	if (stepFlag)
	  printf("SAYINT\t%5ld\t\t\"", val);
	sayint(val);
	if (stepFlag)
	  printf("\"");
	break;
      }
      case I_SAYSTR: {
	Aword adr;
	adr = pop();
	if (stepFlag)
	  printf("SAYSTR\t%5ld\t\t\"", adr);
	saystr((char *)adr);
	if (stepFlag)
	  printf("\"");
	break;
      }
      case I_IF: {
	Aword v;
	v = pop();
	if (stepFlag) {
	  printf("IF \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	}
	interpretIf(v);
	break;
      }
      case I_ELSE: {
	if (stepFlag)
	  printf("ELSE");
	interpretElse();
	break;
      }
      case I_ENDIF: {
	if (stepFlag)
	  printf("ENDIF");
	break;
      }
      case I_AND: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag) {
	  printf("AND \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh && rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_OR: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag) {
	  printf("OR \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh || rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_NE: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("NE \t%5ld, %5ld", lh, rh);
	push(lh != rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_EQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("EQ \t%5ld, %5ld", lh, rh);
	push(lh == rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_STREQ: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("STREQ \t%5ld, %5ld", lh, rh);
	push(streq((char *)lh, (char *)rh));
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_STREXACT: {
	Aword lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("STREXACT \t%5ld, %5ld", lh, rh);
	push(strcmp((char *)lh, (char *)rh) == 0);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	free((void *)lh);
	free((void *)rh);
	break;
      }
      case I_LE: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("LE \t%5ld, %5ld", lh, rh);
	push(lh <= rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_GE: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("GE \t%5ld, %5ld", lh, rh);
	push(lh >= rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_LT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("LT \t%5ld, %5ld", lh, rh);
	push(lh < rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_GT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("GT \t%5ld, %5ld", lh, rh);
	push(lh > rh);
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }
      case I_PLUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("PLUS \t%5ld, %5ld", lh, rh);
	push(lh + rh);
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_MINUS: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("MINUS \t%5ld, %5ld", lh, rh);
	push(lh - rh);
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_MULT: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("MULT \t%5ld, %5ld", lh, rh);
	push(lh * rh);
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_DIV: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("DIV \t%5ld, %5ld", lh, rh);
	push(lh / rh);
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_NOT: {
	Aword val;
	val = pop();
	if (stepFlag) {
	  printf("NOT \t");
	  if (val) printf("TRUE"); else printf("FALSE");
	}
	push(!val);
	if (stepFlag) {
	  if (top()) printf("\t\t(TRUE)");
	  else printf("\t\t(FALSE)");
	}
	break;
      }
      case I_MAX: {
	Aword atr, whr;
	atr = pop();
	whr = pop();
	if (stepFlag)
	  printf("MAX \t%5ld, %5ld", atr, whr);
	push(agrmax(atr, whr));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_SUM: {
	Aword atr, whr;
	atr = pop();
	whr = pop();
	if (stepFlag)
	  printf("SUM \t%5ld, %5ld", atr, whr);
	push(agrsum(atr, whr));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_COUNT: {
	Aword whr;
	whr = pop();
	if (stepFlag)
	  printf("COUNT \t%5ld", whr);
	push(agrcount(whr));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_RND: {
	Aword  from, to;
	from = pop();
	to = pop();
	if (stepFlag)
	  printf("RANDOM \t%5ld, %5ld", from, to);
	push(rnd(from, to));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_BTW: {
	Aint low, high, val;
	high = pop();
	low = pop();
	val = pop();
	if (stepFlag)
	  printf("BETWEEN \t%5ld, %5ld, %5ld", val, low, high);
	push(btw(val, low, high));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }
      case I_CONTAINS: {
	Aword string, substring;
	substring = pop();
	string = pop();
	if (stepFlag)
	  printf("CONTAINS \t%5ld, %5ld", string, substring);
	push(contains(string, substring));
	if (stepFlag)
	  printf("\t(%ld)", top());
	break;
      }

      case I_DUP:
	if (stepFlag)
	  printf("DUP");
	dup();
	break;

      case I_DEPEND:
	if (stepFlag)
	  printf("DEPEND");
	break;

      case I_DEPCASE:
	if (stepFlag)
	  printf("DEPCASE");
	depcase();
	break;

      case I_DEPEXEC: {
	Aword v;
	v = pop();
	if (stepFlag) {
	  printf("DEPEXEC \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	}
	depexec(v);
	break;
      }
	
      case I_DEPELSE:
	if (stepFlag)
	  printf("DEPELSE");
	depcase();
	break;

      case I_ENDDEP:
	if (stepFlag)
	  printf("ENDDEP");
	pop();
	break;

      case I_ISA: {
	Aint lh, rh;
	rh = pop();
	lh = pop();
	if (stepFlag)
	  printf("ISA \t%5ld, %5ld", lh, rh);
	push(isA(lh, rh));
	if (stepFlag) {
	  if (top()) printf("\t(TRUE)");
	  else printf("\t(FALSE)");
	}
	break;
      }

      case I_BLOCK: {
	Aint size;
	size = pop();
	if (stepFlag)
	  printf("BLOCK \t%5ld", size);
	newBlock(size);
	break;
      }

      case I_GETLOCAL: {
	Aint blocksBelow, variableNumber;
	blocksBelow = pop();
	variableNumber = pop();
	if (stepFlag)
	  printf("GETLOCAL \t%5ld, %5ld", blocksBelow, variableNumber);
	push(getLocal(blocksBelow, variableNumber));
	if (stepFlag)
	  printf("\t(%5ld)", top());
	break;
      }

      case I_SETLOCAL: {
	Aint blocksBelow, variableNumber, value;
	blocksBelow = pop();
	variableNumber = pop();
	value = pop();
	if (stepFlag)
	  printf("SETLOCAL \t%5ld, %5ld, %5ld", blocksBelow, variableNumber, value);
	setLocal(blocksBelow, variableNumber, value);
	break;
      }

      case I_ENDBLOCK: {
	if (stepFlag)
	  printf("ENDBLOCK");
	endBlock();
	break;
      }

      case I_EACH: {
	Aint local = pop();
	if (stepFlag)
	  printf("FOR \t%5ld", local);
	interpretEach(local);
	break;
      }

      case I_ENDEACH: {
	if (stepFlag)
	  printf("ENDEACH");
	endEach();
	break;
      }

      case I_RETURN:
	if (stepFlag)
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
      break;

    default:
      syserr("Unknown instruction class.");
      break;
    }
  }
}

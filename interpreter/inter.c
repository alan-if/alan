/*----------------------------------------------------------------------*\

  inter.c

  Interpreter unit for Alan interpreter Arun

\*----------------------------------------------------------------------*/


#include <stdio.h>

#include "types.h"
#include "arun.h"
#include "parse.h"
#include "exe.h"
#include "stack.h"
#include "sysdep.h"



/* PRIVATE DATA */

static int pc;


#ifdef _PROTOTYPES_
static void skip_else(void)
#else
static void skip_else()
#endif
{
  int lev = 1;
  Aword i;

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


#ifdef _PROTOTYPES_
static void skip_endif(void)
#else
static void skip_endif()
#endif
{
  int lev = 1;
  Aword i;

  while (TRUE) {
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


#ifdef _PROTOTYPES_
static void if_(
     Aword v
)
#else
static void if_(v)
     Aword v;
#endif
{
  if (!v)
    skip_else();
}


#ifdef _PROTOTYPES_
static void else_(void)
#else
static void else_()
#endif
{
  skip_endif();
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
  
  if (stpflg) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  oldpc = pc;
  pc = adr;
  while(TRUE) {
    if (stpflg) printf("\n%4x: ", pc);
    if (pc > memTop)
      syserr("Interpreting outside program.");

    i = memory[pc++];
    
    switch (I_CLASS(i)) {
    case C_CONST:
      if (stpflg) printf("PUSH  \t%5d", I_OP(i));
      push(I_OP(i));
      break;
    case C_CURVAR:
      switch (I_OP(i)) {
      case V_PARAM:
	if (stpflg) printf("PARAM \t%5d\t\t(%d)", top(), params[top()-1].code);
	push(params[pop()-1].code);
	break;
      case V_CURLOC:
	if (stpflg) printf("CURLOC \t\t\t(%d)", cur.loc);
	push(cur.loc);
	break;
      case V_CURACT:
	if (stpflg) printf("CURACT \t\t\t(%d)", cur.act);
	push(cur.act);
	break;
      case V_CURVRB:
	if (stpflg) printf("CURVRB \t\t\t(%d)", cur.vrb);
	push(cur.vrb);
	break;
      case V_SCORE:
	if (stpflg) printf("CURSCORE \t\t\t(%d)", cur.score);
	push(cur.score);
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
	if (stpflg) {
	  printf("PRINT \t%5d, %5d\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	print(fpos, len);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_GETSTR: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (stpflg)
	  printf("GETSTR\t%5d, %5d", fpos, len);
	getstr(fpos, len);
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_QUIT: {
	if (stpflg)
	  printf("QUIT");
	quit();
	break;
      }
      case I_LOOK: {
	if (stpflg)
	  printf("LOOK");
	look();
	break;
      }
      case I_SAVE: {
	if (stpflg)
	  printf("SAVE");
	save();
	break;
      }
      case I_RESTORE: {
	if (stpflg)
	  printf("RESTORE");
	restore();
	break;
      }
      case I_LIST: {
	Aword cnt;
	cnt = pop();
	if (stpflg)
	  printf("LIST \t%5d", cnt);
	list(cnt);
	break;
      }
      case I_EMPTY: {
	Aword cnt, whr;
	cnt = pop();
	whr = pop();
	if (stpflg)
	  printf("EMPTY \t%5d, %5d", cnt, whr);
	empty(cnt, whr);
	break;
      }
      case I_SCORE: {
	Aword sc;
	sc = pop();
	if (stpflg)
	  printf("SCORE \t%5d\t\t(%d)", sc, scores[sc-1]);
	score(sc);
	break;
      }
      case I_SCHEDULE: {
	Aword evt, whr, aft;
	evt = pop();
	whr = pop();
	aft = pop();
	if (stpflg)
	  printf("SCHEDULE \t%5d, %5d, %5d", evt, whr, aft);
	schedule(evt, whr, aft);
	break;
      }
      case I_CANCEL: {
	Aword evt;
	evt = pop();
	if (stpflg)
	  printf("CANCEL \t%5d", evt);
	cancel(evt);
	break;
      }
      case I_MAKE: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (stpflg) {
	  printf("MAKE \t%5d, %5d, ", id, atr);
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
	if (stpflg) {
	  printf("SET \t%5d, %5d, %5d", id, atr, val);
	}
	set(id, atr, val);
	break;
      }
      case I_STRSET: {
	Aword id, atr, str;
	id = pop();
	atr = pop();
	str = pop();
	if (stpflg) {
	  printf("STRSET\t%5d, %5d, %5d", id, atr, str);
	}
	setstr(id, atr, str);
	break;
      }
      case I_INCR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (stpflg) {
	  printf("INCR\t%5d, %5d, %5d", id, atr, step);
	}
	incr(id, atr, step);
	break;
      }
      case I_DECR: {
	Aword id, atr, step;
	id = pop();
	atr = pop();
	step = pop();
	if (stpflg) {
	  printf("DECR\t%5d, %5d, %5d", id, atr, step);
	}
	decr(id, atr, step);
	break;
      }
      case I_ATTRIBUTE: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stpflg)
	  printf("ATTRIBUTE %5d, %5d", id, atr);
	push(attribute(id, atr));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_STRATTR: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stpflg)
	  printf("STRATTR \t%5d, %5d", id, atr);
	push(strattr(id, atr));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_LOCATE: {
	Aword id, whr;
	id = pop();
	whr = pop();
	if (stpflg)
	  printf("LOCATE \t%5d, %5d", id, whr);
	locate(id, whr);
	break;
      }
      case I_WHERE: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("WHERE \t%5d", id);
	push(where(id));
	if (stpflg)
	  printf("\t\t(%d)", top());
	break;
      }
      case I_HERE: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("HERE \t%5d", id);
	push(isHere(id));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_NEAR: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("NEAR \t%5d", id);
	push(isNear(id));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_USE: {
	Aword act, scr;
	act = pop();
	scr = pop();
	if (stpflg)
	  printf("USE \t%5d, %5d", act, scr);
	use(act, scr);
	break;
      }
      case I_IN: {
	Aword obj, cnt;
	obj = pop();
	cnt = pop();
	if (stpflg)
	  printf("IN \t%5d, %5d ", obj, cnt);
	push(in(obj, cnt));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_DESCRIBE: {
	Aword id;
	id = pop();
	if (stpflg) {
	  printf("DESCRIBE \t%5d\t\"", id);
	  col = 34;		/* To format it better! */
	}
	describe(id);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SAY: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("SAY \t%5d\t\t\"", id);
	say(id);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SAYINT: {
	Aword val;
	val = pop();
	if (stpflg)
	  printf("SAYINT\t%5d\t\t\"", val);
	sayint(val);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SAYSTR: {
	Aword adr;
	adr = pop();
	if (stpflg)
	  printf("SAYSTR\t%5d\t\t\"", adr);
	saystr((char *)adr);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_IF: {
	Aword v;
	v = pop();
	if (stpflg) {
	  printf("IF \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	}
	if_(v);
	break;
      }
      case I_ELSE: {
	if (stpflg)
	  printf("ELSE");
	else_();
	break;
      }
      case I_ENDIF: {
	if (stpflg)
	  printf("ENDIF");
	break;
      }
      case I_AND: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg) {
	  printf("AND \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh && rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_OR: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg) {
	  printf("OR \t");
	  if (lh) printf("TRUE, "); else printf("FALSE, ");
	  if (rh) printf("TRUE"); else printf("FALSE");
        }
	push(lh || rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_NE: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("NE \t%5d, %5d", lh, rh);
	push(lh != rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_EQ: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("EQ \t%5d, %5d", lh, rh);
	push(lh == rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_STREQ: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("STREQ \t%5d, %5d", lh, rh);
	push(streq((char *)lh, (char *)rh));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_STREXACT: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("STREXACT \t%5d, %5d", lh, rh);
	push(strcmp((char *)lh, (char *)rh) == 0);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	free((void *)lh);
	free((void *)rh);
	break;
      }
      case I_LE: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("LE \t%5d, %5d", lh, rh);
	push(lh <= rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_GE: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("GE \t%5d, %5d", lh, rh);
	push(lh >= rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_LT: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("LT \t%5d, %5d", lh, rh);
	push(lh < rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_GT: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("GT \t%5d, %5d", lh, rh);
	push(lh > rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_PLUS: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("PLUS \t%5d, %5d", lh, rh);
	push(lh + rh);
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_MINUS: {
	Aword lh, rh;
	lh = pop();
	rh = pop();
	if (stpflg)
	  printf("MINUS \t%5d, %5d", lh, rh);
	push(lh - rh);
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_NOT: {
	Aword val;
	val = pop();
	if (stpflg) {
	  printf("NOT \t");
	  if (val) printf("TRUE"); else printf("FALSE");
	}
	push(!val);
	if (stpflg)
	  if (top()) printf("\t\t(TRUE)"); else printf("\t\t(FALSE)");
	break;
      }
      case I_MAX: {
	Aword atr, whr;
	atr = pop();
	whr = pop();
	if (stpflg)
	  printf("MAX \t%5d, %5d", atr, whr);
	push(agrmax(atr, whr));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_SUM: {
	Aword atr, whr;
	atr = pop();
	whr = pop();
	if (stpflg)
	  printf("SUM \t%5d, %5d", atr, whr);
	push(agrsum(atr, whr));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_COUNT: {
	Aword whr;
	whr = pop();
	if (stpflg)
	  printf("COUNT \t%5d", whr);
	push(agrcount(whr));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_RND: {
	Aword  from, to;
	from = pop();
	to = pop();
	if (stpflg)
	  printf("RANDOM \t%5d, %5d", from, to);
	push(rnd(from, to));
	if (stpflg)
	  printf("\t(%d)", top());
	break;
      }
      case I_RETURN:
	if (stpflg)
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

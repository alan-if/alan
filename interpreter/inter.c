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
static void if_(
     Aword v
)
#else
static void if_(v)
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
static void else_(void)
#else
static void else_()
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


#ifdef _PROTOTYPES_
static void depstart(void)
#else
static void depstart(void)
#endif
{
  /* A DEPSTART was executed so skip across the redundant DEPCASE to
     start at the first expression */
  pc++;
}


#ifdef _PROTOTYPES_
static void swap(void)
#else
static void swap()
#endif
{
  Aword v1 = pop();
  Aword v2 = pop();

  push(v1);
  push(v2);
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
	case I_DEPSTART:
	  lev++;
	  break;
	case I_DEPEND:
	  if (lev == 1) return;
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

  /* Skip to end of DEPENDING block (next DEPEND on same level) because
     we have just executed a DEPCASE/DEPELSE statement as a result of a DEPCASE
     catching */

  while (TRUE) {
    i = memory[pc++];
    if (I_CLASS(i) == (Aword)C_STMOP)
      switch (I_OP(i)) {
      case I_DEPSTART:
	lev++;
	break;
      case I_DEPEND:
	lev--;
	if (lev == 0) return;
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
      if (stpflg) printf("PUSH  \t%5ld", I_OP(i));
      push(I_OP(i));
      break;
    case C_CURVAR:
      switch (I_OP(i)) {
      case V_PARAM:
	if (stpflg) printf("PARAM \t%5ld\t\t(%ld)", top(), params[top()-1].code);
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
	  printf("PRINT \t%5ld, %5ld\t\"", fpos, len);
	  col = 34;		/* To format it better! */
	}
	print(fpos, len);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SYSTEM: {
	Aword fpos, len;
	fpos = pop();
	len = pop();
	if (stpflg) {
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
	if (stpflg)
	  printf("GETSTR\t%5ld, %5ld", fpos, len);
	getstr(fpos, len);
	if (stpflg)
	  printf("\t(%ld)", top());
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
      case I_RESTART: {
	if (stpflg)
	  printf("RESTART");
	restart();
	break;
      }
      case I_LIST: {
	Aword cnt;
	cnt = pop();
	if (stpflg)
	  printf("LIST \t%5ld", cnt);
	list(cnt);
	break;
      }
      case I_EMPTY: {
	Aword cnt, whr;
	cnt = pop();
	whr = pop();
	if (stpflg)
	  printf("EMPTY \t%5ld, %5ld", cnt, whr);
	empty(cnt, whr);
	break;
      }
      case I_SCORE: {
	Aword sc;
	sc = pop();
	if (stpflg)
	  printf("SCORE \t%5ld\t\t(%ld)", sc, scores[sc-1]);
	score(sc);
	break;
      }
      case I_VISITS: {
	Aword v;
	v = pop();
	if (stpflg)
	  printf("VISITS \t%5ld", v);
	visits(v);
	break;
      }
      case I_SCHEDULE: {
	Aword evt, whr, aft;
	evt = pop();
	whr = pop();
	aft = pop();
	if (stpflg)
	  printf("SCHEDULE \t%5ld, %5ld, %5ld", evt, whr, aft);
	schedule(evt, whr, aft);
	break;
      }
      case I_CANCEL: {
	Aword evt;
	evt = pop();
	if (stpflg)
	  printf("CANCEL \t%5ld", evt);
	cancl(evt);
	break;
      }
      case I_MAKE: {
	Aword id, atr, val;
	id = pop();
	atr = pop();
	val = pop();
	if (stpflg) {
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
	if (stpflg) {
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
	if (stpflg) {
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
	if (stpflg) {
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
	if (stpflg) {
	  printf("DECR\t%5ld, %5ld, %5ld", id, atr, step);
	}
	decr(id, atr, step);
	break;
      }
      case I_ATTRIBUTE: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stpflg)
	  printf("ATTRIBUTE %5ld, %5ld", id, atr);
	push(attribute(id, atr));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_STRATTR: {
	Aword id, atr;
	id = pop();
	atr = pop();
	if (stpflg)
	  printf("STRATTR \t%5ld, %5ld", id, atr);
	push(strattr(id, atr));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_LOCATE: {
	Aword id, whr;
	id = pop();
	whr = pop();
	if (stpflg)
	  printf("LOCATE \t%5ld, %5ld", id, whr);
	locate(id, whr);
	break;
      }
      case I_WHERE: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("WHERE \t%5ld", id);
	push(where(id));
	if (stpflg)
	  printf("\t\t(%ld)", top());
	break;
      }
      case I_HERE: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("HERE \t%5ld", id);
	push(isHere(id));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_NEAR: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("NEAR \t%5ld", id);
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
	  printf("USE \t%5ld, %5ld", act, scr);
	use(act, scr);
	break;
      }
      case I_IN: {
	Aword obj, cnt;
	obj = pop();
	cnt = pop();
	if (stpflg)
	  printf("IN \t%5ld, %5ld ", obj, cnt);
	push(in(obj, cnt));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_DESCRIBE: {
	Aword id;
	id = pop();
	if (stpflg) {
	  printf("DESCRIBE \t%5ld\t", id);
	  col = 34;		/* To format it better! */
	}
	describe(id);
	break;
      }
      case I_SAY: {
	Aword id;
	id = pop();
	if (stpflg)
	  printf("SAY \t%5ld\t\t\"", id);
	say(id);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SAYINT: {
	Aword val;
	val = pop();
	if (stpflg)
	  printf("SAYINT\t%5ld\t\t\"", val);
	sayint(val);
	if (stpflg)
	  printf("\"");
	break;
      }
      case I_SAYSTR: {
	Aword adr;
	adr = pop();
	if (stpflg)
	  printf("SAYSTR\t%5ld\t\t\"", adr);
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
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
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
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
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
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("NE \t%5ld, %5ld", lh, rh);
	push(lh != rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_EQ: {
	Aword lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("EQ \t%5ld, %5ld", lh, rh);
	push(lh == rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_STREQ: {
	Aword lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("STREQ \t%5ld, %5ld", lh, rh);
	push(streq((char *)lh, (char *)rh));
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_STREXACT: {
	Aword lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("STREXACT \t%5ld, %5ld", lh, rh);
	push(strcmp((char *)lh, (char *)rh) == 0);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	free((void *)lh);
	free((void *)rh);
	break;
      }
      case I_LE: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("LE \t%5ld, %5ld", lh, rh);
	push(lh <= rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_GE: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("GE \t%5ld, %5ld", lh, rh);
	push(lh >= rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_LT: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("LT \t%5ld, %5ld", lh, rh);
	push((signed int)lh < (signed int)rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_GT: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("GT \t%5ld, %5ld", lh, rh);
	push(lh > rh);
	if (stpflg)
	  if (top()) printf("\t(TRUE)"); else printf("\t(FALSE)");
	break;
      }
      case I_PLUS: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("PLUS \t%5ld, %5ld", lh, rh);
	push(lh + rh);
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_MINUS: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("MINUS \t%5ld, %5ld", lh, rh);
	push(lh - rh);
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_MULT: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("MULT \t%5ld, %5ld", lh, rh);
	push(lh * rh);
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_DIV: {
	Aint lh, rh;
	if (header->vers[0] == 2 && header->vers[1] == 7) /* Check for 2.7 version */
	  swap();
	rh = pop();
	lh = pop();
	if (stpflg)
	  printf("DIV \t%5ld, %5ld", lh, rh);
	push(lh / rh);
	if (stpflg)
	  printf("\t(%ld)", top());
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
	  printf("MAX \t%5ld, %5ld", atr, whr);
	push(agrmax(atr, whr));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_SUM: {
	Aword atr, whr;
	atr = pop();
	whr = pop();
	if (stpflg)
	  printf("SUM \t%5ld, %5ld", atr, whr);
	push(agrsum(atr, whr));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_COUNT: {
	Aword whr;
	whr = pop();
	if (stpflg)
	  printf("COUNT \t%5ld", whr);
	push(agrcount(whr));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_RND: {
	Aword  from, to;
	from = pop();
	to = pop();
	if (stpflg)
	  printf("RANDOM \t%5ld, %5ld", from, to);
	push(rnd(from, to));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_BTW: {
	Aint low, high, val;
	high = pop();
	low = pop();
	val = pop();
	if (stpflg)
	  printf("BETWEEN \t%5ld, %5ld, %5ld", val, low, high);
	push(btw(val, low, high));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }
      case I_CONTAINS: {
	Aword string, substring;
	substring = pop();
	string = pop();
	if (stpflg)
	  printf("CONTAINS \t%5ld, %5ld", string, substring);
	push(contains(string, substring));
	if (stpflg)
	  printf("\t(%ld)", top());
	break;
      }

      case I_DEPSTART:
	if (stpflg)
	  printf("DEPSTART");
	depstart();
	break;

      case I_DEPCASE:
	if (stpflg)
	  printf("DEPCASE");
	depcase();
	break;

      case I_DEPEXEC: {
	Aword v;
	v = pop();
	if (stpflg) {
	  printf("DEPEXEC \t");
	  if (v) printf(" TRUE"); else printf("FALSE");
	}
	depexec(v);
	break;
      }
	
      case I_DEPELSE:
	if (stpflg)
	  printf("DEPELSE");
	depcase();
	break;

      case I_DEPEND:
	if (stpflg)
	  printf("DEPEND");
	break;

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

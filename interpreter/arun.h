#ifndef _ARUN_H_
#define _ARUN_H_
/*----------------------------------------------------------------------*\

  ARUN.H

  Header file for main unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

#include "types.h"


/* TYPES */

/* Error message numbers */
typedef enum msgnum {
  HUH,
  WHAT,
  WHAT_ALL,
  WHAT_IT,
  WHAT_THEM,
  MULTIPLE,
  WANT,
  NOUN,
  AFTER_BUT,
  BUT_ALL,
  NOT_MUCH,
  WHICH_ONE,
  NO_SUCH,
  NO_WAY,
  CANT0,
  CANT,
  NOTHING,
  SEEOBJ1,
  SEEOBJ2,
  SEEOBJ3,
  SEEOBJ4,
  SEEACT,
  CONTAINS1,
  CONTAINS2,
  CONTAINS3,
  CONTAINS4,
  CONTAINS5,
  EMPTY1,
  EMPTY2,
  SCOREMSG1,
  SCOREMSG2,
  KNOW_WORD,
  MORE,
  AGAIN,
  SAVEFAILED,
  SAVEMISSING,
  SAVEVERS,
  NOMSG
} MsgNum;


/* DATA */

#define MEMORYSIZE 1000L

extern int memTop;		/* Top of memory */


/* The Amachine memory */
extern Aword *memory;
extern AcdHdr *header;

/* Amachine variables */
extern CurVars cur;

/* Amachine data structures */
extern WrdElem *dict;		/* Dictionary pointer */
extern ActElem *acts;		/* Actor table pointer */
extern LocElem *locs;		/* Location table pointer */
extern VrbElem *vrbs;		/* Verb table pointer */
extern StxElem *stxs;		/* Syntax table pointer */
extern ObjElem *objs;		/* Object table pointer */
extern CntElem *cnts;		/* Container table pointer */
extern RulElem *ruls;		/* Rule table pointer */
extern EvtElem *evts;		/* Event table pointer */
extern Aword *scores;		/* Score table pointer */
extern Aword *freq;		/* Cumulated frequencies */

extern int dictsize;		/* Number of entries in dictionary */

/* The text and message file */
extern FILE *txtfil;
extern FILE *logfil;

/* Save file name */
extern char savfnm[256];


/* Screen formatting info */
extern int col, lin;
extern int paglen, pagwidth;


extern Boolean trcflg;
extern Boolean dbgflg;
extern Boolean stpflg;
extern Boolean logflg;
extern Boolean fail;
extern Boolean anyOutput;
extern Boolean needsp;

#define endOfTable(x) eot((Aword *) x)

#ifdef _PROTOTYPES_
extern void *allocate(int len);
extern void syserr(char *msg);
extern void error(enum msgnum msg);

extern void output(char string[]);
extern void prmsg(enum msgnum msg);
extern void para(void);
extern void newline(void);

extern Boolean checklim(Aword cnt, Aword obj);
extern Boolean possible(void);
extern Boolean exitto(int to, int from);
extern void action(ParamElem *plst);
extern void go(int dir);

extern Boolean eot(Aword *adr);
extern Boolean isObj(Aword x);
extern Boolean isCnt(Aword x);
extern Boolean isAct(Aword x);
extern Boolean isLoc(Aword x);
extern Boolean isLit(Aword x);
extern Boolean isNum(Aword x);
extern Boolean isStr(Aword x);

#else
extern void *allocate();
extern void error();
extern void output();
extern void prmsg();
extern void print();
extern void para();
extern Boolean checklim();
extern Boolean possible();
extern Boolean eot();
extern Boolean isObj();
extern Boolean isCnt();
extern Boolean isAct();
extern Boolean isLoc();
extern Boolean isLit();
#endif

#endif

#ifndef _MAIN_H_
#define _MAIN_H_
/*----------------------------------------------------------------------*\

  MAIN.H

  Header file for main unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"
#include <setjmp.h>


/* DATA */

#define MEMORYSIZE 1000L

extern int memTop;		/* Top of memory */

extern int conjWord;		/* First conjunction in dictionary */

/* The Amachine memory */
extern Aword *memory;
extern AcdHdr *header;

/* Amachine variables */
extern CurVars current;

/* Amachine data structures */
extern AdminEntry *admin;	/* Administrative data about instances */
extern WrdEntry *dict;		/* Dictionary pointer */
extern ClassEntry *class;	/* Class table pointer */
extern InstanceEntry *instance; /* Instance table pointer */
extern ContainerEntry *container; /* Container table pointer */

extern VerbEntry *vrbs;		/* Verb table pointer */
extern StxEntry *stxs;		/* Syntax table pointer */
extern RulEntry *ruls;		/* Rule table pointer */
extern EventEntry *events;	/* Event table pointer */
extern MsgEntry *msgs;		/* Message table pointer */
extern Aword *scores;		/* Score table pointer */
extern Aword *freq;		/* Cumulated frequencies */

extern int dictsize;		/* Number of entries in dictionary */

/* The text and message file */
extern FILE *txtfil;
extern FILE *logfil;

/* File names */
extern char *adventureName;

/* Screen formatting info */
extern int col, lin;
extern int pageLength, pageWidth;

/* Long jump buffer for restart */
extern jmp_buf restart_label;

extern Boolean verbose;
extern Boolean ignoreErrorOption;
extern Boolean debugOption;
extern Boolean traceOption;
extern Boolean tracePushOption;
extern Boolean traceStackOption;
extern Boolean singleStepOption;
extern Boolean logOption;
extern Boolean statusLineOption;
extern Boolean regressionTestOption;
extern Boolean fail;
extern Boolean anyOutput;
extern Boolean needsp;

#define endOfTable(x) eot((Aword *) x)


#ifdef _PROTOTYPES_
extern void *allocate(unsigned long len);
extern void terminate(int code);
extern void usage(void);
extern void error(MsgKind msg);
extern void syserr(char *msg);
extern void statusline(void);
extern void output(char string[]);
extern void prmsg(MsgKind msg);
extern void para(void);
extern void newline(void);

extern Boolean eot(Aword *adr);
extern Boolean isObj(Aword x);
extern Boolean isCnt(Aword x);
extern Boolean isAct(Aword x);
extern Boolean isLoc(Aword x);
extern Boolean isLit(Aword x);
extern Boolean isNum(Aword x);
extern Boolean isStr(Aword x);

/* Run the game! */
extern void run(void);

#else
extern void *allocate();
extern void terminate();
extern void syserr();
extern void usage();
extern void error();
extern void output();
extern void statusline();
extern void prmsg();
extern void print();
extern void para();
extern void newline();
extern Boolean eot();
extern Boolean isObj();
extern Boolean isCnt();
extern Boolean isAct();
extern Boolean isLoc();
extern Boolean isLit();
extern void run();
#endif

#endif

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

/* Event queue */
extern int eventQueueSize;
extern EventQueueEntry *eventQueue;
extern int eventQueueTop;		/* Event queue top pointer */

/* Amachine variables */
extern CurVars current;

/* Amachine data structures - Dynamic */
extern InstanceEntry *instance; /* Instance table pointer */
extern AdminEntry *admin;	/* Administrative data about instances */
extern AttributeEntry *attributes; /* Dynamic attribute values */
extern Aword *scores;		/* Score table pointer */

/* Amachine data structures - Static */
extern WrdEntry *dict;		/* Dictionary pointer */
extern ClassEntry *class;	/* Class table pointer */
extern ContainerEntry *container; /* Container table pointer */

extern VerbEntry *vrbs;		/* Verb table pointer */
extern ParseEntry *stxs;		/* Syntax table pointer */
extern RulEntry *ruls;		/* Rule table pointer */
extern EventEntry *events;	/* Event table pointer */
extern MsgEntry *msgs;		/* Message table pointer */
extern Aword *freq;		/* Cumulated frequencies */

extern int dictsize;		/* Number of entries in dictionary */

/* The text and message file */
extern FILE *textFile;
extern FILE *logFile;

/* File names */
extern char *adventureName;

/* Screen formatting info */
extern int col, lin;
extern int pageLength, pageWidth;

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restart_label;
extern jmp_buf error_label;

extern Boolean verbose;
extern Boolean ignoreErrorOption;
extern Boolean debugOption;
extern Boolean sectionTraceOption;
extern Boolean tracePushOption;
extern Boolean traceStackOption;
extern Boolean singleStepOption;
extern Boolean transcriptOption;
extern Boolean logOption;
extern Boolean statusLineOption;
extern Boolean regressionTestOption;
extern Boolean fail;
extern Boolean anyOutput;
extern Boolean needSpace;
extern Boolean capitalize;

#define endOfTable(x) eot((Aword *) x)


/* Functions: */
extern void *allocate(unsigned long len);
extern void *duplicate(void *original, unsigned long len);
extern void terminate(int code);
extern void usage(void);
extern void error(MsgKind msg);
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
extern Boolean isLiteral(Aword x);
extern Boolean isNum(Aword x);
extern Boolean isStr(Aword x);

/* Run the game! */
extern void run(void);

#endif

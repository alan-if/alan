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

extern int memTop;		/* Top of memory */

extern int conjWord;		/* First conjunction in dictionary */

/* The Amachine memory */
extern Aword *memory;
extern AcdHdr *header;

/* Event queue */
extern int eventQueueSize;
extern EventQueueEntry *eventQueue;
extern Aint eventQueueTop;		/* Event queue top pointer */

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
#ifdef HAVE_GLK
#include "glk.h"
strid_t logFile;
#else
FILE *logFile;
#endif

/* File names */
extern char *adventureName;

/* Screen formatting info */
extern int col, lin;
extern int pageLength, pageWidth;

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restart_label;
extern jmp_buf error_label;

extern Bool verbose;
extern Bool ignoreErrorOption;
extern Bool debugOption;
extern Bool sectionTraceOption;
extern Bool tracePushOption;
extern Bool traceStackOption;
extern Bool singleStepOption;
extern Bool transcriptOption;
extern Bool logOption;
extern Bool statusLineOption;
extern Bool regressionTestOption;
extern Bool fail;
extern Bool anyOutput;
extern Bool needSpace;
extern Bool capitalize;

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
extern void logPrint(char string[]);

extern Bool eot(Aword *adr);
extern Bool isObj(Aword x);
extern Bool isCnt(Aword x);
extern Bool isAct(Aword x);
extern Bool isLoc(Aword x);
extern Bool isLiteral(Aword x);
extern Bool isNum(Aword x);
extern Bool isStr(Aword x);

/* Run the game! */
extern void run(void);

#endif

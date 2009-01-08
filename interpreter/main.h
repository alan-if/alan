#ifndef _MAIN_H_
#define _MAIN_H_
/*----------------------------------------------------------------------*\

  Header file for main unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"
#include <setjmp.h>


/* DATA */

extern int conjWord;		/* First conjunction in dictionary */

/* Event queue */
extern int eventQueueSize;
extern EventQueueEntry *eventQueue;
extern Aint eventQueueTop;		/* Event queue top pointer */

/* Amachine data structures - Dynamic */
extern Aword *scores;		/* Score table pointer */

/* Amachine data structures - Static */

extern VerbEntry *vrbs;		/* Verb table pointer */
extern SyntaxEntry *stxs;	/* Syntax table pointer */
extern RulEntry *ruls;		/* Rule table pointer */
extern EventEntry *events;	/* Event table pointer */
extern MessageEntry *msgs;	/* Message table pointer */
extern Aword *freq;		/* Cumulated character frequencies for text decoding */

/* The text and message file */
extern FILE *textFile;
#ifdef HAVE_GLK
#include "glk.h"
strid_t logFile;
#else
FILE *logFile;
#endif

/* File names */
extern char *adventureName;	/* The name of the game */
extern char *adventureFileName;

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restartLabel;
extern jmp_buf returnLabel;
extern jmp_buf forfeitLabel;
#define NO_JUMP_RETURN 0
#define ERROR_RETURN 1
#define UNDO_RETURN 2


extern Bool fail;



/* FUNCTIONS: */

/* Run the game! */
extern void run(void);

#endif

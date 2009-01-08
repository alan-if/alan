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
extern DictionaryEntry *dictionary; /* Dictionary pointer */
extern ClassEntry *classes;	/* Class table pointer */
extern ContainerEntry *container; /* Container table pointer */

extern VerbEntry *vrbs;		/* Verb table pointer */
extern SyntaxEntry *stxs;	/* Syntax table pointer */
extern RulEntry *ruls;		/* Rule table pointer */
extern EventEntry *events;	/* Event table pointer */
extern MessageEntry *msgs;	/* Message table pointer */
extern Aword *freq;		/* Cumulated frequencies */

extern int dictionarySize;		/* Number of entries in dictionary */

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

/* Screen formatting info */
extern int col, lin;
extern int pageLength, pageWidth;

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restartLabel;
extern jmp_buf returnLabel;
extern jmp_buf forfeitLabel;
#define NO_JUMP_RETURN 0
#define ERROR_RETURN 1
#define UNDO_RETURN 2


extern Bool fail;
extern Bool anyOutput;
extern Bool needSpace;
extern Bool capitalize;



/* FUNCTIONS: */

#ifndef SMARTALLOC
extern void *allocate(unsigned long len);
#else
#define allocate(s) calloc(s, 1)
#endif
extern void *duplicate(void *original, unsigned long len);
extern void error(MsgKind msg);
extern void output(char string[]);
extern void printMessage(MsgKind msg);
extern void printMessageWithParameters(MsgKind msg, Parameter *messageParameters);
extern void para(void);
extern void newline(void);
extern void printAndLog(char string[]);

extern Bool isObject(int instance);
extern Bool isContainer(int instance);
extern Bool isActor(int instance);
extern Bool isLocation(int instance);
extern Bool isLiteral(int instance);
extern Bool isNumeric(int instance);
extern Bool isString(int instance);

extern Bool isVerbWord(int wordIndex);
extern Bool isConjunctionWord(int wordIndex);
extern Bool isButWord(int wordIndex);
extern Bool isThemWord(int wordIndex);
extern Bool isItWord(int wordIndex);
extern Bool isNounWord(int wordIndex);
extern Bool isAdjectiveWord(int wordIndex);
extern Bool isPrepositionWord(int wordIndex);
extern Bool isAll(int wordCode);
extern Bool isAllWord(int wordIndex);
extern Bool isDirectionWord(int wordIndex);
extern Bool isNoise(int wordCode);
extern Bool isPronoun(int wordCode);
extern Bool isPronounWord(int wordIndex);
extern Bool isLiteralWord(int wordIndex);


/* Run the game! */
extern void run(void);

#endif

#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_
/*----------------------------------------------------------------------*\

			     Adventure.h

		      The Global Adventure Node
\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "List.h"
#include "Where.h"

#include "dump.h"


/* Types: */

typedef struct Adventure {	/* ADVENTURE */
  List *objectAttributes;	/* List of default object attributes */
  List *locationAttributes;	/* List of default location attributes */
  List *actorAttributes;	/* List of default actor attributes */
  List *classes;		/* List of defined classes */
  List *instances;		/* List of instances */
  List *synonyms;		/* List of synonyms */
  List *syntaxes;		/* List of syntax definitions */
  List *verbs;			/* List of global verbs */
  List *events;			/* List of events */
  List *rules;			/* List of rules */
  List *stringAttributes;	/* List of string attributes to initialize */
  Where *where;			/* Where to start */
  List *statements;		/* List of start statements */
  int *scores;			/* Pointer to array of scores */
  List *msgs;			/* List of error messages */
} Adventure;



/* Data: */
extern Adventure adventure;


/* Functions: */
#ifdef _PROTOTYPES_

/* Initialise some data */
extern void initAdventure(void);

/* Analyse the whole adventure */
extern void analyseAdventure(void);

/* Generate the whole adventure */
extern void generateAdventure(char acdFileName[]);

/* Dump the whole Adventure */
extern void dumpAdventure(DumpKind kind);

/* Print out a short summary of the adventure */
extern void summary(void);

#else
extern void initAdventure();
extern void analyseAdventure();
extern void generateAdventure();
extern void dumpAdventure();
extern void summary();
#endif

#endif

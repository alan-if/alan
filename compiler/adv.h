#ifndef _ADV_H_
#define _ADV_H_

/* USE other definitions */
#include "lst.h"
#include "whr.h"

#include "dump.h"


/* Types: */

typedef struct AdvNod {		/* ADVenture */
  List *atrs;			/* List of default attributes */
  List *oatrs;			/* List of default object attributes */
  List *latrs;			/* List of default location attributes */
  List *aatrs;			/* List of default actor attributes */
  List *syns;			/* List of synonyms */
  List *stxs;			/* List of syntax definitions */
  List *vrbs;			/* List of global verbs */
  List *clas;			/* List of defined classes */
  List *inss;			/* List of defined instances */
  List *locs;			/* List of locations */
  List *objs;			/* List of objects */
  List *evts;			/* List of events */
  List *cnts;			/* List of containers */
  List *acts;			/* List of actors */
  List *ruls;			/* List of rules */
  List *stratrs;		/* List of string attributes to initialize */
  WhrNod *whr;			/* Where to start */
  List *stms;			/* List of start statements */
  int *scores;			/* Pointer to array of scores */
  List *msgs;			/* List of error messages */
} AdvNod;



/* Data: */
extern AdvNod adv;


/* Functions: */

/* Initialise some data */
extern void initadv(void);

extern void symbolizeAdv(void);

/* Analyse the whole adventure */
extern void anadv(void);

/* Generate the whole adventure */
extern void geadv(char acdfnm[]);

/* Dump the whole Adventure */
extern void duadv(enum dmpKd dmp);

/* Print out a short summary of the adventure */
extern void summary(void);


#endif

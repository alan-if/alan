#ifndef _SLT_H_
#define _SLT_H_
/*----------------------------------------------------------------------*\

				SLT.H
			     Slots Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "whr.h"
#include "cnt.h"


/* Types */

typedef struct Slots {		/* SLOTS  */
  List *namslst;		/* List of lists of adjectives and a noun */
  List *namstms;		/* Name printing statements */
  Aword namsadr;		/* ACODE address to name printing */
  struct WhrNod *whr;		/* Where is it? */
  CntNod *cnt;			/* Container properties */
  List *atrs;			/* Attributes ... */
  Aword atradr;			/* ACODE address to attribute list */
  List *dscr;			/* Long description statements */
  Aword dscradr;		/* ACODE address to long description code */
  List *art;			/* Article printing statements */
  Aword artadr;			/* ACODE address to article statements */
  List *ment;			/* Short (inventory like) statements */
  Aword mentadr;		/* ACODE address to short description (mentioned) code */
  List *vrbs;			/* List of verbs */
  Aword vrbadr;			/* ACODE address to local verb table */
  List *scrs;			/* List of scripts */
  Aaddr scradr;			/* ACODE address to scripts */
} Slots;


/* Data: */

/* Methods: */


/* Create a new Slots node */
extern Slots *newSlots(List *nams,
		       struct WhrNod *whr,
		       List *atrs,
		       CntNod *cnt,
		       List *surr,
		       List *dscr,
		       List *ment,
		       List *art,
		       List *does,
		       List *exts,
		       List *vrbs,
		       List *scrs);

/* Dump a Slots node */
extern void dumpSlots(Slots *slots);


#endif

#ifndef _ACT_H_
#define _ACT_H_

/* USE other definitions */
#include "lst.h"
#include "srcp.h"
#include "nam.h"
#include "cnt.h"
#include "whr.h"

#include "acode.h"


/* Types: */

typedef struct ActNod {		/* ACTOR */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name */
  List *namslst;		/* List of lists of names (adjectives and noun) */
  WhrNod *whr;			/* Where initially? */
  CntNod *props;		/* Container properties */
  List *atrs;			/* List of attributes */
  Aaddr atradr;			/* ACODE address to attribute table */
  List *namstms;		/* Name printing statements */
  Aaddr namadr;			/* ACODE address to name printing code */
  List *dscr;			/* Description statements */
  Aaddr dscradr;		/* ACODE address to description routine  */
  List *vrbs;			/* List of local verbs */
  Aaddr vrbadr;			/* ACODE address to verbs */
  List *scrs;			/* List of scripts */
  Aaddr scradr;			/* ACODE address to scripts */
} ActNod;


/* Data: */

extern int aatrmax;
extern int actmin, actmax;
extern int actcount;



/* Functions: */

#ifdef _PROTOTYPES_

/* Initialise actor data */
extern void initact(void);

/* Allocate an Actor node */
extern ActNod *newact(Srcp *srcp,
		      NamNod *nam,
		      List *nams,
		      WhrNod *whr,
		      CntNod *props,
		      List *atrs,
		      List *dscr,
		      List *vrbs,
		      List *scripts);

/* Prepare Actors before analysis */
extern void prepacts(void);

/* Analyze the Actors */
extern void anacts(void);

/* Generate code for the Actors */
extern Aword geacts(void);

/* Dump an Actor node */
extern void duact(ActNod *act);

#else
extern void initact();
extern ActNod *newact();
extern void prepacts();
extern void anacts();
extern Aword geacts();
extern void duact();
#endif

#endif

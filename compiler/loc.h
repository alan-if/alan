#ifndef _LOC_H_
#define _LOC_H_

/* USE: */
#include "acode.h"
#include "srcp.h"
#include "nam.h"
#include "ext.h"
#include "lst.h"


/* Types: */

typedef struct LocNod {		/* LOCATION */
  Srcp srcp;			/* Source position */
  IdNod *id;			/* Name of this location */
  List *nams;			/* List of names */
  List *atrs;			/* Attributes ... */
  Aword atradr;			/* ACODE address of attribute table */
  List *namstms;		/* Statements printing the name */
  Aword namsadr;		/* ACODE address of name printing code */
  List *dscr;			/* List of statements as description */
  Aword dscradr;		/* ACODE address of description code */
  List *does;			/* List of statements when entered */
  Aword doesadr;		/* ACODE address of does code */
  List *vrbs;			/* List of verbs here */
  Aword vrbadr;			/* ACODE address of local verbs */
  List *exts;			/* List of exits */
  Aword extadr;			/* ACODE address of exit table */
} LocNod;



/* Data: */

extern int latrmax;
extern int locmin, locmax;
extern int loccount;


/* Functions: */


/* Create a new Location node */
extern LocNod *newloc(Srcp *srcp,
		      NamNod *nam,
		      List *nams,
		      List *atrs,
		      List *dscr,
		      List *does,
		      List *exts,
		      List *vrbs);

/* Prepare Locations before analysis */
extern void preplocs(void);

/* Analyse the Locations */
extern void anlocs(void);

/* Generate code for all Locations */
extern Aword gelocs(void);

/* Dump a Location node */
extern void duloc(LocNod *loc);


#endif

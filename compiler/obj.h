#ifndef _OBJ_H_
#define _OBJ_H_
/*----------------------------------------------------------------------*\

				OBJ.H
			     Object Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "lst.h"
#include "whr.h"
#include "cnt.h"


/* Types */

typedef struct ObjNod {		/* OBJECT */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name of the object */
  List *nams;			/* List of adjectives and a noun */
  WhrNod *whr;			/* Where is it? */
  List *art;			/* Article printing statements */
  Aword artadr;			/* ACODE address to attribute statements */
  CntNod *props;		/* Container properties */
  List *atrs;			/* Attributes ... */
  Aword atradr;			/* ACODE address to attribute list */
  List *namstms;		/* Name printing statements */
  Aword namsadr;		/* ACODE address to name printing */
  List *dscr1;			/* Long description statements */
  Aword d1adr;			/* ACODE address to long description code */
  List *dscr2;			/* Short (inventory) statements */
  Aword d2adr;			/* ACODE address to short description code */
  List *vrbs;			/* List of verbs */
  Aword vrbadr;			/* ACODE address to local verb table */
} ObjNod;


/* Data: */

extern int oatrmax;
extern int objmin, objmax;
extern int objcount;


/* Methods */

#ifdef _PROTOTYPES_

/* Create a new Object node */
extern ObjNod *newobj(Srcp *srcp,
		      NamNod *nam,
		      List *nams,
		      WhrNod *whr,
		      List *art,
		      CntNod *props,
		      List *atrs,
		      List *dscr1,
		      List *dscr2,
		      List *vrbs);

/* Analyze all Objects */
extern void anobjs(void);

/* Prepare all Objects before analysis */
extern void prepobjs(void);

/* Generate code for all Objects */
extern Aaddr geobjs(void);

/* Dump an Object node */
extern void duobj(ObjNod *obj);

#else
extern ObjNod *newobj();
extern void prepobjs();
extern void anobjs();
extern Aaddr geobjs();
extern void duobj();
#endif

#endif

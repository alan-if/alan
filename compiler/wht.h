#ifndef _WHT_H_
#define _WHT_H_
/*----------------------------------------------------------------------*\

				WHT.H
			      What Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "nam.h"


/* Types: */

typedef enum WhtKind {
  WHT_OBJ,
  WHT_LOC,
  WHT_ACT,
  WHT_ID
} WhtKind;

typedef struct WhtNod {		/* WHAT nodes */
  Srcp srcp;			/* Source position */
  WhtKind wht;			/* What kind */
  NamNod *nam;			/* What name */
} WhtNod;


/* Data: */


/* Functions: */

/* Create a new What node */
extern WhtNod *newwht(Srcp *srcp,
		      WhtKind wht,
		      NamNod *nam);

/* Generate a reference to a What */
extern void gewht(WhtNod *wht);

/* Dump a What node */
extern void duwht(WhtNod *wht);


#endif

#ifndef _WHT_H_
#define _WHT_H_
/*----------------------------------------------------------------------*\

				WHT.H
			      What Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "id.h"


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
  struct IdNod *id;			/* What identifier */
} WhtNod;


/* Data: */


/* Functions: */

/* Create a new What node */
extern WhtNod *newwht(Srcp *srcp,
		      WhtKind wht,
		      struct IdNod *id);

/* Generate a reference to a What */
extern void gewht(WhtNod *wht);

/* Dump a What node */
extern void duwht(WhtNod *wht);


#endif

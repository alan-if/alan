#ifndef _WHR_H_
#define _WHR_H_
/*----------------------------------------------------------------------*\

				WHR.H
			     Where Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "evt.h"
#include "wht.h"
#include "lst.h"


/* Types: */

typedef enum WhrKind {		/* WHERE kinds */
  WHR_DEFAULT,			/* None */
  WHR_HERE,			/* HERE */
  WHR_NEAR,			/* NEARBY */
  WHR_AT,			/* AT what */
  WHR_IN			/* IN what */
} WhrKind;


typedef struct WhrNod {		/* WHERE node */
  Srcp srcp;			/* Source position */
  WhrKind whr;			/* Where kind */
  WhtNod *wht;			/* What */
} WhrNod;


/* Data: */


/* Functions: */

/* Allocate a new Where node */
extern WhrNod *newwhr(Srcp *srcp,
		      WhrKind whr,
		      WhtNod *wht);

/* Analyse a Where reference */
extern void anwhr(WhrNod *whr, EvtNod *evt, List *pars);

/* Generate a Where reference */
extern void gewhr(WhrNod *whr);

/* Dump a Where node */
extern void duwhr(WhrNod *whr);


#endif

#ifndef _WHR_X_H_
#define _WHR_X_H_
/*----------------------------------------------------------------------*\

				WHR.H
			  Where Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "whr.h"

/* USE: */
#include "evt.h"


/* DATA: */


/* FUNCTIONS: */

/* Allocate a new Where node */
extern WhrNod *newwhr(Srcp *srcp,
		      WhrKind kind,
		      WhtNod *wht);

/* Symbolize a Where reference */
extern void symbolizeWhr(WhrNod *whr);

/* Analyse a Where reference */
extern void anwhr(WhrNod *whr, EvtNod *evt, List *pars);

/* Generate a Where reference */
extern void gewhr(WhrNod *whr);

/* Dump a Where node */
extern void duwhr(WhrNod *whr);

#endif

#ifndef _WHR_X_H_
#define _WHR_X_H_
/*----------------------------------------------------------------------*\

				WHR.H
			  Where Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "whr.h"

/* USE: */
#include "context.h"


/* DATA: */


/* FUNCTIONS: */

extern WhrNod *newwhr(Srcp *srcp,
		      WhrKind kind,
		      What *wht);
extern void symbolizeWhere(WhrNod *whr);
extern void verifyInitialLocation(WhrNod *whr);
extern void analyzeWhere(WhrNod *whr, Context *context);
extern Aword generateInitialLocation(WhrNod *whr);
extern void gewhr(WhrNod *whr, int currentInstance);
extern void duwhr(WhrNod *whr);

#endif

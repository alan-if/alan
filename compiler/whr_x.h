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

extern Where *newwhr(Srcp *srcp,
		      WhrKind kind,
		      What *wht);
extern void symbolizeWhere(Where *whr);
extern void verifyInitialLocation(Where *whr);
extern void analyzeWhere(Where *whr, Context *context);
extern Aword generateInitialLocation(Where *whr);
extern void gewhr(Where *whr, int currentInstance);
extern void duwhr(Where *whr);

#endif

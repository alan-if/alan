#ifndef _WHR_X_H_
#define _WHR_X_H_
/*----------------------------------------------------------------------*\

				WHR.H
			  Where Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "whr.h"

/* USE: */
#include "context.h"
#include "wht.h"


/* DATA: */


/* FUNCTIONS: */

extern Where *newWhere(Srcp *srcp, Bool directly, WhereKind kind, struct Expression *what);
extern void symbolizeWhere(Where *whr);
extern void verifyInitialLocation(Where *whr);
extern void analyzeWhere(Where *whr, Context *context);
extern Aword generateInitialLocation(Where *whr);
extern void generateWhere(Where *whr);
extern void dumpWhere(Where *whr);

#endif

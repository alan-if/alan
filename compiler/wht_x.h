#ifndef _WHT_X_H_
#define _WHT_X_H_
/*----------------------------------------------------------------------*\

				WHT_X.H
			  What Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "wht.h"


/* USE: */
#include "context.h"

/* DATA: */

/* FUNCTIONS: */

extern What *newWhat(Srcp *srcp, WhatKind wht, struct IdNode *id);
extern void symbolizeWhat(What *wht);
extern Bool verifyWhatContext(What *what, Context *context);
extern void generateWhat(What *wht);
extern void dumpWhat(What *wht);

#endif

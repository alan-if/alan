#ifndef _WHT_X_H_
#define _WHT_X_H_
/*----------------------------------------------------------------------*\

				WHT_X.H
			  What Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "wht.h"


/* USE: */

/* DATA: */

/* FUNCTIONS: */

extern What *newWhat(Srcp *srcp, WhatKind wht, struct IdNode *id);
extern void symbolizeWhat(What *wht);
extern void generateWhat(What *wht, int instanceCode);
extern void dumpWhat(What *wht);

#endif

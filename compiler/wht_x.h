#ifndef _WHT_X_H_
#define _WHT_X_H_
/*----------------------------------------------------------------------*\

				WHT.H
			  What Nodes Export
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "wht.h"

/* Data: */


/* Functions: */

/* Create a new What node */
extern WhtNod *newwht(Srcp *srcp,
		      WhtKind wht,
		      IdNode *id);

extern void symbolizeWht(WhtNod *wht);

/* Generate a reference to a What */
extern void gewht(WhtNod *wht);

/* Dump a What node */
extern void duwht(WhtNod *wht);


#endif

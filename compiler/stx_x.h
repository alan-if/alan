#ifndef _STX_X_H_
#define _STX_X_H_
/*----------------------------------------------------------------------*\

				STX.H
			      Syntax Nodes
                                                                     
\*----------------------------------------------------------------------*/

#include "stx.h"

/* USE: */



/* DATA: */


/* FUNCTIONS: */

/* Create a new Syntax node */
extern StxNod *newstx(Srcp *srcp,
		      IdNode *verb,
		      List *comps,
		      List *ress);

/* Create a deafult syntax node */
extern StxNod *defaultStx(char vrbstr[]);

/* Compare parameter lists of two syntaxes */
extern Bool eqparams(StxNod *stx1,
		     StxNod *stx2);

/* Analyze a list of Syntaxs */
extern void anstxs(void);

/* Generate code for a list of Syntaxs */
extern Aaddr gestxs(void);

/* Dump a Syntax node */
extern void dustx(StxNod *stx);


#endif

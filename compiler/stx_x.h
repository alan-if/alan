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

extern Syntax *newSyntax(Srcp *srcp,
			 IdNode *verb,
			 List *comps,
			 List *ress);

/* Create a default syntax node */
extern Syntax *defaultSyntax0(char vrbstr[]);
extern Syntax *defaultSyntax1(char vrbstr[]);

/* Compare parameter lists of two syntaxes */
extern Bool eqparams(Syntax *stx1,
		     Syntax *stx2);

extern void analyzeSyntaxes(void);
extern Aaddr generateAllSyntaxes(void);
extern void dumpSyntax(Syntax *stx);


#endif

#ifndef _STX_X_H_
#define _STX_X_H_
/*----------------------------------------------------------------------*\

				STX.H
			      Syntax Nodes
                                                                     
\*----------------------------------------------------------------------*/

#include "stx.h"

/* USE: */
#include "context.h"

/* DATA: */


/* FUNCTIONS: */

extern Syntax *newSyntax(Srcp srcp,
			 IdNode *verb,
			 List *comps,
			 List *ress,
			 Srcp restrictionSrcp);

/* Create a default syntax node */
extern Syntax *defaultSyntax0(char vrbstr[]);
extern Syntax *defaultSyntax1(IdNode *verb, Context *context);

/* Compare parameter lists of two syntaxes */
extern Bool equalParameterLists(Syntax *stx1,
		     Syntax *stx2);

extern void analyzeSyntaxes(void);
extern Aaddr generateParseTable(void);
extern Aaddr generateSyntaxMapping(void);
extern void dumpSyntax(Syntax *stx);


#endif

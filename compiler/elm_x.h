#ifndef _ELM_X_H_
#define _ELM_X_H_
/*----------------------------------------------------------------------*\

				ELM_X.H
		     Syntax Element Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "elm.h"

/* USE: */


/* DATA: */


/* FUNCTIONS: */

/* Create a new Syntax Element node */
extern ElmNod *newelm(Srcp *srcp,
		      ElementKind kind,
		      IdNode *id,
		      int flags);

/* Analyze a list of Syntax elements and return a list of the parameters */
extern List *anelms(List *elms, List *ress, struct StxNod *stx);

/* Generate code for a list of Syntax elements */
extern Aaddr geelms(List *elms, struct StxNod *stx);

/* Dump a Syntax node */
extern void duelm(ElmNod *elm);


#endif

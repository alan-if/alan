#ifndef _VRB_X_H_
#define _VRB_X_H_
/*----------------------------------------------------------------------*\

				VRB_X.H
			  Verb Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "vrb.h"

/* USE: */
#include "lst.h"
#include "context.h"


/* DATA: */


/* FUNCTIONS: */

extern Verb *newVerb(Srcp *srcp, List *ids, List *alts);
extern void analyzeVerbs(List *vrbs, Context *context);
extern Aaddr generateVerbs(List *vrbs);
extern void dumpVerb(Verb *vrb);


#endif

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


/* Data: */

extern int vrbmin, vrbmax;
extern int vrbcount;


/* Methods: */

/* Create a new Verb node */
extern VrbNod *newvrb(Srcp *srcp,
		      List *ids,
		      List *alts);

/* Analyze a list of Verbs */
extern void anvrbs(List *vrbs, Context *context);

/* Generate code for a list of Verbs */
extern Aaddr gevrbs(List *vrbs, int currentInstance);

/* Dump a Verb */
extern void duvrb(VrbNod *vrb);


#endif

#ifndef _VRB_X_H_
#define _VRB_X_H_
/*----------------------------------------------------------------------*\

				VRB_X.H
			  Verb Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "vrb.h"

/* USE: */
#include "lst.h"


/* Data: */

extern int vrbmin, vrbmax;
extern int vrbcount;


/* Methods: */

/* Create a new Verb node */
extern VrbNod *newvrb(Srcp *srcp,
		      List *ids,
		      List *alts);

/* Analyze a list of Verbs */
extern void anvrbs(List *vrbs, SymNod *symbol);

/* Generate code for a list of Verbs */
extern Aaddr gevrbs(List *vrbs, InsNod *ins);

/* Dump a Verb */
extern void duvrb(VrbNod *vrb);


#endif

#ifndef _VRB_H_
#define _VRB_H_
/*----------------------------------------------------------------------*\

				VRB.H
			      Verb Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "obj.h"
#include "act.h"
#include "stx.h"


/* Types: */

typedef struct VrbNod {		/* VERB */
  Srcp srcp;			/* Source position */
  List *nams;			/* List of verbs to perform this action */
  List *alts;			/* Alternative bodies */
  Aaddr altadr;			/* AN - Address to alt-table */
  StxNod *stx;			/* AN - Which syntax defines this verb */
} VrbNod;



/* Data: */

extern int vrbmin, vrbmax;
extern int vrbcount;


/* Methods: */

#ifdef _PROTOTYPES_

/* Create a new Verb node */
extern VrbNod *newvrb(Srcp *srcp,
		      List *nams,
		      List *alts);

/* Analyze a list of Verbs */
extern void anvrbs(List *vrbs,
		   ObjNod *obj,
		   ActNod *act);

/* Generate code for a list of Verbs */
extern Aaddr gevrbs(List *vrbs, ActNod *act);

/* Dump a Verb */
extern void duvrb(VrbNod *vrb);

#else
extern VrbNod *newvrb();
extern void anvrbs();
extern Aaddr gevrbs();
extern void duvrb();
#endif

#endif

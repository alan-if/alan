#ifndef _VRB_H_
#define _VRB_H_
/*----------------------------------------------------------------------*\

				VRB.H
			      Verb Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "ins.h"
#include "stx.h"


/* Types: */

typedef struct VrbNod {		/* VERB */
  Srcp srcp;			/* Source position */
  List *ids;			/* List of verb IDs to perform this action */
  List *alts;			/* Alternative bodies */
  Aaddr altadr;			/* AN - Address to alt-table */
  StxNod *stx;			/* AN - Which syntax defines this verb */
} VrbNod;



/* Data: */

extern int vrbmin, vrbmax;
extern int vrbcount;


/* Methods: */


/* Create a new Verb node */
extern VrbNod *newvrb(Srcp *srcp,
		      List *ids,
		      List *alts);

/* Analyze a list of Verbs */
extern void anvrbs(List *vrbs);

/* Generate code for a list of Verbs */
extern Aaddr gevrbs(List *vrbs, InsNod *ins);

/* Dump a Verb */
extern void duvrb(VrbNod *vrb);


#endif

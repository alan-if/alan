#ifndef _RES_X_H_
#define _RES_X_H_
/*----------------------------------------------------------------------*\

				RES_X.H
		Syntax Element Class Restriction Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "res.h"

/* USE: */
#include "srcp.h"
#include "stx.h"
#include "lst.h"
#include "id.h"


/* DATA: */


/* FUNCTIONS: */

extern ResNod *newRestriction(Srcp *srcp,
			      IdNode *parameterId,
			      RestrictionKind kind,
			      IdNode *classId,
			      List *stms);
extern Aaddr generateRestrictions(List *ress, StxNod *stx);
extern void analyzeRestrictions(List *ress, List *params);
extern void dumpRestriction(ResNod *res);


#endif

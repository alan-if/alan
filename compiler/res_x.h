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

/* Create a new element Restriction node */
extern ResNod *newres(Srcp *srcp,
		      IdNode *id,
		      IdNode *class,
		      List *stms);

/* Analyze a list of Restriction nodes */
extern void anress(List *ress, List *params);

/* Generate code for a list of Restriction nodes */
extern Aaddr geress(List *ress, StxNod *stx);

/* Dump a Restriction node */
extern void dures(ResNod *res);


#endif

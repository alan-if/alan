#ifndef _VRB_H_
#define _VRB_H_
/*----------------------------------------------------------------------*\

				VRB.H
			      Verb Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "sym.h"
#include "stx.h"


/* Types: */

typedef struct VrbNod {		/* VERB */
  Srcp srcp;			/* Source position */
  List *ids;			/* List of verb IDs to perform this action */
  List *alts;			/* Alternative bodies */
  SymNod *symbol;
  Aaddr altadr;			/* AN - Address to alt-table */
  StxNod *stx;			/* AN - Which syntax defines this verb */
} VrbNod;

#endif

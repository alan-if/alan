#ifndef _ID_X_H_
#define _ID_X_H_
/*----------------------------------------------------------------------*\

				ID_X.H
		       Identifier Nodes Export

\*----------------------------------------------------------------------*/

#include "id.h"

/* USE: */
#include "types.h"
#include "sym.h"

/* DATA: */


/* METHODS: */


/* Create a new Id node */
extern IdNode *newId(Srcp *srcp,
		     char str[]);

/* Check if two IDs are the same */
extern Bool equalId(IdNode *id1,	/* IN - The first id */
		    IdNode *id2);	/* IN - Second id */

/* Generate an Id node */
extern void geid(IdNode *id);

/* Dump an Id node */
extern void dumpId(IdNode *id);
#endif


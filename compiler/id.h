#ifndef _ID_H_
#define _ID_H_
/*----------------------------------------------------------------------*\

				Id.h

			   Identifier Nodes

\*----------------------------------------------------------------------*/

/* USE: */

#include "types.h"
#include "Srcp.h"


/* Types: */

typedef struct Id {		/* NAME */
  Srcp srcp;			/* Source position of the name */
  char *string;			/* String representation */
} Id;



/* Data: */


/* Methods: */

#ifdef _PROTOTYPES_

/* Create a new Id node */
extern Id *newId(Srcp *srcp, char string[]);

/* Compare two identifiers */
extern Bool equalIds(Id *id1, Id *id2);

/* Dump an identifier */
extern void dumpId(Id *id);

#else
extern Id *newId();
extern Bool equalIds();
extern void dumpId();
#endif

#endif

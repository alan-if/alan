#ifndef _ID_H_
#define _ID_H_
/*----------------------------------------------------------------------*\

				ID.H
			Identifier Nodes Types

\*----------------------------------------------------------------------*/

/* USE: */
#include "sym.h"
#include "srcp.h"


/* TYPES: */

typedef enum IdKind {
  CLASS_ID,
  INSTANCE_ID,
  EVENT_ID,
  VERB_ID
} IdKind;


typedef struct IdNode {		/* ID */
  Srcp srcp;			/* Source position of the identifier */
  char *string;			/* String representation */
  SymNod *symbol;		/* Symbol table pointer */
} IdNode;

#endif



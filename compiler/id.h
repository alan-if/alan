#ifndef _ID_H_
#define _ID_H_
/*----------------------------------------------------------------------*\

				ID.H
			      Name Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "types.h"

#include "sym.h"


/* Types: */

typedef enum IdKind {
  CLASS_ID,
  INSTANCE_ID,
  EVENT_ID,
  VERB_ID
} IdKind;


typedef struct IdNode {		/* ID */
  Srcp srcp;			/* Source position of the identifier */
  char *string;			/* String representation */
  IdKind kind;			/* What kind of ID */
  struct SymNod *symbol;	/* Symbol table pointer */
} IdNode;


/* Data: */


/* Methods: */


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

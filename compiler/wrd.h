#ifndef _WRD_H_
#define _WRD_H_
/*----------------------------------------------------------------------*\

				WRD.H
			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "../interpreter/acode.h"
#include "lst.h"
#include "id.h"


/* TYPES: */

typedef struct WordNode {	/* DICTIONARY ENTRY */
  int classbits;		/* Class of this entry as a bit in the set */
  int code;			/* Code for the word */
  char *string;			/* Name of this entry */
  List *ref[WRD_CLASSES];	/* Lists of references (objects etc) */
  Aaddr stradr;			/* ACODE address to string */
  Aaddr nounrefadr;		/* ACODE address to reference table */
  Aaddr adjrefadr;		/* ACODE address to reference table */
  struct WordNode *low, *high;	/* Links */
} WordNode;

#endif

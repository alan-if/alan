#ifndef _LIMIT_H_
#define _LIMIT_H_
/*----------------------------------------------------------------------*\

			       Limit.h

			     Limit Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "Srcp.h"
#include "Attribute.h"
#include "List.h"


/* Types: */

typedef struct Limit {		/* LIMIT */
  Srcp srcp;			/* Source position */
  Attribute *attribute;		/* The attribute that limits */
  List *statements;		/* Statements to execute when exceeded */
  Aaddr stmadr;			/* ACODE address to statements */
} Limit;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Limit node */
extern Limit *newLimit(Srcp *srcp,
		       Attribute *attribute,
		       List *statements);

/* Analyse a Limit node */
extern void analyseLimit(Limit *limit);

/* Generate code for the Limits of a container */
extern Aword generateLimits();

/* Dump a Limit node */
extern void dumpLimit(Limit *limit);

#else
extern Limit *newLimit();
extern void analyseLimit();
extern Aword generateLimits();
extern void dumpLimit();
#endif

#endif

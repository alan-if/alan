#ifndef _ALTERNATIVE_H_
#define _ALTERNATIVE_H_
/*----------------------------------------------------------------------*\

			    Alternative.h

			Verb Alternative Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Id.h"
#include "Does.h"

#include "acode.h"


/* Types: */

typedef struct Alternative {	/* ALTERNATIVE */
  Srcp srcp;			/* Source position of this alternative */
  Id *id;			/* The parameter ID */
  List *checks;			/* Checks */
  Aaddr chkadr;			/* ACODE address to check table */
  Does *does;			/* Does-part */
  Aaddr stmadr;			/* ACODE address to action code */
} Alternative;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new verb Alternative node */
extern Alternative *newAlternative(Srcp *srcp,
				   Id *id,
				   List *checks,
				   Does *does);

/* Analyze a list of verb Alternatives */
extern void analyseAlternatives(List *alternatives);

/* Generate code for a list of verb Alternatives */
extern Aaddr generateAlternatives(List *alternatives);

/* Dump a verb Alternative */
extern void dumpAlternative(Alternative *alternative);

#else
extern Alternative *newAlternative();
extern void analyseAlternatives();
extern Aaddr generateAlternatives();
extern void dumpAlternative();
#endif

#endif

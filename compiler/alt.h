#ifndef _ALT_H_
#define _ALT_H_
/*----------------------------------------------------------------------*\

				ALT.H
		       Verb Alternatives Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "ins.h"
#include "id.h"

#include "acode.h"


/* Types: */

typedef enum QualKind {		/* QUAL kinds */
  QUAL_BEFORE,
  QUAL_AFTER,
  QUAL_ONLY,
  QUAL_DEFAULT
} QualKind;


typedef struct AltNod {		/* ALTERNATIVE */
  Srcp srcp;			/* Source position of this alternative */
  IdNod *id;			/* The parameter ID */
  int parameterNumber;
  QualKind qual;		/* Qualifier, when to execute */
  List *chks;			/* Checks */
  Aaddr chkadr;			/* ACODE address to check table */
  List *stms;			/* Does-part statements */
  Aaddr stmadr;			/* ACODE address to action code */
} AltNod;



/* Data: */


/* Functions: */

/* Create a new verb Alternative node */
extern AltNod *newalt(Srcp *srcp,
		      IdNod *id,
		      List *chks,
		      QualKind qual,
		      List *stms);

/* Analyze a list of verb Alternatives */
extern void analts(List *alts, InsNod *ins, List *pars);

/* Generate code for a list of verb Alternatives */
extern Aaddr gealts(List *alts, InsNod *ins);

/* Dump a verb Alternative */
extern void dualt(AltNod *alt);


#endif

#ifndef _ALT_H_
#define _ALT_H_
/*----------------------------------------------------------------------*\

				ALT.H
		       Verb Alternatives Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "act.h"
#include "nam.h"

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
  NamNod *nam;			/* The parameter ID */
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
		      NamNod *nam,
		      List *chks,
		      QualKind qual,
		      List *stms);

/* Analyze a list of verb Alternatives */
extern void analts(List *alts, ActNod *act, List *pars);

/* Generate code for a list of verb Alternatives */
extern Aaddr gealts(List *alts, ActNod *act);

/* Dump a verb Alternative */
extern void dualt(AltNod *alt);


#endif

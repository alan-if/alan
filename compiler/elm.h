#ifndef _ELM_H_
#define _ELM_H_
/*----------------------------------------------------------------------*\

				ELM.H
			      Syntax Element Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"

#include "id.h"
#include "lst.h"
#include "stx.h"

#include "acode.h"


/* Types: */

typedef enum ElmKind {
  ELMPAR,			/* Parameter Element */
  ELMWRD,			/* Player Word Element */
  ELMEOS			/* End of syntax */
} ElmKind;

typedef struct ElmNod {		/* ELEMENT */
  Srcp srcp;			/* Source position of this element */
  ElmKind kind;			/* Kind of Parameter */
  IdNod *id;			/* Name */
  int flags;			/* Multiple/omni etc. flags */
  int no;			/* AN - parameter number */
  struct ResNod *res;		/* AN - link to the class restriction check */
  struct StxNod *stx;		/* AN - pointer back to syntax node */
} ElmNod;



/* Data: */


/* Functions: */

/* Create a new Syntax Element node */
extern ElmNod *newelm(Srcp *srcp,
		      ElmKind kind,
		      struct IdNod *id,
		      int flags);

/* Analyze a list of Syntax elements and return a list of the parameters */
extern List *anelms(List *elms, List *ress, StxNod *stx);

/* Generate code for a list of Syntax elements */
extern Aaddr geelms(List *elms, StxNod *stx);

/* Dump a Syntax node */
extern void duelm(ElmNod *elm);


#endif

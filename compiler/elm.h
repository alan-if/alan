#ifndef _ELM_H_
#define _ELM_H_
/*----------------------------------------------------------------------*\

				ELM.H
			      Syntax Element Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"

#include "res.h"		/* RES-Nodes */
#include "stx.h"		/* STX-Nodes */

#include "lst.h"
#include "nam.h"

#include "acode.h"


/* Types: */

typedef enum ElmKind {
  ELMPAR,			/* Parameter Element */
  ELMWRD			/* Player Word Element */
} ElmKind;

typedef struct ElmNod {		/* ELEMENT */
  Srcp srcp;			/* Source position of this element */
  ElmKind kind;			/* Kind of Parameter */
  NamNod *nam;			/* Name */
  Bool multiple;		/* Multiple indicator */
  int no;			/* AN - parameter number */
  ResNod *res;			/* AN - link to the class restriction check */
  StxNod *stx;			/* AN - pointer back to syntax node */
} ElmNod;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Syntax Element node */
extern ElmNod *newelm(Srcp *srcp,
		      ElmKind kind,
		      NamNod *nam,
		      Bool multiple);

/* Analyze a list of Syntax elements and return a list of the parameters */
extern List *anelms(List *elms, List *ress, StxNod *stx);

/* Generate code for a list of Syntax elements */
extern Aaddr geelms(List *elms);

/* Dump a Syntax node */
extern void duelm(ElmNod *elm);

#else
extern ElmNod *newelm();
extern List *anelms();
extern Aaddr geelms();
extern void duelm();
#endif

#endif

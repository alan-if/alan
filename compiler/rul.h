#ifndef _RUL_H_
#define _RUL_H_
/*----------------------------------------------------------------------*\

				RUL.H
			      Rule Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "exp.h"
#include "lst.h"


/* Types: */

typedef struct RulNod {		/* RULE */
  Srcp srcp;			/* Source position */
  ExpNod *exp;			/* Expression */
  Aaddr expadr;			/* ACODE address to expression code */
  List *stms;			/* Statements */
  Aaddr stmadr;			/* ACODE address to statements */
} RulNod;


/* Data: */
extern int rulmin, rulmax;
extern int rulcount;



/* Functions: */
#ifdef _PROTOTYPES_

/* Allocate a new Rule node */
extern RulNod *newrul(Srcp *srcp,
		      ExpNod *exp,
		      List *stms);

/* Analyse all Rules */
extern void anruls(void);

/* Generate code for all Rules */
extern Aaddr geruls(void);

/* Dump a Rule node */
extern void durul(RulNod *rul);

#else
extern RulNod *newrul();
extern void anruls();
extern Aaddr geruls();
extern void durul();
#endif

#endif

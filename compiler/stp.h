#ifndef _STP_H_
#define _STP_H_
/*----------------------------------------------------------------------*\

				STP.H
			      Step Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "exp.h"
#include "act.h"
#include "lst.h"


/* Types: */

typedef struct StpNod {		/* STEP */
  Srcp srcp;			/* Source position */
  int after;			/* Possibly after some time */
  ExpNod *exp;			/* or an expression */
  Aaddr expadr;			/* ACODE address to expression */
  List *stms;			/* Statements in this block */
  Aaddr stmadr;			/* ACODE address to statements */
} StpNod;



/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

extern StpNod *newstp(Srcp *srcp,
		      int after,
		      ExpNod *exp,
		      List *stms);

/* Analyse all Steps in a list */
extern void anstps(List *stps, ActNod *act);

/* Generate code for all Steps in a list */
extern Aaddr gestps(List *stps, ActNod *act);

/* Dump a Step node */
extern void dustp(StpNod *stp);

#else
extern StpNod *newstp();
extern void anstps();
extern Aaddr gestps();
extern void dustp();
#endif

#endif

#ifndef _LIM_H_
#define _LIM_H_
/*----------------------------------------------------------------------*\

				LIM.H
			     Limit Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "srcp.h"
#include "atr.h"
#include "lst.h"
#include "cnt.h"


/* Types: */

typedef struct LimNod {		/* LIMIT */
  Srcp srcp;			/* Source position */
  AtrNod *atr;			/* The attribute that limits */
  List *stms;			/* Statements to execute when exceeded */
  Aaddr stmadr;			/* ACODE address to statements */
} LimNod;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Limit node */
extern LimNod *newlim(Srcp *srcp,
		      AtrNod *atr,
		      List *stms);

/* Analyse a Limit node */
extern void anlim(LimNod *lim);

/* Generate code for the Limits of a container */
extern Aword gelims(CntNod *cnt);

/* Dump a Limit node */
extern void dulim(LimNod *lim);

#else
extern LimNod *newlim();
extern void anlim();
extern Aword gelims();
extern void dulim();
#endif

#endif

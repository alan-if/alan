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

/* Create a new Limit node */
extern LimNod *newlim(Srcp *srcp,
		      AtrNod *atr,
		      List *stms);

/* Analyse a Limit node */
extern void anlim(LimNod *lim);

/* Generate code for the Limits of a container */
extern Aword gelims(CntNod *cnt, int currentInstance);

/* Dump a Limit node */
extern void dulim(LimNod *lim);


#endif

#ifndef _STEP_H_
#define _STEP_H_
/*----------------------------------------------------------------------*\

				Step.h

			      Step Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Expression.h"

#include "acode.h"


/* Types: */

typedef struct Step {		/* STEP */
  Srcp srcp;			/* Source position */
  Expression *after;		/* Possibly after some number of ticks */
  Expression *expression;	/* or an expression */
  Aaddr expadr;			/* ACODE address to expression */
  List *statements;		/* Statements in this block */
  Aaddr stmadr;			/* ACODE address to statements */
} Step;



/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

extern Step *newStep(Srcp *srcp,
		     Expression *after,
		     Expression *expression,
		     List *statements);

/* Analyse all Steps in a list */
extern void analyseSteps(List *steps);

/* Generate code for all Steps in a list */
extern Aaddr generateSteps(List *steps);

/* Dump a Step node */
extern void dumpStep(Step *step);

#else
extern Step *newStep();
extern void analyseSteps();
extern Aaddr generateSteps();
extern void dumpStep();
#endif

#endif

#ifndef _STP_H_
#define _STP_H_
/*----------------------------------------------------------------------*\

				STP.H
			      Step Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "exp.h"
#include "lst.h"
#include "../interpreter/acode.h"


/* TYPES: */

typedef struct Step {
  Srcp srcp;			/* Source position */
  int after;			/* Possibly after some time */
  Expression *exp;			/* or an expression */
  Aaddr expadr;			/* ACODE address to expression */
  List *stms;			/* Statements in this block */
  Aaddr stmadr;			/* ACODE address to statements */
} Step;

#endif

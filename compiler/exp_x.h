#ifndef _EXP_X_H_
#define _EXP_X_H_
/*----------------------------------------------------------------------*\

				EXP.H
			   Expression Nodes

\*----------------------------------------------------------------------*/

#include "exp.h"

/* USE other definitions */
#include "alan.h"
#include "srcp.h"
#include "wht.h"
#include "whr.h"
#include "evt.h"
#include "id.h"



/* DATA: */


/* FUNCTIONS: */

/* Create a new Expression node */
extern ExpNod *newexp(Srcp *srcp,
		      ExpKind class);

/* Analyze an Expression */
extern void anexp(ExpNod *exp, EvtNod *evt, List *params);

/* Compare two types */
extern Bool eqtyp(TypeKind typ1,
		  TypeKind typ2);

/* Generate code for an Expression */
extern void geexp(ExpNod *exp);


extern void dumpType(TypeKind typ);
extern void dumpExpression(ExpNod *exp);

#endif

#ifndef _EXP_X_H_
#define _EXP_X_H_
/*----------------------------------------------------------------------*\

				EXP_X.H
			   Expression Nodes

\*----------------------------------------------------------------------*/

#include "exp.h"

/* USE other definitions */
#include "srcp.h"
#include "context.h"



/* DATA: */


/* FUNCTIONS: */

/* Create a new Expression node */
extern ExpNod *newexp(Srcp *srcp,
		      ExpKind class);

/* Analyze an Expression */
extern void anexp(ExpNod *exp, Context *context);

/* Compare two types */
extern Bool equalTypes(TypeKind typ1,
		       TypeKind typ2);

/* Generate code for an Expression */
extern void geexp(ExpNod *exp);


extern void dumpType(TypeKind typ);
extern void dumpExpression(ExpNod *exp);

#endif

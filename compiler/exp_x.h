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

extern ExpNod *newexp(Srcp *srcp, ExpKind class);
extern void anexp(ExpNod *exp, Context *context);
extern Bool equalTypes(TypeKind typ1, TypeKind typ2);
extern void geexp(ExpNod *exp);
extern void generateBinaryOperator(ExpNod *exp);
extern void generateAttributeAccess(ExpNod *exp);
extern void generateBetweenCheck(ExpNod *exp);
extern void dumpType(TypeKind typ);
extern void dumpExpression(ExpNod *exp);

#endif

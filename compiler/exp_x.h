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

extern Expression *newExpression(Srcp *srcp, ExpressionKind kind);
extern void analyzeExpression(Expression *exp, Context *context);
extern Bool equalTypes(TypeKind typ1, TypeKind typ2);
extern Symbol *symbolOf(Expression *exp);
extern void generateExpression(Expression *exp);
extern void generateBinaryOperator(Expression *exp);
extern void generateAttributeReference(Expression *exp);
extern void generateAttributeAccess(Expression *exp);
extern void generateBetweenCheck(Expression *exp);
extern void dumpType(TypeKind typ);
extern void dumpExpression(Expression *exp);

#endif

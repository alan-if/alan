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
extern Expression *newAttributeExpression(Srcp srcp, IdNode *attribute, Bool not, Expression *ofWhat);
extern Expression *newStringExpression(Srcp srcp, long fpos, int len);
extern Expression *newIntegerExpression(Srcp srcp, int value);
extern Expression *newIsaExpression(Srcp srcp, Expression *what, Bool not, IdNode *class);
extern Expression *newWhatExpression(Srcp srcp, What *what);

extern void symbolizeExpression(Expression *exp);
extern void analyzeExpression(Expression *exp, Context *context);
extern Bool equalTypes(TypeKind typ1, TypeKind typ2);
extern Symbol *contentOf(Expression *what, Context *context);
extern void verifyContainerExpression(Expression *what, Context *context, char referer[]);
extern Symbol *symbolOfExpression(Expression *exp, Context *context);
extern void generateExpression(Expression *exp);
extern void generateBinaryOperator(Expression *exp);
extern void generateAttributeReference(Expression *exp);
extern void generateAttributeAccess(Expression *exp);
extern void generateBetweenCheck(Expression *exp);
extern void dumpType(TypeKind typ);
extern void dumpExpression(Expression *exp);

#endif

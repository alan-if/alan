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



/* Data: */


/* Functions: */

extern Expression *newAttributeExpression(Srcp srcp, IdNode *attribute,
					  Bool not, Expression *ofWhat);
extern Expression *newBinaryExpression(Srcp srcp, Expression *left, Bool not,
				       OperatorKind operator,
				       Expression *right);
extern Expression *newBetweenExpression(Srcp srcp, Expression *exp, Bool not,
					Expression *low, Expression *high);
extern Expression *newStringExpression(Srcp srcp, long fpos, int len);
extern Expression *newSetExpression(Srcp srcp, List *set);
extern Expression *newScoreExpression(Srcp srcp);
extern Expression *newIntegerExpression(Srcp srcp, int value);
extern Expression *newIsaExpression(Srcp srcp, Expression *what, Bool not,
				    IdNode *class);
extern Expression *newWhatExpression(Srcp srcp, What *what);
extern Expression *newWhereExpression(Srcp srcp, Expression *what, Bool not, Where *where);
extern Expression *newAggregateExpression(Srcp srcp, AggregateKind kind,
					  IdNode *attribute, List *filters);
extern Expression *newRandomRangeExpression(Srcp srcp, Expression *from,
					    Expression *to);
extern Expression *newRandomInExpression(Srcp srcp, Expression *what);

extern void symbolizeExpression(Expression *exp);
extern void analyzeExpression(Expression *exp, Context *context);
extern void analyzeFilterExpressions(char *message, List *filters,
				     Context *context, IdNode **classId,
				     Bool *error);
extern Bool equalTypes(TypeKind typ1, TypeKind typ2);
extern Bool isConstantIdentifier(IdNode *id);
extern Bool isConstantExpression(Expression *exp);
extern Symbol *contentOf(Expression *what, Context *context);
extern void verifyContainerExpression(Expression *what, Context *context,
				      char referer[]);
extern Symbol *symbolOfExpression(Expression *exp, Context *context);
extern void generateExpression(Expression *exp);
extern void generateSetExpression(Expression *what, Expression *exp);
extern void generateFilter(Expression *exp);
extern void generateBinaryOperator(Expression *exp);
extern void generateAttributeReference(Expression *exp);
extern void generateAttributeAccess(Expression *exp);
extern void generateBetweenCheck(Expression *exp);
extern void dumpType(TypeKind typ);
extern void dumpExpression(Expression *exp);

#endif

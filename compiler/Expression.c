/*----------------------------------------------------------------------*\

				Expression.c

			      Expression Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Expression.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newExpression()

  Allocates and initialises a Expression.

 */
#ifdef _PROTOTYPES_
Expression *newExpression(Srcp *srcp,	/* IN - Source position */
	      ExpressionKind kind)	/* IN - Kind of Expression */
#else
Expression *newExpression(srcp, kind)
     Srcp *srcp;
     ExpressionKind kind;
#endif
{
  Expression *new;			/* The newly allocated area */

  new = NEW(Expression);

  new->srcp = *srcp;
  new->kind = kind;

  return(new);
}


/*======================================================================

  analyseExpression()

  Analyze a EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseExpression(Expression *expression, List *parameters)
#else
void analyseExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - analyse expression */
}



/*======================================================================

  dumpType()

  Dump a type.

  */
#ifdef _PROTOTYPES_
void dumpType(TypeKind type)
#else
void dumpQualifier(type)
     TypeKind type;
#endif
{
  switch (type) {
  case UNKNOWN_TYPE: put("UNKNOWN"); break;
  case INTEGER_TYPE: put("INTEGER"); break;
  case BOOLEAN_TYPE: put("BOOLEAN"); break;
  case STRING_TYPE: put("STRING"); break;
  case INSTANCE_TYPE: put("INSTANCE"); break;
  default: put("*** ERROR unknown type ***");
  }
}



/*----------------------------------------------------------------------

  dumpExpressionKind()

  Dump the kind of an expression.

  */
#ifdef _PROTOTYPES_
static void dumpExpressionKind(ExpressionKind kind)
#else
void dumpExpressionKind(kind)
     ExpressionKind kind;
#endif
{
  switch (kind) {
  case WHERE_EXPRESSION: put("WHERE"); break;
  case ATTRIBUTE_EXPRESSION: put("ATTRIBUTE"); break;
  case BINARY_EXPRESSION: put("BINARY"); break;
  case INTEGER_EXPRESSION: put("INTEGER"); break;
  case STRING_EXPRESSION: put("STRING"); break;
  case AGGREGATE_EXPRESSION: put("AGGREGATE"); break;
  case RANDOM_EXPRESSION: put("RANDOM"); break;
  case SCORE_EXPRESSION: put("SCORE"); break;
  case WHAT_EXPRESSION: put("WHAT"); break;
  default: put("*** ERROR unknown expression kind ***");
  }
}



/*----------------------------------------------------------------------

  dumpOperator()

  Dump the name of an operator.

  */
#ifdef _PROTOTYPES_
static void dumpOperator(OperatorKind kind)
#else
void dumpOperator(kind)
     OperatorKind kind;
#endif
{
  switch (kind) {
  case PLUS_OPERATOR: put("PLUS"); break;
  case MINUS_OPERATOR: put("MINUS"); break;
  case MULT_OPERATOR: put("MULT"); break;
  case DIV_OPERATOR: put("DIV"); break;
  case AND_OPERATOR: put("AND"); break;
  case OR_OPERATOR: put("OR"); break;
  case NE_OPERATOR: put("NE"); break;
  case EQ_OPERATOR: put("EQ"); break;
  case EXACT_OPERATOR: put("EXACT"); break;
  case LE_OPERATOR: put("LE"); break;
  case GE_OPERATOR: put("GE"); break;
  case LT_OPERATOR: put("LT"); break;
  case GT_OPERATOR: put("GT"); break;
  default: put("*** ERROR unknown operator kind ***");
  }
}


/*----------------------------------------------------------------------

  dumpAggregate()

  Dump the name of an aggregate.

  */
#ifdef _PROTOTYPES_
static void dumpAggregate(AggregateKind kind)
#else
void dumpAggregate(kind)
     AggregateKind kind;
#endif
{
  switch (kind) {
  case SUM_AGGREGATE: put("SUM"); break;
  case MAX_AGGREGATE: put("MAX"); break;
  case COUNT_AGGREGATE: put("COUNT"); break;
  default: put("*** ERROR unknown aggregate kind ***");
  }
}


/*======================================================================

  dumpExpression()

  Dump a Expression node.

 */
#ifdef _PROTOTYPES_
void dumpExpression(Expression *expression)
#else
void dumpExpression(expression)
     Expression *expression;
#endif
{
  if (expression == NULL) {
    put("NULL");
    return;
  }

  put("EXPRESSION: "); dumpSrcp(&expression->srcp); in();
  put("kind: "); if (expression->not) put("NOT "); dumpExpressionKind(expression->kind); nl();
  put("type: "); dumpType(expression->type); nl();

  switch (expression->kind) {
  case WHERE_EXPRESSION:
    put("what: "); dumpExpression(expression->fields.where.what); nl();
    put("where: "); dumpWhere(expression->fields.where.where);
    break;
  case ATTRIBUTE_EXPRESSION:
    put("what: "); dumpExpression(expression->fields.attribute.what); nl();
    put("attribute: "); dumpId(expression->fields.attribute.attribute);
    break;
  case BINARY_EXPRESSION:
    put("operator: "); dumpOperator(expression->fields.binary.operator); nl();
    put("left: "); dumpExpression(expression->fields.binary.left); nl();
    put("right: "); dumpExpression(expression->fields.binary.right);
    break;
  case INTEGER_EXPRESSION:
    put("value: "); dumpInteger(expression->fields.value.value);
    break;
  case STRING_EXPRESSION:
    put("fpos: "); dumpInteger(expression->fields.string.fpos); nl();
    put("len: "); dumpInteger(expression->fields.string.len);
    break;
  case AGGREGATE_EXPRESSION:
    put("kind: "); dumpAggregate(expression->fields.aggregate.kind); nl();
    put("attribute: "); dumpId(expression->fields.aggregate.attribute); nl();
    put("where: "); dumpWhere(expression->fields.aggregate.where);
    break;
  case RANDOM_EXPRESSION:
    put("from: "); dumpInteger(expression->fields.random.from); nl();
    put("to: "); dumpInteger(expression->fields.random.to);
    break;
  case WHAT_EXPRESSION:
    put("what: "); dumpWhat(expression->fields.what.what);
    break;
  case SCORE_EXPRESSION:
    break;
  default: put("*** ERROR unknown expression fields ***");
  }
  out();

}

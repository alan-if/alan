/*----------------------------------------------------------------------*\

				Expression.c

			      Expression Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Expression.h"
#include "Statement.h"
#include "Symbol.h"

#include "lmList.h"

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
  new->type = ERROR_TYPE;

  return(new);
}


/*======================================================================

  equalTypes()

  Analyze a EXPRESSION.

 */
#ifdef _PROTOTYPES_
Bool equalTypes(TypeKind type1, TypeKind type2)
#else
Bool equalTypes(type1, type2)
     TypeKind type1, type2;
#endif
{
  if (type1 == ERROR_TYPE || type2 == ERROR_TYPE)
    syserr("Unintialised type in equalTypes()");
  return (type1 == UNKNOWN_TYPE || type2 == UNKNOWN_TYPE || type1 == type2);
}



/*----------------------------------------------------------------------

  analyseWhereExpression()

  Analyze a WHERE_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseWhereExpression(Expression *expression, List *parameters)
#else
void analyseWhereExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* First analyse the WHAT */
  analyseExpression(expression->fields.where.what, parameters);
  if (expression->fields.where.what->kind != WHAT_EXPRESSION)
    lmLog(&expression->fields.where.what->srcp, 311, sevERR, "an Instance");
  else {
    switch (expression->fields.where.what->kind) {
    case WHAT_LOCATION:
      /* 4f - Warn for single classed LOCATIONS, they can not be anywhere! */
      break;
    case WHAT_ACTOR:
      /* 4f - Can not refer to actors if environment is an EVENT */
      break;
    case WHAT_ID:
      (void) symbolCheck(expression->fields.where.what->fields.what.what->id, INSTANCE_SYMBOL);
      break;
    case WHAT_THIS:
      /* 4f - what this? */
    default:
      syserr("Unrecognized switch in analyseWhereExpression().");
      break;
    }
  }

  /* Now analyse the WHERE */
  switch (expression->fields.where.where->kind) {
  case WHERE_HERE:
  case WHERE_NEAR:
    /* 4f - Warn if environment is a RULE */
    break;
  case WHERE_AT:
    switch (expression->fields.where.where->what->kind) {
    case WHAT_ID:
      /* 4f - check the symbol for being a THING */
      break;
    case WHAT_LOCATION:
      expression->fields.where.where->kind = WHERE_HERE;
      break;
    case WHAT_THIS:
      /* 4f - At this? */
      break;
    case WHAT_ACTOR:
      /* 4f - what AT ACTOR */
      break;
    default:
      syserr("Unrecognized switch in analyseWhereExpression().");
      break;
    }
    break;
  case WHERE_IN:
    /* 4f - verify that it is a container */
    break;
  default:
      syserr("Unrecognized switch in analyseWhereExpression().");
    break;
  }
  expression->type = BOOLEAN_TYPE;
}



/*----------------------------------------------------------------------

  analyseAttributeExpression()

  Analyze a ATTRIBUTE_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseAttributeExpression(Expression *expression, List *parameters)
#else
void analyseAttributeExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - Analyse Attribute Expression */
}



/*----------------------------------------------------------------------

  analyseBinaryExpression()

  Analyze a BINARY_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseBinaryExpression(Expression *expression, List *parameters)
#else
void analyseBinaryExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - Analyse Attribute Expression */
}



/*----------------------------------------------------------------------

  analyseRandomExpression()

  Analyze a RANDOM_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseRandomExpression(Expression *expression, List *parameters)
#else
void analyseRandomExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - Analyse Random Expression */
}



/*----------------------------------------------------------------------

  analyseWhatExpression()

  Analyze a WHAT_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseWhatExpression(Expression *expression, List *parameters)
#else
void analyseWhatExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - Analyse What Expression */
}



/*----------------------------------------------------------------------

  analyseAggregateExpression()

  Analyze a AGGREGATE_EXPRESSION.

 */
#ifdef _PROTOTYPES_
void analyseAggregateExpression(Expression *expression, List *parameters)
#else
void analyseAggregateExpression(expression, parameters)
     Expression *expression;
     List *parameters;
#endif
{
  /* 4f - Analyse Aggregate Expression */
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
  if (expression == NULL) return; /* Ignore empty expressions (syntax error) */
  
  switch (expression->kind) {
  case WHERE_EXPRESSION:
    analyseWhereExpression(expression, parameters);
    break;
  case ATTRIBUTE_EXPRESSION:
    analyseAttributeExpression(expression, parameters);
    break;
  case BINARY_EXPRESSION:
    analyseBinaryExpression(expression, parameters);
    break;
  case INTEGER_EXPRESSION:
  case SCORE_EXPRESSION:
    expression->type = INTEGER_TYPE;
    break;
  case STRING_EXPRESSION:
    expression->type = STRING_TYPE;
    break;
  case AGGREGATE_EXPRESSION:
    analyseAggregateExpression(expression, parameters);
    break;
  case RANDOM_EXPRESSION:
    analyseRandomExpression(expression, parameters);
    break;
  case WHAT_EXPRESSION:
    analyseWhatExpression(expression, parameters);
    break;
  default:
    syserr("Unrecognized switch in analyseExpression()");
    break;
  }
}



/*======================================================================

  dumpType()

  Dump a type.

  */
#ifdef _PROTOTYPES_
void dumpType(TypeKind type)
#else
void dumpType(type)
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

#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
/*----------------------------------------------------------------------*\

				EXPRESSION.H

			   Expression Nodes

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "types.h"

#include "Srcp.h"
#include "List.h"
#include "What.h"
#include "Where.h"
#include "Id.h"


/* Types: */

typedef enum TypeKind {		/* TYPE kinds */
  ERROR_TYPE,
  INTEGER_TYPE,
  BOOLEAN_TYPE,
  STRING_TYPE,
  INSTANCE_TYPE,
  UNKNOWN_TYPE
} TypeKind;

typedef enum ExpressionKind {		/* EXPRESSION kinds */
  WHERE_EXPRESSION,
  ATTRIBUTE_EXPRESSION,
  BINARY_EXPRESSION,
  INTEGER_EXPRESSION,
  STRING_EXPRESSION,
  AGGREGATE_EXPRESSION,
  RANDOM_EXPRESSION,
  SCORE_EXPRESSION,
  WHAT_EXPRESSION
} ExpressionKind;

typedef enum OperatorKind {	/* OPERATOR kinds */
  PLUS_OPERATOR,
  MINUS_OPERATOR,
  MULT_OPERATOR,
  DIV_OPERATOR,
  AND_OPERATOR,
  OR_OPERATOR,
  NE_OPERATOR,			/* equal Not */
  EQ_OPERATOR,			/* Equal */
  EXACT_OPERATOR,		/* Exact comparison on strings */
  LE_OPERATOR,			/* Less or equal */
  GE_OPERATOR,			/* Greater or equal */
  LT_OPERATOR,			/* Less than */
  GT_OPERATOR			/* Greater than */
} OperatorKind;


typedef enum AggregateKind {
  SUM_AGGREGATE,
  MAX_AGGREGATE,
  COUNT_AGGREGATE
} AggregateKind;


typedef struct Expression {		/* EXPRESSION */
  Srcp srcp;			/* Source position of the expression */
  ExpressionKind kind;		/* Class of the expression */
  TypeKind type;		/* Type of the expression */
  Bool not;			/* Was there a NOT ? */
  union {

    struct {			/* for WHERE */
      struct Expression *what;	/* Must be a WHAT */
      Where *where;
    } where;

    struct {			/* for ATTRIBUTE */
      struct Expression *what;	/* What is? */
      Id *attribute;		/* Name of the attribute */
    } attribute;

    struct {			/* for BINARY */
      OperatorKind operator;	/* Operator */
      struct Expression *right;	/* Right operand */
      struct Expression *left;	/* Left operand */
    } binary;

    struct {			/* For VALUE */
      int value;		/* Value */
    } value;

    struct {			/* For STRING */
      long fpos;
      int len;
    } string;

    struct {			/* For AGGREGATE */
      AggregateKind kind;	/* Kind of aggregate */
      Id *attribute;		/* Attribute name */
      Where *where;		/* Where to aggregate */
    } aggregate;

    struct {			/* For RANDOM */
      int from;
      int to;
    } random;

    struct {			/* For WHAT */
      What *what;
    } what;

  } fields;
} Expression;



/* Data: */



/* Functions: */

#ifdef _PROTOTYPES_

/* Create a new Expression node */
extern Expression *newExpression(Srcp *srcp,
				 ExpressionKind kind);

/* Analyze an Expression */
extern void analyseExpression(Expression *expression,
			      List *params);

/* Compare two types */
extern Bool equalTypes(TypeKind type1,
		       TypeKind type2);

/* Generate code for an Expression */
extern void generateExpression(Expression *expression);

/* Dump a Type */
extern void dumpType(TypeKind type);

/* Dump an Expression */
extern void dumpExpression(Expression *expression);

#else
extern Expression *newExpression();
extern Bool equalTypes();
extern void analyseExpression();
extern void generateExpression();
extern void dumpTypes();
extern void dumpExpression();
#endif

#endif

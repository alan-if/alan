#ifndef _EXP_H_
#define _EXP_H_
/*----------------------------------------------------------------------*\

				EXP.H
			   Expression Nodes

\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "alan.h"
#include "srcp.h"
#include "wht.h"
#include "whr.h"
#include "id.h"
#include "type.h"
#include "lst.h"


/* TYPES: */

typedef enum ExpressionKind {		/* EXPRESSION kinds */
  WHERE_EXPRESSION,
  ATTRIBUTE_EXPRESSION,
  BINARY_EXPRESSION,
  INTEGER_EXPRESSION,
  STRING_EXPRESSION,
  AGGREGATE_EXPRESSION,
  RANDOM_EXPRESSION,
  SCORE_EXPRESSION,
  WHAT_EXPRESSION,
  BETWEEN_EXPRESSION,
  ISA_EXPRESSION
} ExpressionKind;

typedef enum OperatorKind {		/* OPERATOR kinds */
  PLUS_OPERATOR,
  MINUS_OPERATOR,
  MULT_OPERATOR,
  DIV_OPERATOR,
  AND_OPERATOR,
  OR_OPERATOR,
  NE_OPERATOR,			/* Not equal */
  EQ_OPERATOR,			/* Equal */
  EXACT_OPERATOR,		/* Exact comparison on strings */
  LE_OPERATOR,			/* Less or equal */
  GE_OPERATOR,			/* Greater or equal */
  LT_OPERATOR,			/* Less than */
  GT_OPERATOR,			/* Greater than */
  CONTAINS_OPERATOR		/* String contains substring */
} OperatorKind;


typedef enum AggregateKind {
  SUM_AGGREGATE,
  MAX_AGGREGATE,
  MIN_AGGREGATE,
  COUNT_AGGREGATE
} AggregateKind;


typedef struct Expression {
  Srcp srcp;			/* Source position of the expression */
  ExpressionKind kind;		/* What kind of expression */
  TypeKind type;		/* Type of the expression */
  Symbol *class;		/* For instance types, the class */
  Bool not;			/* Was there a NOT ? */
  union {

    struct {			/* for WHERE */
      struct Expression *wht;	/* Must be a WHAT */
      Where *whr;
    } whr;

    struct {			/* for ATTRIBUTE */
      struct Expression *wht;	/* Attribute of what? */
      struct IdNode *id;	/* Id of the attribute */
      struct Attribute *atr;	/* The attribute node */
    } atr;

    struct {			/* for BINARY */
      OperatorKind op;		/* Operator */
      struct Expression *right;	/* Right operand */
      struct Expression *left;	/* Left operand */
    } bin;

    struct {			/* For VALUE */
      int val;			/* Value */
    } val;

    struct {			/* For STRING */
      Bool encoded;
      long fpos;
      long len;
    } str;

    struct {			/* For AGGREGATE */
      AggregateKind kind;	/* Kind of aggregate */
      struct IdNode *attribute;	/* Attribute id */
      struct List *filters;	/* Expressions to filter against */
    } agr;

    struct {			/* For RANDOM */
      struct Expression *from;
      struct Expression *to;
    } rnd;

    struct {			/* For WHAT */
      What *wht;
    } wht;

    struct {			/* For BETWEEN */
      struct Expression *val;	/* The value */
      struct Expression *low;	/* The boundries */
      struct Expression *high;
    } btw;

    struct {			/* For CONTAINS */
      struct Expression *str;	/* The string to investigate */
      struct Expression *sub;	/* The substring to find */
    } cnt;

    struct {			/* For ISA */
      struct Expression *what;	/* The entity/parameter/what */
      struct IdNode *class;	/* The class id */
    } isa;

  } fields;
} Expression;


#endif

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
  COUNT_AGGREGATE
} AggregateKind;


typedef struct ExpNod {		/* EXPRESSION */
  Srcp srcp;			/* Source position of the expression */
  ExpressionKind kind;		/* What kind of expression */
  TypeKind type;		/* Type of the expression */
  Bool not;			/* Was there a NOT ? */
  union {

    struct {			/* for WHERE */
      struct ExpNod *wht;	/* Must be a WHAT */
      WhrNod *whr;
    } whr;

    struct {			/* for ATTRIBUTE */
      struct ExpNod *wht;	/* What is? */
      struct IdNode *atr;	/* Id of the attribute */
    } atr;

    struct {			/* for BINARY */
      OperatorKind op;		/* Operator */
      struct ExpNod *right;	/* Right operand */
      struct ExpNod *left;	/* Left operand */
    } bin;

    struct {			/* For VALUE */
      int val;			/* Value */
    } val;

    struct {			/* For STRING */
      long fpos;
      int len;
    } str;

    struct {			/* For AGGREGATE */
      AggregateKind kind;	/* Kind of aggregate */
      struct IdNode *atr;	/* Attribute id */
      struct WhrNod *whr;	/* Where to aggregate */
    } agr;

    struct {			/* For RANDOM */
      struct ExpNod *from;
      struct ExpNod *to;
    } rnd;

    struct {			/* For WHAT */
      WhtNod *wht;
    } wht;

    struct {			/* For BETWEEN */
      struct ExpNod *val;	/* The value */
      struct ExpNod *low;	/* The boundries */
      struct ExpNod *high;
    } btw;

    struct {			/* For CONTAINS */
      struct ExpNod *str;	/* The string to investigate */
      struct ExpNod *sub;	/* The substring to find */
    } cnt;

    struct {			/* For ISA */
      struct ExpNod *right;	/* The entity/parameter */
      struct IdNode *id;	/* The class id */
    } isa;

  } fields;
} ExpNod;


#endif

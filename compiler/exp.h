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
#include "evt.h"
#include "nam.h"


/* Types: */

typedef enum TypeKind {		/* TYPE kinds */
  TYPUNK,
  TYPINT,
  TYPBOOL,
  TYPSTR,
  TYPENT,			/* Entities */
  TYPERR
} TypeKind;

typedef enum ExpKind {		/* EXPRESSION kinds */
  EXPWHR,
  EXPATR,
  EXPBIN,
  EXPINT,
  EXPSTR,
  EXPAGR,
  EXPRND,
  EXPSCORE,
  EXPWHT,
  EXPBTW
} ExpKind;

typedef enum OpKind {		/* OPERATOR kinds */
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_AND,
  OP_OR,
  OP_NE,			/* Not equal */
  OP_EQ,			/* Equal */
  OP_EXACT,			/* Exact comparison on strings */
  OP_LE,			/* Less or equal */
  OP_GE,			/* Greater or equal */
  OP_LT,			/* Less than */
  OP_GT				/* Greater than */
} OpKind;


typedef enum AgrKind {
  AGR_SUM,
  AGR_MAX,
  AGR_COUNT
} AgrKind;


typedef struct ExpNod {		/* EXPRESSION */
  Srcp srcp;			/* Source position of the expression */
  ExpKind class;		/* Class of the expression */
  TypeKind typ;			/* Type of the expression */
  Bool not;			/* Was there a NOT ? */
  union {

    struct {			/* for WHERE */
      struct ExpNod *wht;	/* Must be a WHAT */
      WhrNod *whr;
    } whr;

    struct {			/* for ATTRIBUTE */
      struct ExpNod *wht;	/* What is? */
      NamNod *atr;		/* Name of the attribute */
    } atr;

    struct {			/* for BINARY */
      OpKind op;		/* Operator */
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
      AgrKind agr;		/* Kind of aggregate */
      NamNod *atrnam;		/* Attribute name */
      WhrNod *whr;		/* Where to aggregate */
    } agr;

    struct {			/* For RANDOM */
      int from;
      int to;
    } rnd;

    struct {			/* For WHAT */
      WhtNod *wht;
    } wht;

    struct {			/* For BETWEEN */
      struct ExpNod *val;
      struct ExpNod *low;
      struct ExpNod *high;
    } btw;

  } fields;
} ExpNod;



/* Data: */



/* Functions: */


/* Create a new Expression node */
extern ExpNod *newexp(Srcp *srcp,
			     ExpKind class);

/* Analyze an Expression */
extern void anexp(ExpNod *exp, EvtNod *evt, List *params);

/* Compare two types */
extern Bool eqtyp(TypeKind typ1,
		  TypeKind typ2);

/* Generate code for an Expression */
extern void geexp(ExpNod *exp);

/* Dump a Type */
extern void dutyp(TypeKind typ);

/* Dump an Expression */
extern void duexp(ExpNod *exp);


#endif

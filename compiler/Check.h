#ifndef _CHECK_H_
#define _CHECK_H_
/*----------------------------------------------------------------------*\

			       Check.h

			     Check Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"

#include "Expression.h"
#include "List.h"


/* Types: */

typedef struct Check {		/* CHECK */
  Expression *expression;	/* Expression to check */
  Aaddr expadr;			/* ACODE address to code for expression */
  List *statements;		/* Statements for failing check */
  Aaddr stmadr;			/* ACODE address to statement code */
} Check;


/* Data: */


/* Functions: */

#ifdef _PROTOTYPES_

/* Create a new Check node */
extern Check *newCheck(Expression *expression, List *statements);

/* Analyze a list of Checks */
extern void analyseChecks(List *checks, List *parameters);

/* Generate code for a list of Checks */
extern Aaddr generateChecks(List *checks);

/* Dump a Check node */
extern void dumpCheck(Check *check);

#else
extern Check *newCheck();
extern void analyseChecks();
extern Aaddr generateChecks();
extern void dumpCheck();
#endif

#endif

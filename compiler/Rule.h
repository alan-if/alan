#ifndef _RULE_H_
#define _RULE_H_
/*----------------------------------------------------------------------*\

				Rule.h

			      Rule Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "Expression.h"
#include "List.h"

#include "acode.h"


/* Types: */

typedef struct Rule {		/* RULE */
  Srcp srcp;			/* Source position */
  Expression *expression;	/* Expression */
  Aaddr expadr;			/* ACODE address to expression code */
  List *statements;		/* Statements */
  Aaddr stmadr;			/* ACODE address to statements */
} Rule;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Allocate a new Rule node */
extern Rule *newRule(Srcp *srcp,
		     Expression *expression,
		     List *statements);

/* Analyse all Rules */
extern void analyseRule(Rule *rule);

/* Generate code for all Rules */
extern Aaddr generateRules(void);

/* Dump a Rule node */
extern void dumpRule(Rule *rule);

#else
extern Rule *newRule();
extern void analyseRules();
extern Aaddr generateRules();
extern void dumpRule();
#endif

#endif

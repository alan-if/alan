/*----------------------------------------------------------------------*\

				Rule.c

			      Rule Nodes

\*----------------------------------------------------------------------*/


#include "Rule.h"

#include "Expression.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newRule()

  Allocates and initialises a Rule.

 */
#ifdef _PROTOTYPES_
Rule *newRule(Srcp *srcp,	/* IN - Source position */
	      Expression *expression, /* IN - Expression to evaluate */
	      List *statements) /* IN - Statements */
#else
Rule *newRule(srcp, expression, statements)
     Srcp *srcp;
     Expression *expression;
     List *statements;
#endif
{
  Rule *new;			/* The newly allocated area */

  new = NEW(Rule);

  new->srcp = *srcp;
  new->expression = expression;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseRule()

  Analyze a RULE.

 */
#ifdef _PROTOTYPES_
void analyseRule(Rule *rule, List *parameters)
#else
void analyseRule(rule, parameters)
     Rule *rule;
     List *parameters;
#endif
{
  analyseExpression(rule->expression, NULL);
  analyseStatements(rule->statements, NULL, NULL);
}



/*======================================================================

  dumpRule()

  Dump a Rule node.

 */
#ifdef _PROTOTYPES_
void dumpRule(Rule *rule)
#else
void dumpRule(rule)
     Rule *rule;
#endif
{
  if (rule == NULL) {
    put("NULL");
    return;
  }

  put("RULE: "); dumpSrcp(&rule->srcp); in();
  put("expression: "); dumpExpression(rule->expression); nl();
  put("statememts: "); dumpList(rule->statements, STATEMENT_NODE); out();
}

/*----------------------------------------------------------------------*\

				Check.c

			      Check Nodes

\*----------------------------------------------------------------------*/


#include "Check.h"

#include "Statement.h"

#include "dump.h"




/*======================================================================

  newCheck()

  Allocates and initialises a Check.

 */
#ifdef _PROTOTYPES_
Check *newCheck(Expression *expression,	/* IN - Expression to check */
		List *statements) /* IN - Statements to execute else*/
#else
Check *newCheck(srcp, expression, statements)
     Expression *expression;
     List *statements;
#endif
{
  Check *new;			/* The newly allocated area */

  new = NEW(Check);

  new->expression = expression;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseCheck()

  Analyze a CHECK.

 */
#ifdef _PROTOTYPES_
void analyseCheck(Check *check, List *parameters)
#else
void analyseCheck(check, parameters)
     Check *check;
     List *parameters;
#endif
{
  analyseExpression(check->expression, parameters);
  analyseStatements(check->statements, NULL, parameters);
}



/*======================================================================

  analyseChecks()

  Analyze a list of CHECKs.

 */
#ifdef _PROTOTYPES_
void analyseChecks(List *checks, List *parameters)
#else
void analyseChecks(checks, parameters)
     List *checks;
     List *parameters;
#endif
{
  List *list;

  for (list = checks; list; list = list->next)
    analyseCheck(list->element.check, parameters);
}



/*======================================================================

  dumpCheck()

  Dump a Check node.

 */
#ifdef _PROTOTYPES_
void dumpCheck(Check *check)
#else
void dumpCheck(check)
     Check *check;
#endif
{
  if (check == NULL) {
    put("NULL");
    return;
  }

  put("CHECK: "); in();
  put("expression: "); dumpExpression(check->expression); nl();
  put("statememts: "); dumpList(check->statements, STATEMENT_NODE); out();
}



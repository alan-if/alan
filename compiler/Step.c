/*----------------------------------------------------------------------*\

				Step.c

			      Step Nodes

\*----------------------------------------------------------------------*/


#include "Step.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newStep()

  Allocates and initialises a Step.

 */
#ifdef _PROTOTYPES_
extern Step *newStep(Srcp *srcp,
		     Expression *after,	/* IN - After how many steps */
		     Expression *expression, /* IN - Logical expression */
		     List *statements)	/* IN - What to do */
#else
Step *newStep(srcp, after, expression, statements)
     Srcp *srcp;
     Expression *after;
     Expression *expression;
     List *statements;
#endif
{
  Step *new;			/* The newly allocated area */

  new = NEW(Step);

  new->srcp = *srcp;
  new->after = after;
  new->expression = expression;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseStep()

  Analyze a STEP.

 */
#ifdef _PROTOTYPES_
void analyseSteps(List *steps)
#else
void analyseSteps(steps)
     List *steps;
#endif
{
  List *list;

  for (list = steps; list; list = list->next) {
    if (list->the.step->after)
      analyseExpression(list->the.step->after, NULL);
    if (list->the.step->expression)
      analyseExpression(list->the.step->expression, NULL);
    analyseStatements(list->the.step->statements, NULL, NULL);
  }
}



/*======================================================================

  dumpStep()

  Dump a Step node.

 */
#ifdef _PROTOTYPES_
void dumpStep(Step *step)
#else
void dumpStep(step)
     Step *step;
#endif
{
  if (step == NULL) {
    put("NULL");
    return;
  }

  put("STEP: "); dumpSrcp(&step->srcp); in();
  put("after: "); dumpExpression(step->after); nl();
  put("expression: "); dumpExpression(step->expression); nl();
  put("statememts: "); dumpList(step->statements, STATEMENT_NODE); out();
}

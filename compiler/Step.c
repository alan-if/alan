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
void analyseSteps(List *step)
#else
void analyseSteps(step)
     List *steps;
#endif
{
  /* 4f - Analyse steps */
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

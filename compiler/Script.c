/*----------------------------------------------------------------------*\

				Script.c

			      Script Nodes

\*----------------------------------------------------------------------*/


#include "Script.h"
#include "Statement.h"
#include "Step.h"

#include "dump.h"




/*======================================================================

  newScript()

  Allocates and initialises a Script.

 */
#ifdef _PROTOTYPES_
Script *newScript(Srcp *srcp,	/* IN - Source position */
		  Id *id,	/* IN - Name of Script */
		  List *description, /* IN - Description statements */
		  List *steps) /* IN - Steps */
#else
Script *newScript(srcp, id, description, steps)
     Srcp *srcp;
     Id *id;
     List *description;
     List *steps;
#endif
{
  Script *new;			/* The newly allocated area */

  new = NEW(Script);

  new->srcp = *srcp;
  new->id = id;
  new->code = 0;		/* Not numbered yet! */
  new->description = description;
  new->steps = steps;

  return(new);
}


/*======================================================================

  analyseScript()

  Analyze a SCRIPT.

 */
#ifdef _PROTOTYPES_
void analyseScript(Script *script)
#else
void analyseScript(script)
     Script *script;
#endif
{
  /* 4f - find out code for the script */
  analyseStatements(script->description, NULL, NULL);
  analyseSteps(script->steps);
}



/*======================================================================

  dumpScript()

  Dump a Script node.

 */
#ifdef _PROTOTYPES_
void dumpScript(Script *script)
#else
void dumpScript(script)
     Script *script;
#endif
{
  if (script == NULL) {
    put("NULL");
    return;
  }

  put("SCRIPT: "); dumpSrcp(&script->srcp); in();
  put("id: "); dumpId(script->id); nl();
  put("description: "); dumpList(script->description, STATEMENT_NODE); nl();
  put("steps: "); dumpList(script->steps, STEP_NODE); out();
}

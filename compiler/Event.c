/*----------------------------------------------------------------------*\

				Event.c

			      Event Nodes

\*----------------------------------------------------------------------*/


#include "Event.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newEvent()

  Allocates and initialises a Event.

 */
#ifdef _PROTOTYPES_
Event *newEvent(Srcp *srcp,	/* IN - Source position */
		Id *id,		/* IN - Name of Event */
		List *statements) /* IN - Statements */
#else
Event *newEvent(srcp, id, statements)
     Srcp *srcp;
     Id *id;
     List *statements;
#endif
{
  Event *new;			/* The newly allocated area */

  new = NEW(Event);

  new->srcp = *srcp;
  new->id = id;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseEvent()

  Analyze a EVENT.

 */
#ifdef _PROTOTYPES_
void analyseEvent(Event *event, List *parameters)
#else
void analyseEvent(event, parameters)
     Event *event;
     List *parameters;
#endif
{
  analyseStatements(event->statements, NULL, NULL);
}



/*======================================================================

  dumpEvent()

  Dump a Event node.

 */
#ifdef _PROTOTYPES_
void dumpEvent(Event *event)
#else
void dumpEvent(event)
     Event *event;
#endif
{
  if (event == NULL) {
    put("NULL");
    return;
  }

  put("EVENT: "); dumpSrcp(&event->srcp); in();
  put("id: "); dumpId(event->id); nl();
  put("statememts: "); dumpList(event->statements, STATEMENT_NODE); out();
}

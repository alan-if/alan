/*----------------------------------------------------------------------*\

				EVT.C
			     Event Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "adv_x.h"
#include "stm_x.h"
#include "context_x.h"

#include "lmList.h"

#include "evt.h"                /* EVT-nodes */
#include "opt.h"                /* Options */

#include "emit.h"
#include "../interpreter/acode.h"
#include "dump.h"


/* PUBLIC: */

int evtmin, evtmax;
int evtcount = 0;


/*======================================================================

  newevt()

  Allocates and initialises a new evtnod.

  */
EvtNod *newevt(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,	/* IN - Name of the event */
	       List *stms)	/* IN - Statements to execute */
{
  EvtNod *new;		/* The newly allocated node */

  showProgress();

  new = NEW(EvtNod);

  new->srcp = *srcp;
  new->id = id;
  new->stms = stms;
  
  new->id->symbol = newSymbol(id, EVENT_SYMBOL);

  return(new);
}



/*======================================================================

  anevts()

  Analyzes all events by calling the statment list analyzer for all
  events.

  */
void anevts(void)
{
    List *evts;		/* Traversal pointer */
    Context *context = newContext(EVENT_CONTEXT, 0);

    for (evts = adv.evts; evts != NULL; evts = evts->next) {
      if (verbose) {
	showProgress();
      }
      context->event = evts->element.evt;
      analyzeStatements(evts->element.evt->stms, context);
    }
}



/*----------------------------------------------------------------------

  generateEvent()

  Generate one event.

  */
static void generateEvent(EvtNod *evt)	/* IN - The event to generate */
{
  showProgress();

  if ((Bool) opts[OPTDEBUG].value) {
    evt->namadr = nextEmitAddress();
    emitString(evt->id->string);
  } else
    evt->namadr = 0;
  evt->stmadr = nextEmitAddress();
  generateStatements(evt->stms);
  emit0(I_RETURN);
}


/*======================================================================

  generateEvents()

  Generate the events.

  */
Aaddr generateEvents(AcdHdr *header)
{
  List *lst;	/* Traversal pointer */
  Aaddr adr;
  EventEntry entry;
  
  /* First all the events */
  for (lst = adv.evts; lst != NULL; lst = lst->next)
    generateEvent(lst->element.evt);

  adr = nextEmitAddress();		/* Save address of event table */
  for (lst = adv.evts; lst != NULL; lst = lst->next) {
    entry.stringAddress = lst->element.evt->namadr;
    entry.code = lst->element.evt->stmadr;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);

  header->eventMax = eventCount;

  return(adr);
}



/*======================================================================

  duevt()

  Dump an Event node.

  */
void duevt(EvtNod *evt)
{
  if (evt == NULL) {
    put("NULL");
    return;
  }

  put("EVT: "); dumpSrcp(&evt->srcp); indent();
  put("id: "); dumpId(evt->id); nl();
  put("stms: "); dumpList(evt->stms, STATEMENT_LIST); out();
}



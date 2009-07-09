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
#include "dump_x.h"

#include "lmList.h"

#include "evt.h"                /* EVT-nodes */
#include "opt.h"                /* Options */

#include "emit.h"
#include "acode.h"


/* PUBLIC: */

int evtmin, evtmax;
int evtcount = 0;


/*======================================================================*/
Event *newEvent(Srcp *srcp, IdNode *id, List *stms)
{
  Event *new;		/* The newly allocated node */

  progressCounter();

  new = NEW(Event);

  new->srcp = *srcp;
  new->id = id;
  new->stms = stms;

  new->id->symbol = newSymbol(id, EVENT_SYMBOL);

  return(new);
}



/*======================================================================*/
void analyzeEvents(void)
{
    List *evts;		/* Traversal pointer */
    Context *context = newEventContext();

    for (evts = adv.evts; evts != NULL; evts = evts->next) {
      progressCounter();
      context->event = evts->element.evt;
      analyzeStatements(evts->element.evt->stms, context);
    }
}



/*----------------------------------------------------------------------*/
static void generateEvent(Event *evt)	/* IN - The event to generate */
{
  progressCounter();

  if ((Bool) opts[OPTDEBUG].value) {
    evt->namadr = nextEmitAddress();
    emitString(evt->id->string);
  } else
    evt->namadr = 0;
  evt->stmadr = nextEmitAddress();
  generateStatements(evt->stms);
  emit0(I_RETURN);
}


/*======================================================================*/
Aaddr generateEvents(ACodeHeader *header)
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



/*======================================================================*/
void dumpEvent(Event *evt)
{
  if (evt == NULL) {
    put("NULL");
    return;
  }

  put("EVENT: "); dumpSrcp(evt->srcp); indent();
  put("id: "); dumpId(evt->id); nl();
  put("stms: "); dumpList(evt->stms, STATEMENT_LIST); out();
}



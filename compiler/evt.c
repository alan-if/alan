/*----------------------------------------------------------------------*\

				EVT.C
			     Event Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "evt.h"                /* EVT-nodes */
#include "opt.h"                /* Options */

#include "emit.h"
#include "acode.h"
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
  Symbol *sym;		/* Symbol table entry */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(EvtNod);

  new->srcp = *srcp;
  new->id = id;
  new->stms = stms;
  
  sym = lookup(id->string);
  if (sym == NULL)
#ifndef FIXME
    syserr("UNIMPL: newevt() - symbol code handling");
#else
    new->id->code = newsym(id->str, NAMEVT, new);
#endif
  else
    redefined(id, sym);

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
    Context context;

    context.kind = EVENT_CONTEXT;
    for (evts = adv.evts; evts != NULL; evts = evts->next) {
      if (verbose) {
	printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout);
      }
      context.event = evts->element.evt;
      anstms(evts->element.evt->stms, &context);
    }
}



/*----------------------------------------------------------------------

  geevt()

  Generate one event.

  */
static void geevt(EvtNod *evt)	/* IN - The event to generate */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if ((Bool) opts[OPTDEBUG].value) {
    evt->namadr = emadr();
    emitstr(evt->id->string);
  } else
    evt->namadr = 0;
  evt->stmadr = emadr();
  gestms(evt->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}


/*======================================================================

  geevts()

  Generate the events.

  */
Aaddr geevts(void)
{
  List *lst;	/* Traversal pointer */
  Aaddr adr;
  
  /* First all the events */
  for (lst = adv.evts; lst != NULL; lst = lst->next)
    geevt(lst->element.evt);
  
  adr = emadr();		/* Save address of event table */
  for (lst = adv.evts; lst != NULL; lst = lst->next) {
    emit(lst->element.evt->namadr);
    emit(lst->element.evt->stmadr);
  }
  emit(EOF);
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

  put("EVT: "); dumpSrcp(&evt->srcp); in();
  put("id: "); dumpId(evt->id); nl();
  put("stms: "); dulst(evt->stms, LIST_STM); out();
}



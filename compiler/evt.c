/*----------------------------------------------------------------------*\

				EVT.C
			     Event Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"		/* STM-nodes */
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
	       NamNod *nam,	/* IN - Name of the event */
	       List *stms)	/* IN - Statements to execute */
{
  EvtNod *new;		/* The newly allocated node */
  SymNod *sym;		/* Symbol table entry */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(EvtNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->stms = stms;
  
  sym = lookup(nam->str);
  if (sym == NULL)
    new->nam->code = newsym(nam->str, NAMEVT, new);
  else
    redefined(srcp, sym, nam->str);

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

    for (evts = adv.evts; evts != NULL; evts = evts->next) {
      if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }
      anstms(evts->element.evt->stms, NULL, evts->element.evt, NULL);
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
    emitstr(evt->nam->str);
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

  put("EVT: "); dusrcp(&evt->srcp); in();
  put("nam: "); dunam(evt->nam); nl();
  put("stms: ");dulst(evt->stms, STMNOD); out();
}



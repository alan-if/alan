/*----------------------------------------------------------------------*\

                EVT.C
                 Event Nodes

\*----------------------------------------------------------------------*/

#include "evt.h"

#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "adv_x.h"
#include "stm_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "opt.h"                /* Options */

#include "emit.h"
#include "acode.h"


/* PUBLIC: */

int evtmin, evtmax;
int evtcount = 0;


/*======================================================================*/
Event *newEvent(Srcp *srcp, Id *id, List *stms)
{
    Event *new;		/* The newly allocated node */

    progressCounter();

    new = NEW(Event);

    new->srcp = *srcp;
    new->id = id;
    new->stms = stms;

    new->id->symbol = newSymbol(id, EVENT_SYMBOL);

    return new;
}



/*======================================================================*/
void analyzeEvents(void)
{
    Context *context = newEventContext();

    for (List *evts = adv.evts; evts != NULL; evts = evts->next) {
        progressCounter();
        context->event = evts->member.evt;
        analyzeStatements(evts->member.evt->stms, context);
    }
}



/*----------------------------------------------------------------------*/
static void generateEventData(Event *evt)	/* IN - The event to generate */
{
    progressCounter();

    if ((bool) opts[OPTDEBUG].value) {
        evt->nameAddress = nextEmitAddress();
        emitString(evt->id->string);
    } else
        evt->nameAddress = 0;
    evt->stmadr = nextEmitAddress();
    generateStatements(evt->stms);
    emit0(I_RETURN);
}


/*======================================================================*/
static Aaddr generateEventTable(void) {
    Aaddr adr;
    EventEntry entry;

    adr = nextEmitAddress();		/* Save address of event table */
    for (List *lst = adv.evts; lst != NULL; lst = lst->next) {
        entry.id = lst->member.evt->nameAddress;
        entry.code = lst->member.evt->stmadr;
        emitEntry(&entry, sizeof(entry));
    }
    emit(EOF);
    return adr;
}


/*======================================================================*/
Aaddr generateEvents(ACodeHeader *header)
{
    /* First all the events */
    for (List *lst = adv.evts; lst != NULL; lst = lst->next)
        generateEventData(lst->member.evt);

    header->eventMax = eventCount;
    return generateEventTable();
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

/*----------------------------------------------------------------------*\

  SLT.C
  Slot Nodes

\*----------------------------------------------------------------------*/

#include "slt.h"

/* IMPORT */
#include <stdio.h>
#include "util.h"
#include "dump.h"




/*======================================================================

  newslt()

  Allocates and initialises a sltnod.

  */
Slots *newSlots(List *nams,
		WhrNod *whr,
		List *atrs,
		CntNod *cnt,
		List *surr,
		List *dscr,
		List *ment,
		List *art,
		List *does,
		List *exts,
		List *vrbs,
		List *scrs)
{
  Slots *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(Slots);

  new->namslst = nams;
  new->whr = whr;
  new->atrs = atrs;
  new->cnt = cnt;
  new->dscr = dscr;
  new->ment = ment;
  new->art = art;
  new->vrbs = vrbs;

  return(new);
}


/*======================================================================

  dumpSlots()

  Dump a Slots node.

 */
void dumpSlots(Slots *slots)
{
  put("SLOTS: "); in();
  put("namslst: "); dulst(slots->namslst, LIST_NAM); nl();
  put("namstms: "); dulst(slots->namstms, LIST_STM); nl();
  put("namadr: "); duadr(slots->namsadr); nl();
#ifdef FIXME
  put("whr: "); duwhr(slots->whr); nl();
  put("cnt: "); ducnt(slots->cnt); nl();
#endif
  put("atrs: "); dulst(slots->atrs, LIST_ATR); nl();
  put("atradr: "); duadr(slots->atradr); nl();
  put("dscr: "); dulst(slots->dscr, LIST_STM); nl();
  put("dscradr: "); duadr(slots->dscradr); nl();
  put("art: "); dulst(slots->art, LIST_STM); nl();
  put("artadr: "); duadr(slots->artadr); nl();
  put("ment: "); dulst(slots->ment, LIST_STM); nl();
  put("mentadr: "); duadr(slots->mentadr); nl();
  put("vrbs: "); dulst(slots->vrbs, LIST_VRB); nl();
  put("vrbadr: "); duadr(slots->vrbadr); out();
}

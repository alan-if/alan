/*----------------------------------------------------------------------*\

  SLT.C
  Slot Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "slt.h"



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
  put("namslst: "); dulst(slots->namslst, NAMNOD); nl();
  put("namstms: "); dulst(slots->namstms, STMNOD); nl();
  put("namadr: "); duadr(slots->namsadr); nl();
  put("whr: "); duwhr(slots->whr); nl();
  put("cnt: "); ducnt(slots->cnt); nl();
  put("atrs: "); dulst(slots->atrs, ATRNOD); nl();
  put("atradr: "); duadr(slots->atradr); nl();
  put("dscr: "); dulst(slots->dscr, STMNOD); nl();
  put("dscradr: "); duadr(slots->dscradr); nl();
  put("art: "); dulst(slots->art, STMNOD); nl();
  put("artadr: "); duadr(slots->artadr); nl();
  put("ment: "); dulst(slots->ment, STMNOD); nl();
  put("mentadr: "); duadr(slots->mentadr); nl();
  put("vrbs: "); dulst(slots->vrbs, VRBNOD); nl();
  put("vrbadr: "); duadr(slots->vrbadr); out();
}

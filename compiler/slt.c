/*----------------------------------------------------------------------*\

				SLT.C
			      Slot Nodes

\*----------------------------------------------------------------------*/

#include "slt_x.h"


/* IMPORT */
#include <stdio.h>
#include "util.h"
#include "dump.h"
#include "emit.h"

#include "whr_x.h"
#include "cnt_x.h"


/*======================================================================

  newslt()

  Allocates and initialises a sltnod.

  */
SlotsNode *newSlots(List *nams,
		    WhrNod *whr,
		    List *atrs,
		    CntNod *cnt,
		    List *dscr,
		    List *ment,
		    List *art,
		    List *does,
		    List *exts,
		    List *vrbs,
		    List *scrs)
{
  SlotsNode *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(SlotsNode);

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

  generateSlotsData()

  Generate data for one Slots node.

 */
void generateSlotsData(SlotsNode *slots)
{
}


/*======================================================================

  generateSlotsEntry()

  Generate entries for one Slots node.

 */
void generateSlotsEntry(SlotsNode *slots)
{
  emit(slots->namsadr);		/* names */
  emit(slots->whrCode);
  emit(slots->atradr);		/* attributes */
  emit(slots->dscradr);		/* description */
  emit(slots->mentadr);		/* mentioned */
  emit(slots->artadr);		/* article */
  emit(slots->extadr);		/* exits */
  emit(slots->vrbadr);		/* verbs */
}


/*======================================================================

  dumpSlots()

  Dump a Slots node.

 */
void dumpSlots(SlotsNode *slots)
{
  put("SLOTS: "); in();
  put("namslst: "); dulst(slots->namslst, LIST_NAM); nl();
  put("namstms: "); dulst(slots->namstms, LIST_STM); nl();
  put("namadr: "); duadr(slots->namsadr); nl();
  put("whr: "); duwhr(slots->whr); nl();
#ifdef FIXME
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

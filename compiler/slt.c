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



/*----------------------------------------------------------------------*\

  CLA.C
  Class Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "cla.h"



/*======================================================================

  newcla()

  Allocates and initialises a clanod.

  */
ClaNod *newcla(Srcp *srcp,	/* IN - Source Position */
	       NamNod *id,
	       List *heritage,
	       Slots *slt)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  new->id = id;
  new->heritage = heritage;
  new->slt = slt;

  return(new);
}


/*======================================================================

  dumpClass()

  Dump a Class node.

 */
void dumpClass(ClaNod *cla)
{
  put("CLA: "); dusrcp(&cla->srcp); in();
  put("id: "); dunam(cla->id); nl();
  put("heritage: "); dulst(cla->heritage, NAMNOD); nl();
  put("slots: "); dumpSlots(cla->slt); out();
}

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
	       NamNod *nam,
	       List *heritage,
	       Slots *slt)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->heritage = heritage;
  new->slt = slt;

  return(new);
}



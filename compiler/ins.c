/*----------------------------------------------------------------------*\

  INS.C
  Instance Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "ins.h"



/*======================================================================

  newins()

  Allocates and initialises a insnod.

  */
InsNod *newins(Srcp *srcp,	/* IN - Source Position */
	       NamNod *nam,
	       List *heritage,
	       Slots *ins)
{
  InsNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(InsNod);

  new->srcp = *srcp;

  return(new);
}



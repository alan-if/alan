/*----------------------------------------------------------------------*\

  INS.C
  Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"

#include "srcp.h"
#include "lmList.h"




/*======================================================================

  newins()

  Allocates and initialises a insnod.

  */
InsNod *newins(Srcp *srcp,	/* IN - Source Position */
	       IdNod *id,
	       List *heritage,
	       Slots *slt)
{
  InsNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(InsNod);

  new->srcp = *srcp;
  new->id = id;
  new->heritage = heritage;
  new->slt = slt;

  return(new);
}


/*======================================================================

  dumpInstance()

  Dump a Instance node.

 */
void dumpInstance(InsNod *ins)
{
  put("INS: "); dusrcp(&ins->srcp); in();
  put("id: "); dumpId(ins->id); nl();
  put("heritage: "); dulst(ins->heritage, LIST_NAM); nl();
  put("slots: "); dumpSlots(ins->slt); nl();
}

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
	       IdNod *heritage,
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

  analyzeInstances()

  Analyze all Class nodes.

 */
void analyzeInstances(void)
{
  syserr("UNIMPL: analyzeInstances");
}


/*======================================================================

  generateInstances()

  Generate all Class nodes.

 */
Aaddr generateInstances(void)
{
  syserr("UNIMPL: generateInstances");
}


/*======================================================================

  dumpInstance()

  Dump a Instance node.

 */
void dumpInstance(InsNod *ins)
{
  put("INS: "); dusrcp(&ins->srcp); in();
  put("id: "); dumpId(ins->id); nl();
  put("heritage: "); dumpId(ins->heritage); nl();
  put("slots: "); dumpSlots(ins->slt); nl();
}

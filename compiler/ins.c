/*----------------------------------------------------------------------*\

  INS.C
  Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"

#include "srcp_x.h"
#include "lmList.h"


static List *allClasses = NULL;


/*======================================================================

  newins()

  Allocates and initialises a insnod.

  */
InsNod *newins(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,
	       IdNode *heritage,
	       Slots *slt)
{
  InsNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(InsNod);

  new->srcp = *srcp;
  new->id = id;
  new->heritage = heritage;
  new->slt = slt;

  allClasses = concat(allClasses, new, LIST_INS);

  return(new);
}


/*----------------------------------------------------------------------

  analyzeInstance()

  Analyze a Instance node.

 */
static void analyzeInstance(InsNod *ins)
{
}


/*======================================================================

  analyzeInstances()

  Analyze all Class nodes.

 */
void analyzeInstances(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    analyzeInstance(l->element.ins);
}


/*======================================================================

  generateInstances()

  Generate all Class nodes.

 */
Aaddr generateInstances(void)
{
  syserr("UNIMPL: generateInstances");
  return 0;
}


/*======================================================================

  dumpInstance()

  Dump a Instance node.

 */
void dumpInstance(InsNod *ins)
{
  put("INS: "); dumpSrcp(&ins->srcp); in();
  put("id: "); dumpId(ins->id); nl();
  put("heritage: "); dumpId(ins->heritage); nl();
  put("slots: "); dumpSlots(ins->slt); nl();
}

/*----------------------------------------------------------------------*\

  INS.C
  Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"

#include "sym_x.h"
#include "id_x.h"
#include "srcp_x.h"
#include "lmList.h"


static List *allInstances = NULL;


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

  new->symbol = newsym(id->string, INSTANCE_SYMBOL);

  allInstances = concat(allInstances, new, LIST_INS);

  return(new);
}


/*----------------------------------------------------------------------

  symbolizeInstance()

  Symbolize a Instance node.

 */
static void symbolizeInstance(InsNod *ins)
{
  SymNod *heritage;

  if (ins->heritage != NULL) {
    heritage = lookup(ins->heritage->string);
    if (heritage == NULL)
      lmLog(&ins->heritage->srcp, 310, sevERR, ins->heritage->string);
    else if (heritage->kind != CLASS_SYMBOL)
      lmLog(&ins->heritage->srcp, 350, sevERR, "");
    else {
      ins->heritage->symbol = heritage;
      setParent(ins->symbol, ins->heritage->symbol);
    }
  }
}


/*======================================================================

  symbolizeInstances()

  Symbolize all Instance nodes.

 */
void symbolizeInstances(void)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    symbolizeInstance(l->element.ins);
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

  Analyze all Instance nodes.

 */
void analyzeInstances(void)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    analyzeInstance(l->element.ins);
}


/*======================================================================

  generateInstances()

  Generate all Instance nodes.

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

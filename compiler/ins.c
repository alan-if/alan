/*----------------------------------------------------------------------*\

				INS.C
			    Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"
#include "emit.h"

#include "sym_x.h"
#include "slt_x.h"
#include "id_x.h"
#include "srcp_x.h"

#include "lmList.h"


InsNod *theHero;


static List *allInstances = NULL;


/*======================================================================

  initInstances()

*/
void initInstances()
{
  allInstances = NULL;

  theHero = newInstance(&nulsrcp, newId(&nulsrcp, "hero"),
			newId(&nulsrcp, "location"), NULL);
  
}


/*======================================================================

  newInstance()

  Allocates and initialises an instance node.

  */
InsNod *newInstance(Srcp *srcp,	/* IN - Source Position */
		    IdNode *id,
		    IdNode *parent,
		    SlotsNode *slt)
{
  InsNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(InsNod);

  new->srcp = *srcp;
  new->id = id;
  new->parent = parent;
  if (slt)
    new->slt = slt;
  else
    new->slt = newSlots(NULL, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, NULL);

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
  SymNod *parent;

  if (ins->parent != NULL) {
    parent = lookup(ins->parent->string);
    if (parent == NULL)
      lmLog(&ins->parent->srcp, 310, sevERR, ins->parent->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&ins->parent->srcp, 350, sevERR, "");
    else {
      ins->parent->symbol = parent;
      setParent(ins->symbol, ins->parent->symbol);
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


/*----------------------------------------------------------------------

  generateInstanceData

  Generate the data parts for one instance.

*/
static void generateInstanceData(InsNod *ins)
{
  ins->idAddr = emadr();
  emitstr(ins->id->string);

  generateSlotsData(ins->slt);
}


/*----------------------------------------------------------------------

  generateInstanceEntry

*/
static void generateInstanceEntry(InsNod *ins)
{
  emit(ins->symbol->code);	/* First own code */
  emit(ins->idAddr);		/* Address to the id string */
  if (ins->parent == NULL)	/* Then parents */
    emit(0);
  else
    emit(ins->parent->symbol->code);

  generateSlotsEntry(ins->slt);
}


/*======================================================================

  generateInstances()

  Generate all Instance nodes.

 */
Aaddr generateInstances(void)
{
  List *l;
  Aaddr adr;

  for (l = allInstances; l; l = l->next)
    generateInstanceData(l->element.ins);

  adr = emadr();
  for (l = allInstances; l; l = l->next)
    generateInstanceEntry(l->element.ins);

  return (adr);
}



/*======================================================================

  dumpInstance()

  Dump a Instance node.

 */
void dumpInstance(InsNod *ins)
{
  put("INS: "); dumpSrcp(&ins->srcp); in();
  put("id: "); dumpId(ins->id); nl();
  put("parent: "); dumpId(ins->parent); nl();
  put("slots: "); dumpSlots(ins->slt); nl();
}

/*----------------------------------------------------------------------*\

				INS.C
			    Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "sym_x.h"
#include "slt_x.h"
#include "id_x.h"
#include "srcp_x.h"
#include "lst_x.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"
#include "emit.h"
#include "adv.h"

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
			newId(&nulsrcp, "actor"), NULL);
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
  if (slt)
    new->slots = slt;
  else
    new->slots = newEmptySlots();
  new->slots->id = id;
  new->slots->parent = parent;

  new->slots->symbol = newsym(id->string, INSTANCE_SYMBOL);
  new->slots->id->code = new->slots->symbol->code;
  new->slots->symbol->fields.claOrIns.attributes = new->slots->attributes;

  allInstances = concat(allInstances, new, LIST_INS);

  return(new);
}



/*----------------------------------------------------------------------

  symbolizeInstance()

  Symbolize a Instance node.

 */
static void symbolizeInstance(InsNod *ins)
{
  symbolizeSlots(ins->slots);
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
  analyzeSlots(ins->slots);
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
  generateSlotsData(ins->slots);
}


/*----------------------------------------------------------------------

  generateInstanceEntry

*/
static void generateInstanceEntry(InsNod *ins)
{
  InstanceEntry entry;

  generateSlotsEntry(&entry, ins->slots);
  emitN((Aword *)&entry, sizeof(entry)/sizeof(Aword));
}


/*----------------------------------------------------------------------

  generateInstanceTable()

*/
static Aaddr generateInstanceTable(void)
{
  Aaddr address = emadr();
  List *l;

  for (l = allInstances; l; l = l->next)
    generateInstanceEntry(l->element.ins);
  emit(EOF);
  return address;
}


/*======================================================================

  generateInstances()

  Generate all Instance nodes.

 */
void generateInstances(AcdHdr *header)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    generateInstanceData(l->element.ins);

  header->instanceTableAddress = generateInstanceTable();

  header->instanceMax = instanceCount;
  header->theHero = theHero->slots->symbol->code;
}



/*======================================================================

  dumpInstance()

  Dump a Instance node.

 */
void dumpInstance(InsNod *ins)
{
  put("INS: "); dumpSrcp(&ins->srcp); in();
  put("slots: "); dumpSlots(ins->slots); out();
}

/*----------------------------------------------------------------------*\

				INS.C
			    Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "id_x.h"
#include "lst_x.h"
#include "scr_x.h"
#include "slt_x.h"
#include "srcp_x.h"
#include "sym_x.h"
#include "wrd_x.h"
#include "context_x.h"

#include "sysdep.h"
#include "util.h"
#include "dump.h"
#include "emit.h"
#include "adv.h"

#include "lmList.h"


static List *allInstances = NULL;


/*======================================================================

  initInstances()

*/
void initInstances()
{
  allInstances = NULL;
}


/*======================================================================

  addHero()

*/
void addHero(void)
{
  Symbol *hero = lookup("hero");
  InsNod *theHeroInstance;

  if (hero == NULL) {
    theHeroInstance = newInstance(&nulsrcp, newId(&nulsrcp, "hero"),
				  newId(&nulsrcp, "actor"), NULL);
    theHero = theHeroInstance->slots->id->symbol;
  } else
    theHero = hero;
}


/*======================================================================

  newInstance()

  Allocates and initialises an instance node.

  */
InsNod *newInstance(Srcp *srcp,	/* IN - Source Position */
		    IdNode *id,
		    IdNode *parent,
		    Slots *slt)
{
  InsNod *new;                  /* The newly allocated area */
  List *nameList, *list;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(InsNod);

  new->srcp = *srcp;
  if (slt)
    new->slots = slt;
  else
    new->slots = newEmptySlots();
  new->slots->id = id;
  new->slots->parentId = parent;

  new->slots->id->symbol = newSymbol(id, INSTANCE_SYMBOL);
  new->slots->id->symbol->fields.claOrIns.slots = new->slots;

  allInstances = concat(allInstances, new, LIST_INS);

  /* Note instance name in the dictionary */
  if (new->slots->names == NULL)		/* Use the object name */
    newwrd(new->slots->id->string, WRD_NOUN, new->slots->id->code, new);
  else {
    for (nameList = new->slots->names; nameList != NULL; nameList = nameList->next) {
      for (list = nameList->element.lst; list->next != NULL; list = list->next)
	newwrd(list->element.id->string, WRD_ADJ, 0, new);
      newwrd(list->element.id->string, WRD_NOUN, list->element.id->code, new);
    }
  }

  prepareScripts(new->slots->scripts, new);

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
static void analyzeInstance(InsNod *instance)
{
  Context *context = newContext(INSTANCE_CONTEXT);

  context->instance = instance;

  if (instance->slots->parentId == NULL && instance->slots->container == NULL)
    lmLog(&instance->srcp, 422, sevWAR, ""); 
  analyzeSlots(instance->slots, context);
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
  generateInstanceSlotsData(ins->slots);
}


/*----------------------------------------------------------------------

  generateInstanceEntry

*/
static void generateInstanceEntry(InsNod *ins)
{
  InstanceEntry entry;

  generateSlotsEntry(&entry, ins->slots);
  emitEntry(&entry, sizeof(entry));
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
  header->theHero = theHero->code;
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

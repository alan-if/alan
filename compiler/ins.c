/*----------------------------------------------------------------------*\

				INS.C
			    Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "id_x.h"
#include "lst_x.h"
#include "scr_x.h"
#include "prop_x.h"
#include "srcp_x.h"
#include "sym_x.h"
#include "wrd_x.h"
#include "atr_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "sysdep.h"
#include "util.h"
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
  Instance *theHeroInstance;

  if (hero == NULL) {
    theHeroInstance = newInstance(&nulsrcp, newId(nulsrcp, "hero"),
				  newId(nulsrcp, "actor"), NULL);
    theHero = theHeroInstance->props->id->symbol;
  } else
    theHero = hero;
}


/*======================================================================

  newInstance()

  Allocates and initialises an instance node.

  */
Instance *newInstance(Srcp *srcp,	/* IN - Source Position */
		    IdNode *id,
		    IdNode *parent,
		    Properties *props)
{
  Instance *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(Instance);

  new->srcp = *srcp;
  if (props)
    new->props = props;
  else
    new->props = newEmptyProps();
  new->props->id = id;
  new->props->parentId = parent;

  new->props->id->symbol = newSymbol(id, INSTANCE_SYMBOL);
  new->props->id->symbol->fields.entity.props = new->props;

  allInstances = concat(allInstances, new, INSTANCE_LIST);

  return(new);
}



/*----------------------------------------------------------------------

  symbolizeInstance()

  Symbolize a Instance node.

 */
static void symbolizeInstance(Instance *ins)
{
  symbolizeProps(ins->props);
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


/*======================================================================*/
void analyzeAllInstanceAttributes() {
  List *l;
  TRAVERSE(l, allInstances) {
    Properties *props = l->element.ins->props;
    analyzeAttributes(props->attributes, props->id->symbol);
  }
}


/*----------------------------------------------------------------------*/
static void analyzeNameWords(Instance *instance)
{
  List *nameList, *list;

  /* Note names as words in the dictionary */
  if (instance->props->names == NULL) /* No name, use identifier as a noun */
    newNounWord(instance->props->id->string, instance->props->id->code, instance);
  else {
    for (nameList = instance->props->names; nameList != NULL; nameList = nameList->next) {
      for (list = nameList->element.lst; list->next != NULL; list = list->next)
	newAdjectiveWord(list->element.id->string, instance);
      newNounWord(list->element.id->string, list->element.id->code, instance);
    }
  }
}

/*----------------------------------------------------------------------*/
static void analyzePronouns(Instance *instance)
{
  List *p;

  if (instance->props->pronouns && inheritsFrom(instance->props->id->symbol, locationSymbol))
    lmLog(&instance->props->pronounsSrcp, 326, sevERR, NULL);
 
  TRAVERSE(p, instance->props->pronouns)
    p->element.id->code = newPronounWord(p->element.id->string, instance);
}



/*----------------------------------------------------------------------*/
static void analyzeInstance(Instance *instance)
{
  Context *context = newInstanceContext(instance);

  /* Only instances need names and pronouns in the dictionary */
  analyzeNameWords(instance);
  analyzePronouns(instance);

  analyzeProps(instance->props, context);
}


/*======================================================================*/
void analyzeInstances(void)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    analyzeInstance(l->element.ins);
}


/*----------------------------------------------------------------------*/
static void generateInstanceData(Instance *ins)
{
  generateInstancePropertiesData(ins->props);
}


/*----------------------------------------------------------------------*/
static void generateInstanceEntry(Instance *ins)
{
  InstanceEntry entry;

  generatePropertiesEntry(&entry, ins->props);
  emitEntry(&entry, sizeof(entry));
}


/*----------------------------------------------------------------------*/
static Aaddr generateInstanceTable(void)
{
  Aaddr address = nextEmitAddress();
  List *l;

  for (l = allInstances; l; l = l->next)
    generateInstanceEntry(l->element.ins);
  emit(EOF);
  return address;
}


/*======================================================================*/
void generateInstances(AcdHdr *header)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    generateInstanceData(l->element.ins);

  header->instanceTableAddress = generateInstanceTable();

  header->instanceMax = instanceCount;
  header->attributesAreaSize = attributeAreaSize;
  header->theHero = theHero->code;
}



/*======================================================================*/
void dumpInstance(Instance *ins)
{
  put("INS: "); dumpSrcp(&ins->srcp); indent();
  put("props: "); dumpProps(ins->props); out();
}

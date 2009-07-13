/*----------------------------------------------------------------------*\

				INS.C
			    Instance Nodes

\*----------------------------------------------------------------------*/

#include "ins_x.h"

#include "description_x.h"
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


/*======================================================================*/
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


/*======================================================================*/
Instance *newInstance(Srcp *srcp,
		      IdNode *id,
		      IdNode *parent,
		      Properties *props)
{
  Instance *new;                  /* The newly allocated area */

  progressCounter();

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



/*----------------------------------------------------------------------*/
static void symbolizeInstance(Instance *ins)
{
  symbolizeProps(ins->props, FALSE);
}


/*======================================================================*/
void symbolizeInstances(void)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    symbolizeInstance(l->member.ins);
}


/*======================================================================*/
void analyzeAllInstanceAttributes() {
  List *l;
  TRAVERSE(l, allInstances) {
    Properties *props = l->member.ins->props;
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
      for (list = nameList->member.lst; list->next != NULL; list = list->next)
	newAdjectiveWord(list->member.id->string, instance);
      newNounWord(list->member.id->string, list->member.id->code, instance);
    }
  }
}


/*----------------------------------------------------------------------*/
static void analyzePronouns(Instance *instance)
{
  List *p;

  TRAVERSE(p, instance->props->pronouns)
    p->member.id->code = newPronounWord(p->member.id->string, instance);
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
    analyzeInstance(l->member.ins);
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
    generateInstanceEntry(l->member.ins);
  emit(EOF);
  return address;
}


/*======================================================================*/
void generateInstances(ACodeHeader *header)
{
  List *l;

  for (l = allInstances; l; l = l->next)
    generateInstanceData(l->member.ins);

  header->instanceTableAddress = generateInstanceTable();

  header->instanceMax = instanceCount;
  header->attributesAreaSize = attributeAreaSize;
  header->theHero = theHero->code;
}



/*======================================================================*/
void dumpInstance(Instance *ins)
{
  put("INS: "); dumpSrcp(ins->srcp); indent();
  put("props: "); dumpProps(ins->props); out();
}

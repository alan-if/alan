/*----------------------------------------------------------------------*\

				CLA.C
			     Class Nodes

\*----------------------------------------------------------------------*/

#include "cla_x.h"

/* IMPORT */
#include <stdio.h>
#include "types.h"
#include "opt.h"

#include "ext.h"
#include "srcp_x.h"
#include "id_x.h"
#include "sym_x.h"
#include "adv_x.h"
#include "atr_x.h"
#include "prop_x.h"
#include "lst_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "emit.h"
#include "util.h"
#include "options.h"
#include "lmList.h"


/* PUBLIC DATA */

/* Predefined classes */
Class *entity,
  *thing,
  *object,
  *location,
  *actor,
  *literal,
  *integer,
  *string;



/* PRIVATE DATA */

static List *allClasses = NULL;

/*----------------------------------------------------------------------*/
static void addPredefinedProperties() {

  /* Add pronouns */
  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
    thing->props->pronouns = concat(NULL, newId(nulsrcp, "it"), ID_LIST);
    actor->props->pronouns = concat(concat(NULL,
					   newId(nulsrcp, "him"),
					   ID_LIST),
				    newId(nulsrcp, "her"),
				    ID_LIST);

    break;
  case L_SWEDISH:
    thing->props->pronouns = concat(concat(NULL,
					    newId(nulsrcp, "den"),
					    ID_LIST),
				     newId(nulsrcp, "det"),
				     ID_LIST);
    actor->props->pronouns = concat(concat(NULL,
					    newId(nulsrcp, "henne"), ID_LIST),
				     newId(nulsrcp, "honom"), ID_LIST);
    break;
  case L_GERMAN:
    thing->props->pronouns = concat(concat(concat(NULL,
						  newId(nulsrcp, "es"),
						  ID_LIST),
					   newId(nulsrcp, "ihn"),
					   ID_LIST),
				    newId(nulsrcp, "sie"),
				    ID_LIST);
    break;
  }
}


/*======================================================================*/
void initClasses()
{
  IdNode *thingId = newId(nulsrcp, "thing");
  IdNode *entityId = newId(nulsrcp, "entity");
  IdNode *literalId = newId(nulsrcp, "literal");
  IdNode *locationId = newId(nulsrcp, "location");
  IdNode *objectId = newId(nulsrcp, "object");
  IdNode *actorId = newId(nulsrcp, "actor");
  IdNode *integerId = newId(nulsrcp, "integer");
  IdNode *stringId = newId(nulsrcp, "string");

  allClasses = NULL;

  entity = newClass(&nulsrcp, entityId, NULL, NULL);
  adv.clas = concat(adv.clas, entity, CLASS_LIST);
  entitySymbol = entity->props->id->symbol;
  entity->props->predefined = TRUE;

  location = newClass(&nulsrcp, locationId, entityId, NULL);
  adv.clas = concat(adv.clas, location, CLASS_LIST);
  locationSymbol = location->props->id->symbol;
  location->props->predefined = TRUE;

  thing = newClass(&nulsrcp, thingId, entityId, NULL);
  adv.clas = concat(adv.clas, thing, CLASS_LIST);
  thingSymbol = thing->props->id->symbol;
  thing->props->predefined = TRUE;

  object = newClass(&nulsrcp, objectId, thingId, NULL);
  adv.clas = concat(adv.clas, object, CLASS_LIST);
  objectSymbol = object->props->id->symbol;
  object->props->predefined = TRUE;

  actor = newClass(&nulsrcp, actorId, thingId, NULL);
  adv.clas = concat(adv.clas, actor, CLASS_LIST);
  actorSymbol = actor->props->id->symbol;
  actor->props->predefined = TRUE;

  literal = newClass(&nulsrcp, literalId, entityId, NULL);
  adv.clas = concat(adv.clas, literal, CLASS_LIST);
  literalSymbol = literal->props->id->symbol;
  literalSymbol->fields.entity.prohibitedSubclassing = TRUE;
  literal->props->predefined = TRUE;

  integer = newClass(&nulsrcp, integerId, literalId, NULL);
  adv.clas = concat(adv.clas, integer, CLASS_LIST);
  integerSymbol = integer->props->id->symbol;
  integerSymbol->fields.entity.prohibitedSubclassing = TRUE;
  integerSymbol->fields.entity.isBasicType = TRUE;
  integer->props->predefined = TRUE;

  string = newClass(&nulsrcp, stringId, literalId, NULL);
  adv.clas = concat(adv.clas, string, CLASS_LIST);
  stringSymbol = string->props->id->symbol;
  stringSymbol->fields.entity.prohibitedSubclassing = TRUE;
  stringSymbol->fields.entity.isBasicType = TRUE;
  string->props->predefined = TRUE;

  addPredefinedProperties();
}


/*======================================================================*/
Class *newClass(Srcp *srcp,	/* IN - Source Position */
		IdNode *id,
		IdNode *parent,
		Properties *props)
{
  Class *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(Class);

  new->srcp = *srcp;
  if (props == NULL)
    new->props = newEmptyProps();
  else
    new->props = props;

  new->props->id = id;
  new->props->parentId = parent;
  new->props->id->symbol = newSymbol(id, CLASS_SYMBOL);
  new->props->id->symbol->fields.entity.props = new->props;

  allClasses = concat(allClasses, new, CLASS_LIST);

  if (compareStrings(id->string, "container") == 0)
    lmLogv(&id->srcp, 260, sevERR, "class", "'container'", "the built-in container property", NULL);

  return(new);
}


/*----------------------------------------------------------------------*/
static void symbolizeClass(Class *cla)
{
  symbolizeProps(cla->props);

  if (cla->props->parentId != NULL) {
    if (cla->props->parentId->symbol != NULL) {
      if (cla->props->parentId->symbol->kind != CLASS_SYMBOL)
	lmLog(&cla->props->parentId->srcp, 350, sevERR, "");
      else
	setParent(cla->props->id->symbol, cla->props->parentId->symbol);
    }
  }
}


/*======================================================================

  symbolizeClasses()

  Symbolize all Class nodes.

 */
void symbolizeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    symbolizeClass(l->element.cla);
}


/*----------------------------------------------------------------------*/
static void warnForUnimplementedInheritance(Properties *props) {
  int propCount = 0;		/* To count that we catch all */

  /* We can currently inherit:
  	Initial location
  	Names
	Pronouns
  	Attributes
	Container
	DescriptionCheck
	DescriptionStatements
	Entered
	Definite
	Indefinite
	Script
	Exit
	Verb
  */
  propCount = 13;

  if (props->initialize != NULL)
    lmLog(&props->initialize->srcp, 343, sevWAR, "Initialize clause");
  propCount++;
  if (props->mentioned != NULL)
    lmLog(&props->mentionedSrcp, 343, sevWAR, "Mentioned clause");
  propCount++;

  if (propCount != NOOFPROPS)
    SYSERR("Wrong number of inherited props");
}


/*----------------------------------------------------------------------*/
static void analyzeClass(Class *class)
{
  Context *context = newClassContext(class);

  warnForUnimplementedInheritance(class->props);
  analyzeProps(class->props, context);
}


/*======================================================================*/
void analyzeAllClassAttributes() {
  List *l;
  TRAVERSE(l, allClasses) {
    Properties *props = l->element.cla->props;
    analyzeAttributes(props->attributes, props->id->symbol);
  }
}


/*======================================================================*/
void analyzeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    analyzeClass(l->element.cla);
}


/*----------------------------------------------------------------------*/
static void generateClassData(Class *cla)
{
  generateCommonPropertiesData(cla->props);
  if (debugFlag) {
    cla->props->idAddress = nextEmitAddress();
    emitString(cla->props->id->string);
  }
}


/*----------------------------------------------------------------------*/
static void generateClassEntry(Class *cla)
{
  ClassEntry entry;

  cla->adr = nextEmitAddress();

  entry.code = cla->props->id->symbol->code;	/* First own code */

  if (cla->props->parentId == NULL)	/* Then parents */
    entry.parent = 0;
  else
    entry.parent = cla->props->parentId->symbol->code;

  entry.idAddress = cla->props->idAddress;
  entry.descriptionChecks = cla->props->descriptionChecksAddress;
  entry.description = cla->props->descriptionAddress;
  entry.entered = cla->props->enteredAddress;
  entry.definite = cla->props->definiteAddress;
  entry.definiteIsForm = cla->props->definiteIsForm;
  entry.indefinite = cla->props->indefiniteAddress;
  entry.indefiniteIsForm = cla->props->indefiniteIsForm;
  entry.mentioned = cla->props->mentionedAddress;
  entry.verbs = cla->props->verbsAddress;

  emitEntry(&entry, sizeof(entry));
}


/*======================================================================*/
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  acdHeader.entityClassId = entitySymbol->code;
  acdHeader.thingClassId = thingSymbol->code;
  acdHeader.objectClassId = objectSymbol->code;
  acdHeader.locationClassId = locationSymbol->code;
  acdHeader.actorClassId = actorSymbol->code;
  acdHeader.literalClassId = literalSymbol->code;
  acdHeader.integerClassId = integerSymbol->code;
  acdHeader.stringClassId = stringSymbol->code;
  acdHeader.classMax = classCount;

  for (l = allClasses; l; l = l->next)
    generateClassData(l->element.cla);

  adr = nextEmitAddress();
  for (l = allClasses; l; l = l->next)
    generateClassEntry(l->element.cla);
  emit(EOF);

  return (adr);
}



/*======================================================================*/
void dumpClass(Class *cla)
{
  put("CLA: "); dumpSrcp(cla->srcp); indent();
  put("props: "); dumpProps(cla->props); out();
}


/*======================================================================*/
void dumpClasses(void)
{
  dumpList(allClasses, CLASS_LIST);
}

/*----------------------------------------------------------------------*\

				CLA.C
			     Class Nodes

\*----------------------------------------------------------------------*/

#include "cla_x.h"

/* IMPORT */
#include <stdio.h>
#include "types.h"

#include "srcp_x.h"
#include "id_x.h"
#include "sym_x.h"
#include "prop_x.h"
#include "lst_x.h"
#include "context_x.h"

#include "emit.h"
#include "util.h"
#include "options.h"
#include "dump.h"
#include "lmList.h"


/* PUBLIC DATA */

/* Predefined classes */
ClaNod *entity,
  *thing,
  *object,
  *location,
  *actor,
  *literal,
  *integer,
  *string;



/* PRIVATE DATA */

static List *allClasses = NULL;



/*======================================================================*/
void initClasses()
{
  IdNode *thingId = newId(&nulsrcp, "thing");
  IdNode *entityId = newId(&nulsrcp, "entity");
  IdNode *literalId = newId(&nulsrcp, "literal");

  allClasses = NULL;

  entity = newClass(&nulsrcp, entityId, NULL, NULL);
  entitySymbol = entity->props->id->symbol;
  entity->props->predefined = TRUE;

  location = newClass(&nulsrcp, newId(&nulsrcp, "location"),
		      entityId, NULL);
  locationSymbol = location->props->id->symbol;
  location->props->predefined = TRUE;

  thing = newClass(&nulsrcp, thingId, entityId, NULL);
  thingSymbol = thing->props->id->symbol;
  thing->props->predefined = TRUE;

  object = newClass(&nulsrcp, newId(&nulsrcp, "object"),
		    thingId, NULL);
  objectSymbol = object->props->id->symbol;
  object->props->predefined = TRUE;

  actor = newClass(&nulsrcp, newId(&nulsrcp, "actor"),
		   thingId, NULL);
  actorSymbol = actor->props->id->symbol;
  actor->props->predefined = TRUE;

  literal = newClass(&nulsrcp, literalId, entityId, NULL);
  literalSymbol = literal->props->id->symbol;
  literalSymbol->fields.entity.prohibitedSubclassing = TRUE;
  literal->props->predefined = TRUE;

  integer = newClass(&nulsrcp, newId(&nulsrcp, "integer"), literalId, NULL);
  integerSymbol = integer->props->id->symbol;
  integerSymbol->fields.entity.prohibitedSubclassing = TRUE;
  integer->props->predefined = TRUE;

  string = newClass(&nulsrcp, newId(&nulsrcp, "string"), literalId, NULL);
  stringSymbol = string->props->id->symbol;
  stringSymbol->fields.entity.prohibitedSubclassing = TRUE;
  string->props->predefined = TRUE;
}



/*======================================================================*/
ClaNod *newClass(Srcp *srcp,	/* IN - Source Position */
		 IdNode *id,
		 IdNode *parent,
		 Properties *props)
{
  ClaNod *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(ClaNod);

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

  return(new);
}


/*----------------------------------------------------------------------

  symbolizeClass()

  Symbolize a Class node.

 */
static void symbolizeClass(ClaNod *cla)
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
static void analyzeClass(ClaNod *class)
{
  Context *context = newContext(CLASS_CONTEXT);

  context->class = class;

  analyzeProps(class->props, context);
}



/*======================================================================*/
void analyzeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    analyzeClass(l->element.cla);
}


/*----------------------------------------------------------------------*/
static void generateClassData(ClaNod *cla)
{
  generateClassPropertiesData(cla->props);
  if (debugOption) {
    cla->props->idAddress = emadr();
    emitString(cla->props->id->string);
  }
}


/*----------------------------------------------------------------------*/
static void generateClassEntry(ClaNod *cla)
{
  ClassEntry entry;

  cla->adr = emadr();

  entry.code = cla->props->id->symbol->code;	/* First own code */

  if (cla->props->parentId == NULL)	/* Then parents */
    entry.parent = 0;
  else
    entry.parent = cla->props->parentId->symbol->code;

  entry.idAddress = cla->props->idAddress;
  entry.checks = cla->props->descriptionChecksAddress;
  entry.description = cla->props->descriptionAddress;
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

  adr = emadr();
  for (l = allClasses; l; l = l->next)
    generateClassEntry(l->element.cla);
  emit(EOF);

  return (adr);
}



/*======================================================================*/
void dumpClass(ClaNod *cla)
{
  put("CLA: "); dumpSrcp(&cla->srcp); in();
  put("props: "); dumpProps(cla->props); out();
}


/*======================================================================*/
void dumpClasses(void)
{
  dumpList(allClasses, CLASS_LIST);
}

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
#include "dump.h"
#include "lmList.h"


/* PUBLIC DATA */

ClaNod *thing, *object, *location, *actor;



/* PRIVATE DATA */

static List *allClasses = NULL;



/*======================================================================

  initClasses()

*/
void initClasses()
{
  IdNode *thingId = newId(&nulsrcp, "thing");

  allClasses = NULL;

  thing = newClass(&nulsrcp, thingId, NULL, NULL);
  thingSymbol = thing->props->id->symbol;
  object = newClass(&nulsrcp, newId(&nulsrcp, "object"),
		    thingId, NULL);
  objectSymbol = object->props->id->symbol;
  location = newClass(&nulsrcp, newId(&nulsrcp, "location"),
		      thingId, NULL);
  locationSymbol = location->props->id->symbol;
  actor = newClass(&nulsrcp, newId(&nulsrcp, "actor"),
		   thingId, NULL);
  actorSymbol = actor->props->id->symbol;

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

  allClasses = concat(allClasses, new, LIST_CLA);

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



/*----------------------------------------------------------------------

  analyzeClass()

  Analyze a Class node.

 */
static void analyzeClass(ClaNod *class)
{
  Context *context = newContext(CLASS_CONTEXT);

  context->class = class;

  analyzeProps(class->props, context);
}



/*======================================================================

  analyzeClasses()

  Analyze all Class nodes.

 */
void analyzeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    analyzeClass(l->element.cla);
}


/*----------------------------------------------------------------------

  generateClassEntry

*/
static void generateClassEntry(ClaNod *cla)
{
  ClassEntry entry;

  cla->adr = emadr();

  entry.code = cla->props->id->symbol->code;	/* First own code */

  if (cla->props->parentId == NULL)	/* Then parents */
    entry.parent = 0;
  else
    entry.parent = cla->props->parentId->symbol->code;

  entry.description = cla->props->descriptionAddress;

  emitEntry(&entry, sizeof(entry));
}


/*----------------------------------------------------------------------

  generateInstanceData

  Generate the data parts for one instance.

*/
static void generateClassData(ClaNod *cla)
{
  generateClassPropertiesData(cla->props);
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  acdHeader.thingClassId = thing->props->id->symbol->code;
  acdHeader.objectClassId = object->props->id->symbol->code;
  acdHeader.locationClassId = location->props->id->symbol->code;
  acdHeader.actorClassId = actor->props->id->symbol->code;

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


/*======================================================================

  dumpClasses()

  Dump all Class nodes.

 */
void dumpClasses(void)
{
  dumpList(allClasses, LIST_CLA);
}

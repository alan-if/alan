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
#include "slt_x.h"
#include "lst_x.h"

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
  thingSymbol = thing->slots->id->symbol;
  object = newClass(&nulsrcp, newId(&nulsrcp, "object"),
		    thingId, NULL);
  objectSymbol = object->slots->id->symbol;
  location = newClass(&nulsrcp, newId(&nulsrcp, "location"),
		      thingId, NULL);
  locationSymbol = location->slots->id->symbol;
  actor = newClass(&nulsrcp, newId(&nulsrcp, "actor"),
		   thingId, NULL);
  actorSymbol = actor->slots->id->symbol;

}



/*======================================================================

  newcla()

  Allocates and initialises a class node.

  */
ClaNod *newClass(Srcp *srcp,	/* IN - Source Position */
		 IdNode *id,
		 IdNode *parent,
		 Slots *slots)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  if (slots == NULL)
    new->slots = newEmptySlots();
  else
    new->slots = slots;

  new->slots->id = id;
  new->slots->parentId = parent;
  new->slots->id->symbol = newSymbol(id, CLASS_SYMBOL);
  new->slots->id->symbol->fields.claOrIns.slots = new->slots;

  allClasses = concat(allClasses, new, LIST_CLA);

  return(new);
}


/*----------------------------------------------------------------------

  symbolizeClass()

  Symbolize a Class node.

 */
static void symbolizeClass(ClaNod *cla)
{
  symbolizeSlots(cla->slots);

  if (cla->slots->parentId != NULL) {
    if (cla->slots->parentId->symbol != NULL) {
      if (cla->slots->parentId->symbol->kind != CLASS_SYMBOL)
	lmLog(&cla->slots->parentId->srcp, 350, sevERR, "");
      else
	setParent(cla->slots->id->symbol, cla->slots->parentId->symbol);
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
static void analyzeClass(ClaNod *cla)
{
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

  entry.code = cla->slots->id->symbol->code;	/* First own code */
  if (cla->slots->parentId == NULL)	/* Then parents */
    entry.parent = 0;
  else
    entry.parent = cla->slots->parentId->symbol->code;
  emitEntry(&entry, sizeof(entry));
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  acdHeader.thingClassId = thing->slots->id->symbol->code;
  acdHeader.objectClassId = object->slots->id->symbol->code;
  acdHeader.locationClassId = location->slots->id->symbol->code;
  acdHeader.actorClassId = actor->slots->id->symbol->code;

  adr = emadr();
  for (l = allClasses; l; l = l->next)
    generateClassEntry(l->element.cla);
  emit(EOF);

  return (adr);
}



/*======================================================================

  dumpClass()

  Dump a Class node.

 */
void dumpClass(ClaNod *cla)
{
  put("CLA: "); dumpSrcp(&cla->srcp); in();
  put("slots: "); dumpSlots(cla->slots); out();
}


/*======================================================================

  dumpClasses()

  Dump all Class nodes.

 */
void dumpClasses(void)
{
  dumpList(allClasses, LIST_CLA);
}

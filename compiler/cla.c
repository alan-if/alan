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
  allClasses = NULL;

  thing = newcla(&nulsrcp, newId(&nulsrcp, "thing"), NULL, NULL);
  object = newcla(&nulsrcp, newId(&nulsrcp, "object"),
		  newId(&nulsrcp, "thing"), NULL);
  location = newcla(&nulsrcp, newId(&nulsrcp, "location"),
		    newId(&nulsrcp, "thing"), NULL);
  actor = newcla(&nulsrcp, newId(&nulsrcp, "actor"),
		 newId(&nulsrcp, "thing"), NULL);

}



/*======================================================================

  newcla()

  Allocates and initialises a class node.

  */
ClaNod *newcla(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,
	       IdNode *parent,
	       SlotsNode *slt)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  if (slt == NULL)
    new->slots = newEmptySlots();
  else
    new->slots = slt;

  new->slots->id = id;
  new->slots->parent = parent;
  new->slots->symbol = newsym(id->string, CLASS_SYMBOL);

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

  if (cla->slots->parent != NULL) {
    if (cla->slots->parent->symbol != NULL) {
      if (cla->slots->parent->symbol->kind != CLASS_SYMBOL)
	lmLog(&cla->slots->parent->srcp, 350, sevERR, "");
      else
	setParent(cla->slots->symbol, cla->slots->parent->symbol);
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
  cla->adr = emadr();

  emit(cla->slots->symbol->code);	/* First own code */
  if (cla->slots->parent == NULL)	/* Then parents */
    emit(0);
  else
    emit(cla->slots->parent->symbol->code);
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  acdHeader.thingClassId = thing->slots->symbol->code;
  acdHeader.objectClassId = object->slots->symbol->code;
  acdHeader.locationClassId = location->slots->symbol->code;
  acdHeader.actorClassId = actor->slots->symbol->code;

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
  dulst(allClasses, LIST_CLA);
}

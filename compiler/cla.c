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



/* PRIVATE DATA */

static List *allClasses = NULL;

static ClaNod *thing, *object, *location, *actor;



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
  new->parent = parent;
  if (slt == NULL)
    new->slt = newEmptySlots();
  else
    new->slt = slt;

  new->slt->id = id;


  new->symbol = newsym(id->string, CLASS_SYMBOL);

  allClasses = concat(allClasses, new, LIST_CLA);

  return(new);
}


/*----------------------------------------------------------------------

  symbolizeClass()

  Symbolize a Class node.

 */
static void symbolizeClass(ClaNod *cla)
{
  SymNod *parent;

  symbolizeSlots(cla->slt);

  if (cla->parent != NULL) {
    parent = lookup(cla->parent->string);
    if (parent == NULL)
      lmLog(&cla->parent->srcp, 310, sevERR, cla->parent->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&cla->parent->srcp, 350, sevERR, "");
    else {
      cla->parent->symbol = parent;
      setParent(cla->symbol, cla->parent->symbol);
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

  emit(cla->symbol->code);	/* First own code */
  if (cla->parent == NULL)	/* Then parents */
    emit(0);
  else
    emit(cla->parent->symbol->code);
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  acdHeader.thingClassId = thing->symbol->code;
  acdHeader.objectClassId = object->symbol->code;
  acdHeader.locationClassId = location->symbol->code;
  acdHeader.actorClassId = actor->symbol->code;

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
  put("parent: "); dumpId(cla->parent); nl();
  put("slots: "); dumpSlots(cla->slt); out();
}


/*======================================================================

  dumpClasses()

  Dump all Class nodes.

 */
void dumpClasses(void)
{
  dulst(allClasses, LIST_CLA);
}

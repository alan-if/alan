/*----------------------------------------------------------------------*\

				ADD.C
			     Add To Nodes

\*----------------------------------------------------------------------*/

#include "add_x.h"

/* IMPORT: */
#include "slt_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "srcp_x.h"
#include "atr_x.h"

#include "vrb.h"
#include "scr.h"
#include "ext.h"


#include "adv.h"

#include "types.h"
#include "dump.h"
#include "util.h"
#include "lmList.h"



/*======================================================================

  newAdd()

  Allocates and initialises an AddTo node.

  */
AddNode *newAdd(Srcp *srcp,
		IdNode *id,
		IdNode *parent,
		SlotsNode *slt)
{
  AddNode *new;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(AddNode);

  new->srcp = *srcp;
  if (slt)
    new->slots = slt;
  else
    new->slots = newEmptySlots();
  new->toId = id;

  if (parent != NULL)
    lmLogv(&parent->srcp, 341, sevERR, "heritage", "", NULL);

  return(new);
}


/*----------------------------------------------------------------------

  addInitialLocation()

*/
static void addInitialLocation(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->whr != NULL)
    lmLogv(&slots->whr->srcp, 341, sevERR, "initial location", "(yet)", NULL);
}


/*----------------------------------------------------------------------

  addNames()

*/
static void addNames(AddNode *add, Symbol *original)
{
#ifdef FIXME
  if (slots->names != NULL)
    lmLogv(&slots->names->element.nam->srcp, 341, sevERR, "names", "(yet)", NULL);
#endif
}


/*----------------------------------------------------------------------

  addAttributes()

*/
static void addAttributes(AddNode *add, Symbol *originalSymbol)
{
  List *addedAttributes = add->slots->attributes;
  SlotsNode *originalSlots = originalSymbol->fields.claOrIns.slots;
  List *originalAttributes = originalSlots->attributes;
  List *l;

  if (addedAttributes == NULL) return;

  for (l = addedAttributes; l != NULL; l = l->next) {
    AtrNod *originalAttribute = findAttribute(originalAttributes, l->element.atr->id);
    if (originalAttribute != NULL) /* It was found in the original */
      lmLog(&l->element.atr->id->srcp, 336, sevERR, "an existing attribute");
  }
  originalSlots->attributes = combineAttributes(originalSlots->attributes,
						addedAttributes);
}


/*----------------------------------------------------------------------

  addDescription()

*/
static void addDescription(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->description != NULL)
    lmLogv(&slots->descriptionSrcp, 341, sevERR, "description", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addArticle()

*/
static void addArticle(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->article != NULL)
    lmLogv(&slots->articleSrcp, 341, sevERR, "article", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addMentioned()

*/
static void addMentioned(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->mentioned != NULL)
    lmLogv(&slots->mentionedSrcp, 341, sevERR, "mentioned", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addContainer()

*/
static void addContainer(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->container != NULL)
    lmLogv(&slots->container->srcp, 341, sevERR, "container", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addVerbs()

*/
static void addVerbs(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->verbs != NULL)
    lmLogv(&slots->verbs->element.vrb->srcp, 341, sevERR, "verbs", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addScripts()

*/
static void addScripts(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->scripts != NULL)
    lmLogv(&slots->scripts->element.scr->srcp, 341, sevERR, "scripts", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addExits()

*/
static void addExits(AddNode *add, Symbol *original)
{
  SlotsNode *slots = add->slots;

  if (slots->exits != NULL)
    lmLogv(&slots->exits->element.ext->srcp, 341, sevERR, "exits", "(yet)", NULL);

}


/*----------------------------------------------------------------------

  addAddition()

*/
static void addAddition(AddNode *add)
{
  Symbol *originalClass = symcheck(add->toId, CLASS_SYMBOL, NULL);

  addInitialLocation(add, originalClass);
  addNames(add, originalClass);
  addAttributes(add, originalClass);
  addDescription(add, originalClass);
  addArticle(add, originalClass);
  addMentioned(add, originalClass);
  addContainer(add, originalClass);
  addVerbs(add, originalClass);
  addScripts(add, originalClass);
  addExits(add, originalClass);
}



/*======================================================================

  addAdditions()

  Add all additions to the respective classes.

 */
void addAdditions(void)
{
  List *l;

  for (l = adv.adds; l != NULL; l = l->next)
    addAddition(l->element.add);
}


/*======================================================================

  dumpAdd()

  Dump an Add To node.

 */
void dumpAdd(AddNode *add)
{
  put("ADD: "); dumpSrcp(&add->srcp); in();
  put("toId: "); dumpId(add->toId); nl();
  put("slots: "); dumpSlots(add->slots); out();
}

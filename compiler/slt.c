/*----------------------------------------------------------------------*\

				SLT.C
			      Slot Nodes

\*----------------------------------------------------------------------*/

#include "slt_x.h"

/* IMPORT */
#include <stdio.h>
#include "alan.h"
#include "util.h"
#include "dump.h"
#include "emit.h"
#include "lmList.h"


#include "atr_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "nam_x.h"
#include "scr_x.h"
#include "stm_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"


/*======================================================================

  newEmptySlots()

  Allocates and initialises a empty slots node.

  */
SlotsNode *newEmptySlots(void)
{
  return NEW(SlotsNode);
}



/*======================================================================

  newSlots()

  Allocates and initialises a slots node.

  */
SlotsNode *newSlots(List *names,
		    WhrNod *whr,
		    List *attributes,
		    CntNod *container,
		    List *description,
		    Srcp *descriptionSrcp,
		    List *mentioned,
		    Srcp *mentionedSrcp,
		    List *article,
		    Srcp *articleSrcp,
		    List *does,
		    List *exits,
		    List *verbs,
		    List *scripts)
{
  SlotsNode *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(SlotsNode);

  new->names = names;
  new->whr = whr;
  new->attributes = attributes;

  new->container = container;
  if (new->container != NULL)
    new->container->ownerSlots = new;

  new->description = description;
  new->descriptionSrcp = *descriptionSrcp;
  new->mentioned = mentioned;
  new->mentionedSrcp = *mentionedSrcp;
  new->article = article;
  new->articleSrcp = *articleSrcp;
  new->verbs = verbs;
  new->exits = exits;
  new->scripts = scripts;

  return(new);
}



/*----------------------------------------------------------------------

  symbolizeParent()

  Symbolize parent of a Slots node.

 */
static void symbolizeParent(SlotsNode *slots)
{
  Symbol *parent;

  if (slots->parentId != NULL) {
    parent = lookup(slots->parentId->string);
    if (parent == NULL)
      lmLog(&slots->parentId->srcp, 310, sevERR, slots->parentId->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&slots->parentId->srcp, 350, sevERR, "");
    else {
      slots->parentId->symbol = parent;
      setParent(slots->id->symbol, slots->parentId->symbol);
    }
  }
}


/*======================================================================

  symbolizeSlots()

  Symbolize data for one Slots node.

 */
void symbolizeSlots(SlotsNode *slots)
{
  symbolizeParent(slots);
  checkMultipleAttributes(slots->attributes);
  symbolizeWhr(slots->whr);
  symbolizeExits(slots->exits);
}


/*----------------------------------------------------------------------

  analyzeName()

*/

static void analyzeName(SlotsNode *slots)
{
  long fpos;
  int len = 0;
  StmNod *stm;

  if (slots->mentioned == NULL) {
    /* Generate a mentioned from the first of the names */
    /* First output the formated name to the text file */
    fpos = ftell(txtfil);
    len = annams(slots->names, slots->id,
		 inheritsFrom(slots->id->symbol, locationSymbol) ||
		 inheritsFrom(slots->id->symbol, actorSymbol));

    /* Then create a PRINT statement */
    stm = newstm(&nulsrcp, STM_PRINT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    slots->mentioned = concat(NULL, stm, STM_LIST);
  } else
    anstms(slots->mentioned, NULL);
}


/*======================================================================

  analyzeSlots()

  Analyze one Slots node.

 */
void analyzeSlots(SlotsNode *slots, Context *context)
{
  if (slots->whr != NULL) verifyInitialLocation(slots->whr);
  if (inheritsFrom(slots->id->symbol, locationSymbol) && slots->whr != NULL)
    lmLog(&slots->whr->srcp, 405, sevERR, "");
  if (inheritsFrom(slots->id->symbol, actorSymbol)
      && slots->whr != NULL && slots->whr->kind == WHR_IN)
    lmLog(&slots->whr->srcp, 402, sevERR, "An Actor");

  analyzeName(slots);
  anstms(slots->description, context);
  anvrbs(slots->verbs, slots->id->symbol);
  analyzeContainer(slots->container, context);

  if (slots->exits && !inheritsFrom(slots->id->symbol, locationSymbol))
    lmLog(&slots->id->srcp, 352, sevERR, slots->id->string);
  analyzeExits(slots->exits);

  if (slots->scripts && !inheritsFrom(slots->id->symbol, actorSymbol))
    lmLog(&slots->id->srcp, 353, sevERR, slots->id->string);
  analyzeScripts(slots->scripts, context);
}


/*======================================================================

  generateSlotsData()

  Generate data for one Slots node.

 */
void generateSlotsData(SlotsNode *slots, InsNod *instance)
{
  slots->idAddress = emadr();
  emitstr(slots->id->string);

  slots->attributeAddress = generateAttributes(slots->attributes);

  if (slots->description != NULL) {
    slots->descriptionAddress = emadr();
    gestms(slots->description, NULL);
    emit0(C_STMOP, I_RETURN);
  }

  if (slots->mentioned != NULL) {
    slots->mentionedAddress = emadr();
    gestms(slots->mentioned, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    emit(0);

  slots->scriptsAddress = generateScripts(instance);
  slots->verbsAddress = gevrbs(slots->verbs, instance);
  slots->exitsAddress = generateExits(slots->exits);
}


/*======================================================================

  generateSlotsEntry()

  Generate entries for one Slots node.

 */
void generateSlotsEntry(InstanceEntry *entry, SlotsNode *slots)
{
  entry->code = slots->id->symbol->code; /* First own code */
  entry->idAddress = slots->idAddress; /* Address to the id string */

  if (slots->parentId == NULL)	/* Then parents */
    entry->parentClass = 0;
  else
    entry->parentClass = slots->parentId->symbol->code;

  entry->location = generateInitialLocation(slots->whr);
  entry->attributes = slots->attributeAddress;
  entry->description = slots->descriptionAddress;
  if (slots->container != NULL)
    entry->container = slots->container->code;
  else
    entry->container = 0;
  entry->mentioned = slots->mentionedAddress;
  entry->article = slots->articleAddress;
  entry->exits = slots->exitsAddress;
  entry->verbs = slots->verbsAddress;
  entry->scripts = slots->scriptsAddress;
}


/*======================================================================

  dumpSlots()

  Dump a Slots node.

 */
void dumpSlots(SlotsNode *slots)
{
  put("SLOTS: "); dumpPointer(slots); in();
  put("id: "); dumpId(slots->id); nl();
  put("names: "); dulst(slots->names, LIST_NAM); nl();
  put("whr: "); duwhr(slots->whr); nl();
  put("container: "); dumpContainer(slots->container); nl();
  put("attributes: "); dulst(slots->attributes, LIST_ATR); nl();
  put("attributeAddress: "); dumpAddress(slots->attributeAddress); nl();
  put("description: "); dulst(slots->description, LIST_STM); nl();
  put("descriptionAddress: "); dumpAddress(slots->descriptionAddress); nl();
  put("article: "); dulst(slots->article, LIST_STM); nl();
  put("articleAddres: "); dumpAddress(slots->articleAddress); nl();
  put("mentioned: "); dulst(slots->mentioned, LIST_STM); nl();
  put("mentionedAddress: "); dumpAddress(slots->mentionedAddress); nl();
  put("scripts: "); dulst(slots->scripts, LIST_SCR); nl();
  put("scriptsAddress: "); dumpAddress(slots->scriptsAddress); nl();
  put("verbs: "); dulst(slots->verbs, LIST_VRB); nl();
  put("verbsAddress: "); dumpAddress(slots->verbsAddress); nl();
  put("exits: "); dulst(slots->exits, LIST_EXT); nl();
  put("exitsAddress: "); dumpAddress(slots->exitsAddress); out();
}

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
		    CntNod *cnt,
		    List *description,
		    List *mentioned,
		    List *art,
		    List *does,
		    List *exits,
		    List *verbs,
		    List *scrs)
{
  SlotsNode *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(SlotsNode);

  new->names = names;
  new->whr = whr;
  new->attributes = attributes;
  new->cnt = cnt;
  new->description = description;
  new->mentioned = mentioned;
  new->art = art;
  new->verbs = verbs;
  new->exits = exits;

  return(new);
}



/*----------------------------------------------------------------------

  symbolizeParent()

  Symbolize parent of a Slots node.

 */
static void symbolizeParent(SlotsNode *slots)
{
  SymNod *parent;

  if (slots->parent != NULL) {
    parent = lookup(slots->parent->string);
    if (parent == NULL)
      lmLog(&slots->parent->srcp, 310, sevERR, slots->parent->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&slots->parent->srcp, 350, sevERR, "");
    else {
      slots->parent->symbol = parent;
      setParent(slots->symbol, slots->parent->symbol);
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
		 inheritsFrom(slots->symbol, location->slots->symbol) ||
		 inheritsFrom(slots->symbol, actor->slots->symbol));

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
void analyzeSlots(SlotsNode *slots)
{
  if (slots->whr != NULL) verifyAtLocation(slots->whr);

  analyzeName(slots);
  anstms(slots->description, NULL);
  anvrbs(slots->verbs, slots->symbol);

  if (slots->exits && !inheritsFrom(slots->symbol, location->slots->symbol))
    lmLog(&slots->id->srcp, 352, sevERR, slots->id->string);
  analyzeExits(slots->exits);
}


/*======================================================================

  generateSlotsData()

  Generate data for one Slots node.

 */
void generateSlotsData(SlotsNode *slots)
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

  slots->exitsAddress = generateExits(slots->exits);
}


/*======================================================================

  generateSlotsEntry()

  Generate entries for one Slots node.

 */
void generateSlotsEntry(InstanceEntry *entry, SlotsNode *slots)
{
  entry->code = slots->symbol->code; /* First own code */
  entry->idAddress = slots->idAddress; /* Address to the id string */

  if (slots->parent == NULL)	/* Then parents */
    entry->parent = 0;
  else
    entry->parent = slots->parent->symbol->code;

  entry->location = generateInitialLocation(slots->whr);
  entry->attributes = slots->attributeAddress;
  entry->description = slots->descriptionAddress;
  entry->mentioned = slots->mentionedAddress;
  entry->article = slots->artadr;
  entry->exits = slots->exitsAddress;
  entry->verbs = slots->verbAddress;
}


/*======================================================================

  dumpSlots()

  Dump a Slots node.

 */
void dumpSlots(SlotsNode *slots)
{
  put("SLOTS: "); in();
  put("id: "); dumpId(slots->id); nl();
  put("names: "); dulst(slots->names, LIST_NAM); nl();
  put("whr: "); duwhr(slots->whr); nl();
#ifdef FIXME
  put("cnt: "); ducnt(slots->cnt); nl();
#endif
  put("attributes: "); dulst(slots->attributes, LIST_ATR); nl();
  put("attributeAddress: "); duadr(slots->attributeAddress); nl();
  put("description: "); dulst(slots->description, LIST_STM); nl();
  put("descriptionAddress: "); duadr(slots->descriptionAddress); nl();
  put("art: "); dulst(slots->art, LIST_STM); nl();
  put("artadr: "); duadr(slots->artadr); nl();
  put("mentioned: "); dulst(slots->mentioned, LIST_STM); nl();
  put("mentionedAddress: "); duadr(slots->mentionedAddress); nl();
  put("verbs: "); dulst(slots->verbs, LIST_VRB); nl();
  put("verbAddress: "); duadr(slots->verbAddress); nl();
  put("exits: "); dulst(slots->exits, LIST_EXT); nl();
  put("exitsAddress: "); duadr(slots->exitsAddress); out();
}

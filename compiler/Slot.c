/*----------------------------------------------------------------------*\

			       Slot.c

			     Slot Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "Slot.h"
#include "Class.h"
#include "Attribute.h"
#include "Symbol.h"

#include "dump.h"
#include "lmList.h"


/* PUBLIC */


/* PRIVATE */


/*======================================================================

  newSlot()

  Creates a new Slot and insert it in the symbol table.

  */
#ifdef _PROTOTYPES_
Slot *newSlot(List *heritage,
	      List *name,
	      Where *where,
	      List *attributes,
	      Container *container,
	      List *surroundings,
	      List *description,
	      List *mentioned,
	      Does *does,
	      List *exits,
	      List *verbs,
	      List *scripts)
#else
Slot *newSlot(heritage, name, where, attributes, container, surroundings,
		description, mentioned, does, exits, verbs, scripts)
     List *heritage;
     List *name;
     Where *where;
     List *attributes;
     Container *container;
     List *surroundings;
     List *description;
     List *mentioned;
     Does *does;
     List *exits;
     List *verbs;
     List *scripts;
#endif
{
  Slot *new;			/* The newly created Slot */

  new = NEW(Slot);

  new->state = IDLE;
  new->heritage = heritage;
  new->name = name;
  new->where = where;
  new->attributes = attributes;
  new->inheritedAttributeLists = NULL;
  new->container = container;
  new->surroundings = surroundings;
  new->description = description;
  new->mentioned = mentioned;
  new->does = does;
  new->exits = exits;
  new->verbs = verbs;
  new->scripts = scripts;
  
  return(new);
}


/*======================================================================

  analyseSlot()

  Do the analysis for a Slot.

  */
#ifdef _PROTOTYPES_
void analyseSlot(Slot *slot)
#else
void analyseSlot(slot)
     Slot *slot;
#endif
{
  List *list, *sentinel = NULL;
  Symbol *symbol;
  Bool remove;			/* Forced to remove the item? */
  List *localAttributes;
  Attribute *attribute;

  slot->state = LOOKING_FOR_CIRCLES;
  /* Verify that the inherited slots exist and are classes */
  for (list = slot->heritage; list; list = list->next) {
    remove = FALSE;
    if ((symbol = symbolCheck(list->element.id, CLASS_SYMBOL)) != NULL) {
      /* Ok, it was a class, now check for circular inheritance */
      if (symbol->info.class->slot->state == LOOKING_FOR_CIRCLES) {
	lmLog(&list->element.id->srcp, 226, sevERR, list->element.id->string);
	remove = TRUE;
      } else {
	if (symbol->info.class->slot->state != FINISHED)
	  analyseSlot(symbol->info.class->slot);
	if (symbol->info.class->slot->attributes != NULL)
	  slot->inheritedAttributeLists = prepend(symbol->info.class->slot->attributes,
						  symbol->info.class->slot->inheritedAttributeLists);
	else
	  slot->inheritedAttributeLists = symbol->info.class->slot->inheritedAttributeLists;
      }
    } else
      remove = TRUE;
    if (remove) {
      if (sentinel == NULL)	/* At first item */
	slot->heritage = list->next;
      else
	sentinel->next = list->next; /* Unlink the item */
    } else
      sentinel = list;
  }
  slot->state = NUMBERING_ATTRIBUTES;
  /* Number the attributes */
  for (localAttributes = slot->attributes; localAttributes; localAttributes = localAttributes->next) {
    attribute = findAttributeInLists(localAttributes->element.attribute->id,
				     slot->inheritedAttributeLists);
    if (attribute == NULL)
      localAttributes->element.attribute->code = ++attributeCount;
    else if (!equalTypes(localAttributes->element.attribute->type, attribute->type))
      lmLog(&localAttributes->element.attribute->srcp, 228, sevERR, NULL);
    else
      localAttributes->element.attribute->code = attribute->code;
  }

  /* 4f - Check for inheritance clashes */
  /* 4f - Analyse the name */
  /* 4f - Analyse the where */
  /* 4f - Analyse the attributes */
  /* 4f - Analyse the container */
  /* 4f - Analyse the surroundings */
  /* 4f - Analyse the description */
  /* 4f - Analyse the mentioned */
  /* 4f - Analyse the does */
  /* 4f - Analyse the exits */
  /* 4f - Analyse the verbs */
  /* 4f - Analyse the scripts */
  slot->state = FINISHED;
}



/*======================================================================

  dumpSlot()

  Dump a Slot.

  */
#ifdef _PROTOTYPES_
void dumpSlot(Slot *slot)
#else
void dumpSlot(slot)
     Slot *slot;
#endif
{
  if (slot == NULL) {
    put("NULL");
    return;
  }

  put("SLOT: "); in();
  put("heritage: "); dumpList(slot->heritage, ID_NODE); nl();
  put("name: "); dumpList(slot->name, ID_NODE); nl();
  put("where: "); dumpWhere(slot->where); nl();
  put("attributes: "); dumpList(slot->attributes, ATTRIBUTE_NODE); nl();
  put("inheritedAttributeLists: "); dumpList(slot->inheritedAttributeLists, ADDRESS_NODE); nl();
  put("container: "); dumpContainer(slot->container); nl();
  put("surroundings: "); dumpList(slot->surroundings, STATEMENT_NODE); nl();
  put("description: "); dumpList(slot->description, STATEMENT_NODE); nl();
  put("mentioned: "); dumpList(slot->mentioned, STATEMENT_NODE); nl();
  put("does: "); dumpDoes(slot->does); nl();
  put("exits: "); dumpList(slot->exits, EXIT_NODE); nl();
  put("verbs: "); dumpList(slot->verbs, VERB_NODE); nl();
  put("scripts: "); dumpList(slot->scripts, SCRIPT_NODE); out();
}

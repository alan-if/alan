/*----------------------------------------------------------------------*\

			       Slot.c

			     Slot Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "Slot.h"
#include "Class.h"
#include "Attribute.h"
#include "Exit.h"
#include "Script.h"
#include "Statement.h"
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
void analyseSlot(Id *id,	/* IN - Identifier with source position */
		 Slot *slot)	/* IN - The slot to analyse */
#else
void analyseSlot(id, slot)
     Id *id;
     Slot *slot;
#endif
{
  List *list, *sentinel = NULL;
  Symbol *symbol;
  Bool remove;			/* Forced to remove the item? */
  List *localAttributes, *inhertitedAttributeList;
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
	  analyseSlot(symbol->info.class->id, symbol->info.class->slot);
	/* Now collect the inhertied attributes into the list */
	if (symbol->info.class->slot->attributes != NULL)
	  inhertitedAttributeList =
	    prepend(symbol->info.class->slot->attributes,
		    copyList(symbol->info.class->slot->inheritedAttributeLists));
	else
	  inhertitedAttributeList =
	    copyList(symbol->info.class->slot->inheritedAttributeLists);
	/* Now inhertiedAttributes contains all lists of attributes inherited
	   from this class */
	if (inhertitedAttributeList != NULL)
	  slot->inheritedAttributeLists =
	    combine(slot->inheritedAttributeLists, inhertitedAttributeList);
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
    attribute = findAttributeInLists(&id->srcp, localAttributes->element.attribute->id,
				     slot->inheritedAttributeLists);
    if (attribute == NULL)
      localAttributes->element.attribute->code = ++attributeCount;
    else if (!equalTypes(localAttributes->element.attribute->type, attribute->type))
      lmLog(&localAttributes->element.attribute->srcp, 228, sevERR, NULL);
    else
      localAttributes->element.attribute->code = attribute->code;
  }

  /* 4f - Analyse the name */

  /* 4f - Analyse the where */

  /* Analyse the container */
  if (slot->container && !anyIsA(slot->heritage, "container"))
    lmLogv(&slot->container->srcp, 223, sevERR, id->string, "container", NULL);
  else
    analyseContainer(slot->container);

  /* Analyse the surroundings */
  if (slot->surroundings && !anyIsA(slot->heritage, "location"))
    lmLogv(&slot->surroundings->element.statement->srcp, 223, sevERR, id->string, "location", NULL);
  else
    /* 4f - Analyse the surroundings statements */

  /* 4f - Analyse the description */
  /* 4f - Analyse the mentioned */
  /* 4f - Analyse the does */

  /* Analyse the exits */
  if (slot->exits && !anyIsA(slot->heritage, "location"))
    lmLogv(&slot->exits->element.exit->srcp, 223, sevERR, id->string, "location", NULL);
  else
    /* 4f - Analyse the exits */;

  /* 4f - Analyse the verbs */

  /* Analyse the scripts */
  /* 4f - find and resolve inherited scripts, as above for attributes */
  if (slot->scripts && !anyIsA(slot->heritage, "actor"))
    lmLogv(&slot->scripts->element.script->srcp, 223, sevERR, id->string, "actor", NULL);
  else
    /* 4f - Analyse the scripts */;

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

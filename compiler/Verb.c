/*----------------------------------------------------------------------*\

				Verb.c

			      Verb Nodes

\*----------------------------------------------------------------------*/


#include "Verb.h"
#include "Statement.h"
#include "Symbol.h"

#include "lmList.h"
#include "dump.h"




/*======================================================================

  newVerb()

  Allocates and initialises a Verb.

 */
#ifdef _PROTOTYPES_
Verb *newVerb(Srcp *srcp,	/* IN - Source position */
	      List *ids,	/* IN - List of verb ids to perform action */
	      List *alternatives) /* IN - The alternative bodies */
#else
Verb *newVerb(srcp, ids, alternatives)
     Srcp *srcp;
     List *ids;
     List *alternatives;
#endif
{
  Verb *new;			/* The newly allocated area */
  List *list;
  Symbol *symbol;

  new = NEW(Verb);

  new->srcp = *srcp;
  new->ids = ids;
  new->alternatives = alternatives;

  /* Look up all ids in a VERB and enter them as verb symbols */
  for (list = ids; list; list = list->next) {
    symbol = lookup(list->element.id->string);
    if (symbol == NULL)
      symbol = newSymbol(list->element.id, VERB_SYMBOL);
    else if (symbol->kind != ERROR_SYMBOL)
      if (symbol->kind != VERB_SYMBOL) {
	symbol->kind = ERROR_SYMBOL;
	lmLogv(&list->element.id->srcp, 300, sevERR, list->element.id->string,
	       symbolKindString(VERB_SYMBOL), NULL);
      }
  }

  return(new);
}


/*======================================================================

  findVerb()

  Find an verb in a list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Verb *findVerb(Id *id,		/* IN - The verb id to find */
	       List *verbs)	/* IN - List of verb nodes */
#else
Verb *findVerb(id, verbs)
     Id *id;
     List *verbs;
#endif
{
  List *list;
  List *ids;

  for (list = verbs; list; list = list->next)
    for (ids = list->element.verb->ids; ids; ids = ids->next)
      if (equalIds(id, ids->element.id))
	return (list->element.verb);
  return NULL;
}



/*======================================================================

  findVerbInLists()

  Find an verb in a list of verb list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Verb *findVerbInLists(Srcp *srcp, /* IN - The source position to point to in case of error */
		      Id *id,	/* IN - The verb id to find */
		      List *lists) /* IN - The lists of verb lists */
#else
Verb *findVerbInLists(srcp, id, lists)
     Srcp *srcp;
     Id *id;
     List *lists;
#endif
{
  List *list;
  Verb *found1 = NULL, *found2 = NULL;

  for (list = lists; list; list = list->next) {
    found1 = findVerb(id, list->element.list);
    if (found2)
      if (found1 != found2) {
	lmLogv(srcp, 229, sevERR, "verb", id->string);
	return (found1);
      }
    found2 = found1;
  }
  return (found1);
}



/*======================================================================

  inheritVerbs()

  Inherit verbs from a slot, and update the list 

 */
#ifdef _PROTOTYPES_
void inheritVerbs(Slot *slot, /* IN - The slot to inherit from */
		  List **verbListsP) /* INOUT - Combine with this */
#else
void inheritVerbs(slot, verbListsP)
     Slot *slot;
     List **verbListsP;
#endif
{
  List *inheritedVerbList;

  if (slot->verbs != NULL)
    inheritedVerbList = prepend(slot->verbs,
				copyList(slot->inheritedVerbLists));
  else
    inheritedVerbList = copyList(slot->inheritedVerbLists);
  /* Now the inheritedVerbList contains all lists of verbs
     inherited from this class */
  if (inheritedVerbList != NULL)
    *verbListsP = combine(*verbListsP, inheritedVerbList);
}



/*======================================================================

  analyseVerb()

  Analyze a VERB.

 */
#ifdef _PROTOTYPES_
void analyseVerb(Verb *verb, List *parameters)
#else
void analyseVerb(verb, parameters)
     Verb *verb;
     List *parameters;
#endif
{
  /* 4f - Analyse verb */
}



/*======================================================================

  analyseVerbs()

  Analyze a list of VERBs.

 */
#ifdef _PROTOTYPES_
void analyseVerbs(List *verbs, List *parameters)
#else
void analyseVerbs(verbs, parameters)
     List *verbs;
     List *parameters;
#endif
{
  List *list, *list2, *ids, *other;
  

  for (list = verbs; list != NULL; list = list->next) {
    /* Analyse the verb */
    analyseVerb(list->element.verb, parameters);
    /* Verify that the ids are not multiply defined */
    for (ids = list->element.verb->ids; ids; ids = ids->next) {
      /* In this VERB ? */
      for (other = ids->next; other; other = other->next)
	if (equalIds(ids->element.id, other->element.id)) {
	  lmLog(&other->element.id->srcp, 204, sevERR, other->element.id->string);
	  break;
	}
      /* Then the ids in the other VERBs */
      for (list2 = list->next; list2; list2 = list2->next)
	  for (other = list2->element.verb->ids; other; other = other->next)
	    if (equalIds(ids->element.id, other->element.id)) {
	      lmLog(&other->element.id->srcp, 205, sevERR, other->element.id->string);
	      break;
	    }
    }
  }
}



/*======================================================================

  dumpVerb()

  Dump a Verb node.

 */
#ifdef _PROTOTYPES_
void dumpVerb(Verb *verb)
#else
void dumpVerb(verb)
     Verb *verb;
#endif
{
  if (verb == NULL) {
    put("NULL");
    return;
  }

  put("VERB: "); dumpSrcp(&verb->srcp); in();
  put("ids: "); dumpList(verb->ids, ID_NODE); nl();
  put("alternatives: "); dumpList(verb->alternatives, ALTERNATIVE_NODE); out();
}


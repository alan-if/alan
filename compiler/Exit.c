/*----------------------------------------------------------------------*\

				Exit.c

			      Exit Nodes

\*----------------------------------------------------------------------*/


#include "Exit.h"

#include "Check.h"
#include "Statement.h"
#include "Symbol.h"

#include "dump.h"
#include "lmList.h"




/*======================================================================

  newExit()

  Allocates and initialises a Exit.

 */
#ifdef _PROTOTYPES_
Exit *newExit(Srcp *srcp,	/* IN - Source Position */
	      List *directions,	/* IN - The directions */
	      Id *to,		/* IN - Where to? */
	      List *checks,	/* IN - List of checks */
	      Does *does)	/* IN - The does clause */
#else
Exit *newExit(srcp, directions, to, checks, does)
     Srcp *srcp;
     List *directions;
     Id *to;
     List *checks;
     Does *does;
#endif
{
  Exit *new;			/* The newly allocated area */
  Symbol *symbol;
  List *list;

  new = NEW(Exit);

  new->srcp = *srcp;
  new->directions = directions;
  new->to = to;
  new->checks = checks;
  new->does = does;

  /* Look up all directions in a exit and enter them as direction symbols */
  for (list = directions; list; list = list->next) {
    symbol = lookup(list->element.id->string);
    if (symbol == NULL)
      symbol = newSymbol(list->element.id, DIRECTION_SYMBOL);
    else if (symbol->kind != ERROR_SYMBOL)
      if (symbol->kind != DIRECTION_SYMBOL) {
	symbol->kind = ERROR_SYMBOL;
	lmLogv(&list->element.id->srcp, 300, sevERR, list->element.id->string,
	       symbolKindString(DIRECTION_SYMBOL), NULL);
      }
  }

  return(new);
}


/*======================================================================

  findExit()

  Find an exit in a list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Exit *findExit(Id *id,		/* IN - The exit id to find */
	       List *exits)	/* IN - List of exit nodes */
#else
Exit *findExit(id, exits)
     Id *id;
     List *exits;
#endif
{
  List *list;
  List *direction;

  for (list = exits; list; list = list->next)
    for (direction = list->element.exit->directions; direction; direction = direction->next)
      if (equalIds(id, direction->element.id))
	return (list->element.exit);
  return NULL;
}



/*======================================================================

  findExitInLists()

  Find an exit in a list of exit list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Exit *findExitInLists(Srcp *srcp, /* IN - The source position to point to in case of error */
		      Id *id,	/* IN - The exit id to find */
		      List *lists) /* IN - The lists of exit lists */
#else
Exit *findExitInLists(srcp, id, lists)
     Srcp *srcp;
     Id *id;
     List *lists;
#endif
{
  List *list;
  Exit *found1 = NULL, *found2 = NULL;

  for (list = lists; list; list = list->next) {
    found1 = findExit(id, list->element.list);
    if (found2)
      if (found1 != found2) {
	lmLogv(srcp, 229, sevERR, "exit", id->string);
	return (found1);
      }
    found2 = found1;
  }
  return (found1);
}



/*======================================================================

  inheritExits()

  Inherit exits from a slot, and update the list 

 */
#ifdef _PROTOTYPES_
void inheritExits(Slot *slot, /* IN - The slot to inherit from */
		  List **exitListsP) /* INOUT - Combine with this */
#else
void inheritExits(slot, exitListsP)
     Slot *slot;
     List **exitListsP;
#endif
{
  List *inheritedExitList;

  if (slot->exits != NULL)
    inheritedExitList = prepend(slot->exits,
				copyList(slot->inheritedExitLists));
  else
    inheritedExitList = copyList(slot->inheritedExitLists);
  /* Now the inheritedExitList contains all lists of exits
     inherited from this class */
  if (inheritedExitList != NULL)
    *exitListsP = combine(*exitListsP, inheritedExitList);
}



/*----------------------------------------------------------------------

  analyseExit()

  Analyze an exit.

 */
#ifdef _PROTOTYPES_
static void analyseExit(Exit *exit)
#else
static void analyseExit(exit)
     Exit *exit;
#endif
{
  /* Analyse an exit */
  if (symbolCheck(exit->to, INSTANCE_SYMBOL))
    classCheck(exit->to, "location");

  analyseChecks(exit->checks, NULL);
  analyseDoes(exit->does, NULL);
}



/*======================================================================

  analyseExits()

  Analyze all exits in a list.

 */
#ifdef _PROTOTYPES_
void analyseExits(List *exits)
#else
void analyseExits(exits)
     List *exits;
#endif
{
  List *list, *list2, *directions, *other;
  

  for (list = exits; list != NULL; list = list->next) {
    /* Analyse the exit */
    analyseExit(list->element.exit);
    /* Verify that the directions are not multiply defined */
    for (directions = list->element.exit->directions; directions; directions = directions->next) {
      /* In this EXIT ? */
      for (other = directions->next; other; other = other->next)
	if (equalIds(directions->element.id, other->element.id)) {
	  lmLog(&other->element.id->srcp, 202, sevERR, other->element.id->string);
	  break;
	}
      /* Then the directions in the other EXITs */
      for (list2 = list->next; list2; list2 = list2->next)
	  for (other = list2->element.exit->directions; other; other = other->next)
	    if (equalIds(directions->element.id, other->element.id)) {
	      lmLog(&other->element.id->srcp, 202, sevERR, other->element.id->string);
	      break;
	    }
    }
  }
}



/*======================================================================

  dumpExit()

  Dump a Exit node.

 */
#ifdef _PROTOTYPES_
void dumpExit(Exit *exit)
#else
void dumpExit(exit)
     Exit *exit;
#endif
{
  if (exit == NULL) {
    put("NULL");
    return;
  }

  put("EXIT: "); in();
  put("directions: "); dumpList(exit->directions, ID_NODE); nl();
  put("to: "); dumpId(exit->to); nl();
  put("checks: "); dumpList(exit->checks, CHECK_NODE); nl();
  put("does: "); dumpDoes(exit->does); out();
}

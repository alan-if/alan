/*----------------------------------------------------------------------*\

				Script.c

			      Script Nodes

\*----------------------------------------------------------------------*/


#include "Script.h"
#include "Statement.h"
#include "Step.h"

#include "lmList.h"
#include "dump.h"


/* Public: */

int scriptCount;




/*======================================================================

  newScript()

  Allocates and initialises a Script.

 */
#ifdef _PROTOTYPES_
Script *newScript(Srcp *srcp,	/* IN - Source position */
		  Id *id,	/* IN - Name of Script */
		  List *description, /* IN - Description statements */
		  List *steps) /* IN - Steps */
#else
Script *newScript(srcp, id, description, steps)
     Srcp *srcp;
     Id *id;
     List *description;
     List *steps;
#endif
{
  Script *new;			/* The newly allocated area */

  new = NEW(Script);

  new->srcp = *srcp;
  new->id = id;
  new->code = 0;		/* Not numbered yet! */
  new->description = description;
  new->steps = steps;

  return(new);
}



/*======================================================================

  findScript()

  Find an script in a list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Script *findScript(Id *id,	/* IN - The script id to find */
		   List *scripts) /* IN - List of script nodes */
#else
Script *findScript(id, scripts)
     Id *id;
     List *scripts;
#endif
{
  List *list;

  for (list = scripts; list; list = list->next)
    if (equalIds(id, list->element.script->id))
      return (list->element.script);
  return NULL;
}



/*======================================================================

  findScriptInLists()

  Find an script in a list of script list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Script *findScriptInLists(Srcp *srcp, /* IN - The source position to point to in case of error */
			  Id *id, /* IN - The script id to find */
			  List *lists) /* IN - The lists of script lists */
#else
Script *findScriptInLists(srcp, id, lists)
     Srcp *srcp;
     Id *id;
     List *lists;
#endif
{
  List *list;
  Script *found1 = NULL, *found2 = NULL;

  for (list = lists; list; list = list->next) {
    found1 = findScript(id, list->element.list);
    if (found2)
      if (found1 != found2) {
	lmLogv(srcp, 229, sevERR, "script", id->string);
	return (found1);
      }
    found2 = found1;
  }
  return (found1);
}



/*======================================================================

  inheritScripts()

  Inherit scripts from a slot, and update the list 

 */
#ifdef _PROTOTYPES_
void inheritScripts(Slot *slot, /* IN - The slot to inherit from */
		    List **scriptListsP) /* INOUT - Combine with this */
#else
void inheritScripts(slot, scriptListsP)
     Slot *slot;
     List **scriptListsP;
#endif
{
  List *inheritedScriptList;

  if (slot->scripts != NULL)
    inheritedScriptList = prepend(slot->scripts,
				     copyList(slot->inheritedScriptLists));
  else
    inheritedScriptList = copyList(slot->inheritedScriptLists);
  /* Now the inheritedScriptList contains all lists of scripts
     inherited from this class */
  if (inheritedScriptList != NULL)
    *scriptListsP = combine(*scriptListsP, inheritedScriptList);
}



/*----------------------------------------------------------------------

  analyseScript()

  Analyze a SCRIPT.

 */
#ifdef _PROTOTYPES_
static void analyseScript(Script *script)
#else
static void analyseScript(script)
     Script *script;
#endif
{
  analyseStatements(script->description, NULL, NULL);
  analyseSteps(script->steps);
}



/*======================================================================

  analyseScripts()

  Analyse all entries in an script list. Remove duplicates.
  When entity types of scripts are introduced, also register the
  class.

 */
#ifdef _PROTOTYPES_
void analyseScripts(List *scripts) /* IN - List of script nodes */
#else
void analyseScripts(scripts)
     List *scripts;
#endif
{
  List *list, *next, *sentinel;
  Bool remove;

  for (list = scripts; list; list = list->next) {
    sentinel = list;
    analyseScript(list->element.script); /* Analyse the script in itself */
    for (next = list->next; next; next = next->next) {
      /* Check for duplicates */
      remove = FALSE;
      if (equalIds(next->element.script->id, list->element.script->id)) {
	lmLogv(&next->element.script->id->srcp, 218, sevERR, "script",
	       list->element.script->id->string, NULL);
	remove = TRUE;
      }
      if (remove) {
	sentinel->next = next->next; /* Unlink the item */
      } else
	sentinel = next;
    }
  }
}



/*======================================================================

  dumpScript()

  Dump a Script node.

 */
#ifdef _PROTOTYPES_
void dumpScript(Script *script)
#else
void dumpScript(script)
     Script *script;
#endif
{
  if (script == NULL) {
    put("NULL");
    return;
  }

  put("SCRIPT: "); dumpSrcp(&script->srcp); in();
  put("id: "); dumpId(script->id); nl();
  put("description: "); dumpList(script->description, STATEMENT_NODE); nl();
  put("steps: "); dumpList(script->steps, STEP_NODE); out();
}

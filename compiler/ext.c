/*----------------------------------------------------------------------*\

                               EXT.C
                            Exit Nodes

\*----------------------------------------------------------------------*/

#include "ext_x.h"

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "chk_x.h"
#include "wrd_x.h"
#include "dump_x.h"

#include "elm.h"                /* ELM-nodes */

#include "emit.h"
#include "lmList.h"
#include "../interpreter/acode.h"


/* PUBLIC: */

int dirmin, dirmax;
int dircount = 0;



/*======================================================================

  newext()

  Allocates and initialises an extnod.

 */
ExtNod *newext(Srcp *srcp,	/* IN - Source Position */
	       List *dirs,	/* IN - Directions of this exit */
	       IdNode *target,	/* IN - Name of the location it leads to */
	       List *chks,	/* IN - List of checks to perform first */
	       List *stms)	/* IN - The statements to execute */
{
  ExtNod *new;			/* The newly created node */
  Symbol *sym;
  List *lst;			/* Traversal pointer */

  showProgress();

  new = NEW(ExtNod);

  new->srcp = *srcp;
  new->dirs = dirs;
  new->target = target;
  new->chks = chks;
  new->stms = stms;

  for (lst = dirs; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find any earlier definition */
    if (sym == NULL) {
      lst->element.id->symbol = newSymbol(lst->element.id, DIRECTION_SYMBOL);
      lst->element.id->code = lst->element.id->symbol->code;
      newWord(lst->element.id->string, DIRECTION_WORD,
	      lst->element.id->symbol->code, NULL);
    } else if (sym->kind == DIRECTION_SYMBOL) {
      lst->element.id->symbol = sym;
      lst->element.id->code = lst->element.id->symbol->code;
    } else
      redefined(lst->element.id, sym);
  }

  return(new);
}



/*-----------------------------------------------------------------------

  symbolizeExit()

*/
static void symbolizeExit(ExtNod *theExit)
{
  symbolizeId(theExit->target);
#ifdef FIXME
  symbolizeChecks(theExit->checks);
  symbolizeStatements(theExit->does);
#endif
}


/*======================================================================

  symbolizeExits()

*/
void symbolizeExits(List *theExitList)
{
  List *lst;

  for (lst = theExitList; lst != NULL; lst = lst->next)
    symbolizeExit(lst->element.ext);
}


/*----------------------------------------------------------------------

  analyzeExit()

 */
static void analyzeExit(ExtNod *ext, Context *context)
{
  inheritCheck(ext->target, "Target of an Exit", "an instance", "location");

  analyzeChecks(ext->chks, context);
  analyzeStatements(ext->stms, context);
}



/*======================================================================

  analyzeExits()

  Analyzes all exits in a list by calling the exit analyzer for all
  exits.

 */
void analyzeExits(List *exts, Context *context)
{
  List *ext, *dir, *lst, *other;

  for (lst = exts; lst != NULL; lst = lst->next)
    analyzeExit(lst->element.ext, context);

  /* Check for multiple definitions of a direction */
  for (ext = exts; ext != NULL; ext = ext->next) {
    dir = ext->element.ext->dirs;
    /* First check other directions in this EXIT */
    for (other = dir->next; other != NULL; other = other->next) {
      if (other->element.id->symbol->code == dir->element.id->symbol->code) {
	lmLog(&other->element.id->srcp, 202, sevWAR,
	      other->element.id->string);
	break;
      }
    }
    /* Then the directions in the other EXITs */
    for (lst = ext->next; lst != NULL; lst = lst->next) {
      for (other = lst->element.ext->dirs; other != NULL; other = other->next)
	if (other->element.id->symbol->code == dir->element.id->symbol->code) {
	  lmLog(&other->element.id->srcp, 203, sevWAR,
		other->element.id->string);
	  break;
	}
    }
  }
}



/*----------------------------------------------------------------------*/
static Aaddr generateExitStatements(ExtNod *ext)
{
  Aaddr stmadr = nextEmitAddress();

  if (ext->stms == NULL)
    return(0);
  
  generateStatements(ext->stms);
  emit0(I_RETURN);
  return(stmadr);
}



/*----------------------------------------------------------------------

  generateExitEntry()

  Generate one exit entry in the exit table.

 */
static void generateExitEntry(ExtNod *ext) /* IN - The exit to generate */
{
  List *dir;
  ExitEntry entry;

  for (dir = ext->dirs; dir != NULL; dir = dir->next) {
    entry.code = dir->element.id->symbol->code;
    entry.checks = ext->chks? ext->chkadr : 0;
    entry.action = ext->stms? ext->stmadr : 0;
    entry.target = ext->target->symbol->code;
    emitEntry(&entry, sizeof(entry));
  }
}



/*======================================================================*/
Aaddr generateExits(List *exits)
{
  List *lst;			/* Traversal pointer */
  Aaddr extadr;			/* The adress where the exits start */
  
  if (exits == NULL)
    return(0);

  for (lst = exits; lst != NULL; lst = lst->next) {
    lst->element.ext->chkadr = nextEmitAddress();
    if (lst->element.ext->chks != NULL)
      lst->element.ext->chkadr = generateChecks(lst->element.ext->chks);
    lst->element.ext->stmadr = generateExitStatements(lst->element.ext);
    emit0(I_RETURN);
  }
  
  extadr = nextEmitAddress();
  for (lst = exits; lst != NULL; lst = lst->next)
    generateExitEntry(lst->element.ext);
  emit(EOF);
  return(extadr);
}




/*======================================================================

  dumpExit()

  Dump an Exit node.

 */
void dumpExit(ExtNod *ext)
{
  if (ext == NULL) {
    put("NULL");
    return;
  }

  put("EXT: "); dumpSrcp(&ext->srcp); indent();
  put("dirs: "); dumpList(ext->dirs, ID_LIST); nl();
  put("target: "); dumpId(ext->target); nl();
  put("chks: "); dumpList(ext->chks, CHECK_LIST); nl();
  put("chkadr: "); dumpAddress(ext->chkadr); nl();
  put("stms: "); dumpList(ext->stms, STATEMENT_LIST); nl();
  put("stmadr: "); dumpAddress(ext->stmadr); out();
}



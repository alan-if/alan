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
#include "wrd_x.h"

#include "chk.h"                /* CHK-nodes */
#include "elm.h"                /* ELM-nodes */

#include "emit.h"
#include "lmList.h"
#include "acode.h"
#include "dump.h"


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
  SymNod *sym;
  List *lst;			/* Traversal pointer */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ExtNod);

  new->srcp = *srcp;
  new->dirs = dirs;
  new->target = target;
  new->chks = chks;
  new->stms = stms;

  for (lst = dirs; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find any earlier definition */
    if (sym == NULL) {
      lst->element.id->symbol = newSymbol(lst->element.id->string, DIRECTION_SYMBOL);
      lst->element.id->code = lst->element.id->symbol->code;
      newwrd(lst->element.id->string, WRD_DIR, lst->element.id->symbol->code, NULL);
    } else if (sym->kind == DIRECTION_SYMBOL) {
      lst->element.id->symbol = sym;
      lst->element.id->code = lst->element.id->symbol->code;
    } else
      redefined(&lst->element.id->srcp, sym, lst->element.id->string);
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
static void analyzeExit(ExtNod *ext)	/* IN - Exit to analyze */
{
  inheritCheck(ext->target, "an instance", "location");

  anchks(ext->chks, NULL);
  anstms(ext->stms, NULL);
}



/*======================================================================

  analyzeExits()

  Analyzes all exits in a list by calling the exit analyzer for all
  exits.

 */
void analyzeExits(List *exts)		/* IN - List of exits to analyze */
{
  List *ext, *dir, *lst, *other;

  for (lst = exts; lst != NULL; lst = lst->next)
    analyzeExit(lst->element.ext);

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



/*----------------------------------------------------------------------

  geexstms()

  Generate the stms for one exit.

 */
static Aaddr geexstms(ExtNod *ext) /* IN - The exit to generate */
{
  Aaddr stmadr = emadr();

  if (ext->stms == NULL)
    return(0);
  
  gestms(ext->stms, NULL);
  emit0(C_STMOP, I_RETURN);
  return(stmadr);
}



/*----------------------------------------------------------------------

  generateExitEntry()

  Generate one exit entry in the exit table.

 */
static void generateExitEntry(ExtNod *ext) /* IN - The exit to generate */
{
  List *dir;
  Bool same = FALSE;
  ExitEntry entry;

  for (dir = ext->dirs; dir != NULL; dir = dir->next) {
    entry.done = same;			/* For reversing process */
    entry.code = dir->element.id->symbol->code;
    entry.checks = ext->chks? ext->chkadr : 0;
    entry.action = ext->stms? ext->stmadr : 0;
    entry.target = ext->target->symbol->code;
    same = TRUE;
    emitEntry(&entry, sizeof(entry));
  }
}



/*======================================================================

  generateExits()

  Generate the data structure for the exits for a location.

 */
Aaddr generateExits(List *exts)	/* IN - The exits */
{
  List *lst;			/* Traversal pointer */
  Aaddr extadr;			/* The adress where the exits start */
  
  if (exts == NULL)
    return(0);

  for (lst = exts; lst != NULL; lst = lst->next) {
    lst->element.ext->chkadr = emadr();
    if (lst->element.ext->chks != NULL)
      lst->element.ext->chkadr = gechks(lst->element.ext->chks, NULL);
    lst->element.ext->stmadr = geexstms(lst->element.ext);
    emit0(C_STMOP, I_RETURN);
  }
  
  extadr = emadr();
  for (lst = exts; lst != NULL; lst = lst->next)
    generateExitEntry(lst->element.ext);
  emit(EOF);
  return(extadr);
}




/*======================================================================

  duext()

  Dump an Exit node.

 */
void duext(ExtNod *ext)
{
  if (ext == NULL) {
    put("NULL");
    return;
  }

  put("EXT: "); dumpSrcp(&ext->srcp); in();
  put("dirs: "); dulst(ext->dirs, LIST_ID); nl();
  put("target: "); dumpId(ext->target); nl();
  put("chks: "); dulst(ext->chks, LIST_CHK); nl();
  put("chkadr: "); duadr(ext->chkadr); nl();
  put("stms: "); dulst(ext->stms, LIST_STM); nl();
  put("stmadr: "); duadr(ext->stmadr); out();
}



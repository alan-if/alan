/*----------------------------------------------------------------------*\

                               EXT.C
                            Exit Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"		/* STM-nodes */
#include "chk.h"                /* CHK-nodes */
#include "elm.h"                /* ELM-nodes */
#include "ext.h"                /* EXT-nodes */
#include "wrd.h"                /* WRD-nodes */
#include "ins.h"                /* INS-nodes */

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
	       List *dirs,	/* IN - Directions of this ext */
	       IdNode *to,	/* IN - Name of the location it leads to */
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
  new->to = to;
  new->chks = chks;
  new->stms = stms;

  for (lst = dirs; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find any earlier definition */
#ifndef FIXME
    syserr("UNIMPL: newext() - symbol codes and words");
#else
    if (sym == NULL) {
      lst->element.id->code = newsym(lst->element.id->string, NAMDIR, new);
      newwrd(lst->element.id->string, WRD_DIR, lst->element.id->code, NULL);
    } else if (sym->class == NAMDIR)
      lst->element.id->code = sym->code;
    else
      redefined(&lst->element.id->srcp, sym, lst->element.id->string);
#endif
  }

  return(new);
}



/*----------------------------------------------------------------------

  anext()

  Analyzes one exit.

 */
static void anext(ExtNod *ext)	/* IN - Exit to analyze */
{
  SymNod *sym;			/* Symbol table entry */
  ElmNod *elm;

  sym = symcheck(&elm, ext->to->string, INSTANCE_SYMBOL, NULL);
  syserr("UNIMPL: check that an instance is a location");

  anchks(ext->chks, NULL, NULL);
  anstms(ext->stms, NULL, NULL, NULL);
}



/*======================================================================

  anexts()

  Analyzes all exits in a list by calling the exit analyzer for all
  exits.

 */
void anexts(List *exts)		/* IN - List of exits to analyze */
{
  List *ext, *dir, *lst, *other;

  for (lst = exts; lst != NULL; lst = lst->next)
    anext(lst->element.ext);

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

  geextent()

  Generate one exit in an exit list.

 */
static void geextent(ExtNod *ext) /* IN - The exit to generate */
{
  List *dir;
  Bool same = FALSE;
  
  for (dir = ext->dirs; dir != NULL; dir = dir->next) {
    emit(same);			/* For reversing process */
    emit(dir->element.id->symbol->code);

    if (ext->chks != NULL)
      emit(ext->chkadr);
    else
      emit(0);

    if (ext->stms != NULL)
      emit(ext->stmadr);
    else
      emit(0);

    geid(ext->to);
    same = TRUE;
  }
}



/*======================================================================

  geexts()

  Generate the data structure for the exits for a location.

 */
Aaddr geexts(List *exts)	/* IN - The exits */
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
    geextent(lst->element.ext);
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
  put("to: "); dumpId(ext->to); nl();
  put("chks: "); dulst(ext->chks, LIST_CHK); nl();
  put("chkadr: "); duadr(ext->chkadr); nl();
  put("stms: "); dulst(ext->stms, LIST_STM); nl();
  put("stmadr: "); duadr(ext->stmadr); out();
}



/*----------------------------------------------------------------------*\

                               EXT.C
                            Exit Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"		/* STM-nodes */
#include "chk.h"                /* CHK-nodes */
#include "elm.h"                /* ELM-nodes */
#include "ext.h"                /* EXT-nodes */
#include "wrd.h"                /* WRD-nodes */
#include "loc.h"                /* LOC-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */

int dirmin, dirmax;
int dircount = 0;



/*======================================================================

  newext()

  Allocates and initialises an extnod.

 */
#ifdef _PROTOTYPES_
ExtNod *newext(Srcp *srcp, List *dirs, NamNod *to, List *chks, List *stms)
                		/* IN - Source Position */
                		/* IN - Directions of this ext */
                		/* IN - Name of the location it leads to */
                		/* IN - List of checks to perform first */
                		/* IN - The statements to execute */
#else
ExtNod *newext(srcp, dirs, to, chks, stms)
     Srcp *srcp;		/* IN - Source Position */
     List *dirs;		/* IN - Directions of this ext */
     NamNod *to;		/* IN - Name of the location it leads to */
     List *chks;		/* IN - List of checks to perform first */
     List *stms;		/* IN - The statements to execute */
#endif
{
  ExtNod *new;			/* The newly created node */
  SymNod *sym;
  List *lst;			/* Traversal pointer */

  new = NEW(ExtNod);

  new->srcp = *srcp;
  new->dirs = dirs;
  new->to = to;
  new->chks = chks;
  new->stms = stms;

  for (lst = dirs; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.nam->str); /* Find any earlier definition */
    if (sym == NULL) {
      lst->element.nam->code = newsym(lst->element.nam->str, NAMDIR, new);
      newwrd(lst->element.nam->str, WRD_DIR, lst->element.nam->code, NULL);
    } else if (sym->class == NAMDIR)
      lst->element.nam->code = sym->code;
    else
      redefined(&lst->element.nam->srcp, sym, lst->element.nam->str);
  }

  return(new);
}



/*----------------------------------------------------------------------

  anext()

  Analyzes one exit.

 */
#ifdef _PROTOTYPES_
static void anext(ExtNod *ext)
                 		/* IN - Exit to analyze */
#else
static void anext(ext)
     ExtNod *ext;		/* IN - Exit to analyze */
#endif
{
  SymNod *sym;			/* Symbol table entry */
  ElmNod *elm;

  symcheck(&sym, &elm, ext->to, NAMLOC, NAMANY, NULL);

  anchks(ext->chks, NULL, NULL);
  anstms(ext->stms, NULL, NULL, NULL);
}



/*======================================================================

  anexts()

  Analyzes all exits in a list by calling the exit analyzer for all
  exits.

 */
#ifdef _PROTOTYPES_
void anexts(List *exts)
                		/* IN - List of exits to analyze */
#else
void anexts(exts)
     List *exts;		/* IN - List of exits to analyze */
#endif
{
  List *ext, *dir, *lst, *other;

  for (lst = exts; lst != NULL; lst = lst->next)
    anext(lst->element.ext);

  /* Check for multiple definitions of a direction */
  for (ext = exts; ext != NULL; ext = ext->next) {
    dir = ext->element.ext->dirs;
    /* First check other directions in this EXIT */
    for (other = dir->next; other != NULL; other = other->next) {
      if (other->element.nam->code == dir->element.nam->code) {
	lmLog(&other->element.nam->srcp, 202, sevWAR,
	      other->element.nam->str);
	break;
      }
    }
    /* Then the directions in the other EXITs */
    for (lst = ext->next; lst != NULL; lst = lst->next) {
      for (other = lst->element.ext->dirs; other != NULL; other = other->next)
	if (other->element.nam->code == dir->element.nam->code) {
	  lmLog(&other->element.nam->srcp, 203, sevWAR,
		other->element.nam->str);
	  break;
	}
    }
  }
}



/*----------------------------------------------------------------------

  geexstms()

  Generate the stms for one exit.

 */
#ifdef _PROTOTYPES_
static Aaddr geexstms(ExtNod *ext)
                 		/* IN - The exit to generate */
#else
static Aaddr geexstms(ext)
     ExtNod *ext;		/* IN - The exit to generate */
#endif
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
#ifdef _PROTOTYPES_
static void geextent(ExtNod *ext)
                 		/* IN - The exit to generate */
#else
static void geextent(ext)
     ExtNod *ext;		/* IN - The exit to generate */
#endif
{
  List *dir;
  Bool same = FALSE;
  
  for (dir = ext->dirs; dir != NULL; dir = dir->next) {
    emit(same);			/* For reversing process */
    emit(dir->element.nam->code);

    if (ext->chks != NULL)
      emit(ext->chkadr);
    else
      emit(0);

    if (ext->stms != NULL)
      emit(ext->stmadr);
    else
      emit(0);

    genam(ext->to);
    same = TRUE;
  }
}



/*======================================================================

  geexts()

  Generate the data structure for the exits for a location.

 */
#ifdef _PROTOTYPES_
Aaddr geexts(List *exts)
                		/* IN - The exits */
#else
Aaddr geexts(exts)
     List *exts;		/* IN - The exits */
#endif
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
#ifdef _PROTOTYPES_
void duext(ExtNod *ext)
#else
void duext(ext)
     ExtNod *ext;
#endif
{
  if (ext == NULL) {
    put("NULL");
    return;
  }

  put("EXT: "); dusrcp(&ext->srcp); in();
  put("dirs: "); dulst(ext->dirs, NAMNOD); nl();
  put("to: "); dunam(ext->to); nl();
  put("chks: "); dulst(ext->chks, CHKNOD); nl();
  put("chkadr: "); duint(ext->chkadr); nl();
  put("stms: "); dulst(ext->stms, STMNOD); nl();
  put("stmadr: "); duint(ext->stmadr); out();
}



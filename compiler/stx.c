/*----------------------------------------------------------------------*\

                               STX.C
                            Syntax Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-nodes */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stx.h"                /* STX-nodes */
#include "elm.h"                /* ELM-nodes */
#include "wrd.h"                /* WRD-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newstx()

  Allocates and initialises a syntax node.

 */
#ifdef _PROTOTYPES_
StxNod *newstx(Srcp *srcp, NamNod *nam, List *elms, List *ress)
                		/* IN - Source Position */
                 		/* IN - Name of the verb it defines */
                		/* IN - List of elements */
                		/* IN - List of class restrictions */
#else
StxNod *newstx(srcp, nam, elms, ress)
     Srcp *srcp;		/* IN - Source Position */
     NamNod *nam;		/* IN - Name of the verb it defines */
     List *elms;		/* IN - List of elements */
     List *ress;		/* IN - List of class restrictions */
#endif
{
  StxNod *new;			/* The newly created node */

  new = NEW(StxNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->elms = elms;
  new->ress = ress;

  new->generated = FALSE;

  return(new);
}



/*----------------------------------------------------------------------

  anstx()

  Analyzes one syntax node.

 */
#ifdef _PROTOTYPES_
static void anstx(StxNod *stx)
                 		/* IN - Syntax node to analyze */
#else
static void anstx(stx)
     StxNod *stx;		/* IN - Syntax node to analyze */
#endif
{
  SymNod *sym;

  /* Find which verb it defines */
  sym = lookup(stx->nam->str);	/* Find earlier definition */
  if (sym == NULL)
    lmLog(&stx->nam->srcp, 207, sevWAR, stx->nam->str);
  else if (sym->class != NAMVRB)
    lmLog(&stx->nam->srcp, 208, sevWAR, stx->nam->str);
  else {
    stx->nam->kind = NAMVRB;
    stx->nam->code = sym->code;
  }

  stx->pars = anelms(stx->elms, stx->ress, stx);
  anress(stx->ress, stx->pars);

  /* Link the last syntax element to this stx to prepare for code generation */
  stx->elms->tail->element.elm->stx = stx;
}



/*======================================================================

  anstxs()

  Analyzes all syntax nodes in the adventure by calling the analyzer for
  each one.

 */
#ifdef _PROTOTYPES_
void anstxs(void)
#else
void anstxs()
#endif
{
  List *lst, *other;

  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    anstx(lst->element.stx);

  /* Check for multiple definitions of the syntax for a verb */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    for (other = lst->next; other != NULL; other = other->next) {
      if (other->element.stx->nam->code != -1)
	if (other->element.stx->nam->code == lst->element.stx->nam->code) {
	  lmLog(&other->element.stx->nam->srcp, 206, sevWAR,
		other->element.stx->nam->str);
	  break;
	}
    }
}



/*======================================================================

  defaultStx()

  Returns the address a defaut syntax node which is used for verbs
  without any defined syntax:

  Syntax x = x (object).

 */
#ifdef _PROTOTYPES_
StxNod *defaultStx(char *vrbstr)
                   		/* IN - The string for the verb */
#else
StxNod *defaultStx(vrbstr)
     char vrbstr[];		/* IN - The string for the verb */
#endif
{
  StxNod *stx;
  List *elms;

  elms = concat(concat(NULL,
		       newelm(&nulsrcp, ELMWRD, newnam(&nulsrcp,
						       vrbstr),
			      FALSE)),
		newelm(&nulsrcp, ELMPAR, newnam(&nulsrcp, "object"), FALSE));
  stx = newstx(&nulsrcp, newnam(&nulsrcp, vrbstr), elms, NULL);

  adv.stxs = concat(adv.stxs, stx);
  anstx(stx);			/* Make sure the syntax is analysed */
  return stx;
}



/*======================================================================

  eqparams()

  Compare two syntax nodes and return true if their parameter lists are
  compatible (same ordering with same parameter names).

  */
#ifdef _PROTOTYPES_
Bool eqparams(StxNod *stx1, StxNod *stx2)
                  		/* IN - Syntax node to compare */
                  		/* IN - Syntax node to compare */
#else
Bool eqparams(stx1, stx2)
     StxNod *stx1;		/* IN - Syntax node to compare */
     StxNod *stx2;		/* IN - Syntax node to compare */
#endif
{
  List *elm1, *elm2;

  for (elm1 = stx1->pars, elm2 = stx2->pars;
       elm1 != NULL && elm2 != NULL;
       elm1 = elm1->next, elm2 = elm2->next) {
    if (!eqnams(elm1->element.elm->nam, elm2->element.elm->nam))
      return FALSE;
    if (elm1->element.elm->multiple != elm2->element.elm->multiple)
      return FALSE;
  }
  return elm1 == elm2;		/* Both NULL => equal */
}


/*----------------------------------------------------------------------

  gestx()

  Generate one syntax node.

 */
#ifdef _PROTOTYPES_
static void gestx(StxNod *stx)
                 		/* IN - Syntax node to generate for */
#else
static void gestx(stx)
     StxNod *stx;		/* IN - Syntax node to generate for */
#endif
{
  WrdNod *wrd;
  List *lst = NULL;
  List *elms = NULL;		/* A list of parallell elms-lists */

  
  if (!stx->generated) {
    /* First word is a verb which points to all stxs starting with that word */
    wrd = findwrd(stx->elms->element.elm->nam->str);
    /* Ignore words not verbs and prepositions */
    if (wrd->classbits&(1L<<WRD_PREP)) lst = wrd->ref[WRD_PREP];
    if (wrd->classbits&(1L<<WRD_VRB)) lst = wrd->ref[WRD_VRB];
    /* Create a list of all parallell elements */
    while (lst) {
      elms = concat(elms, lst->element.stx->elms);
      lst->element.stx->generated = TRUE;
      lst = lst->next;
    }
    stx->elmsadr = geelms(elms, stx);
  } else
    stx->elmsadr = 0;
}



/*----------------------------------------------------------------------

  gestxent()

  Generate a table entry for one syntax node.

 */
#ifdef _PROTOTYPES_
static void gestxent(StxNod *stx)
                 		/* IN - Syntax node to generate for */
#else
static void gestxent(stx)
     StxNod *stx;		/* IN - Syntax node to generate for */
#endif
{
  if (stx->elmsadr != 0) {
    /* The code for the verb word */
    emit(stx->elms->element.elm->nam->code);
    /* Address to syntax element tables */
    emit(stx->elmsadr);
  }
}



/*======================================================================

  gestxs()

  Generate the data structure for all syntax definitions in the \
  adventure.

 */
#ifdef _PROTOTYPES_
Aaddr gestxs(void)
#else
Aaddr gestxs()
#endif
{
  List *lst;
  Aaddr stxadr;

  /* First generate all class restriction checks */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    lst->element.stx->resadr = geress(lst->element.stx->ress, lst->element.stx);

  /* Then the actual stxs */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    gestx(lst->element.stx);

  /* Then a table of entries */
  stxadr = emadr();
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    gestxent(lst->element.stx);
  emit(EOF);
  return(stxadr);
}



/*======================================================================

  dustx()

  Dump an Syntax node.

 */
#ifdef _PROTOTYPES_
void dustx(StxNod *stx)
#else
void dustx(stx)
     StxNod *stx;
#endif
{
  if (stx == NULL) {
    put("NULL");
    return;
  }

  put("STX: "); duadr(stx); dusrcp(&stx->srcp); in();
  put("nam: "); dunam(stx->nam); nl();
  put("generated: "); duBool(stx->generated); nl();
  put("elmsadr: "); duint(stx->elmsadr); nl();
  put("elms: "); dulst(stx->elms, ELMNOD); nl();
  put("resadr: "); duint(stx->resadr); nl();
  put("ress: "); dulst(stx->ress, RESNOD); nl();
  put("pars: "); dulst(stx->pars, ELMNOD); out();
}

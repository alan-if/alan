/*----------------------------------------------------------------------*\

				CHK.C
			     Check Nodes

\*----------------------------------------------------------------------*/


#include "alan.h"

#include "lst.h"		/* LST-nodes */
#include "exp.h"                /* EXP-nodes */
#include "stm.h"		/* STM-nodes */
#include "chk.h"                /* CHK-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"




/*======================================================================

  newchk()

  Allocates and initialises a chknod.

 */
#ifdef _PROTOTYPES_
ChkNod *newchk(ExpNod *exp, List *stms)
                 		/* IN - Expression for this CHECK */
                		/* IN - Statements for a false CHECK */
#else
ChkNod *newchk(exp, stms)
     ExpNod *exp;		/* IN - Expression for this CHECK */
     List *stms;		/* IN - Statements for a false CHECK */
#endif
{
  ChkNod *new;			/* The newly allocated area */

  new = NEW(ChkNod);

  new->exp = exp;
  new->stms = stms;

  return(new);
}


/*----------------------------------------------------------------------

  anchk()

  Analyze one CHECK.

 */
#ifdef _PROTOTYPES_
static void anchk(ChkNod *chk, ActNod *act, List *pars)
                 		/* IN - Check to analyze */
                 		/* IN - Possibly inside Actor? */
                		/* IN - Possible parameters */
#else
static void anchk(chk, act, pars)
     ChkNod *chk;		/* IN - Check to analyze */
     ActNod *act;		/* IN - Possibly inside Actor? */
     List *pars;		/* IN - Possible parameters */
#endif
{
    anexp(chk->exp, NULL, pars);
    anstms(chk->stms, act, NULL, pars);
}



/*======================================================================

  anchks()

  Analyze all CHECKs in a list.

 */
#ifdef _PROTOTYPES_
void anchks(List *chks, ActNod *act, List *pars)
                		/* IN - Checks to analyze */
                 		/* IN - Possibly inside Actor? */
                		/* IN - Possible parameter list */
#else
void anchks(chks, act, pars)
     List *chks;		/* IN - Checks to analyze */
     ActNod *act;		/* IN - Possibly inside Actor? */
     List *pars;		/* IN - Possible parameter list */
#endif
{
  while (chks != NULL) {
    anchk(chks->element.chk, act, pars);
    chks = chks->next;
  }
}




/*======================================================================

  gechks()

  Generate code for the CHECKs for a verb or exit.

 */
#ifdef _PROTOTYPES_
Aword gechks(List *chks, ActNod *act)		/* RETURNS - Address to check table */
                		/* IN - The CHECKs to generate */
                 		/* IN - Inside any actor */
#else
Aword gechks(chks, act)		/* RETURNS - Address to check table */
     List *chks;		/* IN - The CHECKs to generate */
     ActNod *act;		/* IN - Inside any actor */
#endif
{
  List *lst;			/* Traversal pointer */
  Aword tbladr;			/* Save ACODE address to check table */


  /* First checks */
  if (chks->element.chk->exp == NULL) { /* An unconditional CHECK */
    chks->element.chk->expadr = 0;
    chks->element.chk->stmadr = emadr();
    gestms(chks->element.chk->stms, act);
    emit0(C_STMOP, I_RETURN);
  } else
    for (lst = chks; lst != NULL; lst = lst->next) {
      lst->element.chk->expadr = emadr();
      geexp(lst->element.chk->exp);
      emit0(C_STMOP, I_RETURN);
      lst->element.chk->stmadr = emadr();
      gestms(lst->element.chk->stms, act);
      emit0(C_STMOP, I_RETURN);
    }

  /* Then generate a check table */
  tbladr = emadr();
  for (lst = chks; lst != NULL; lst = lst->next) {
    emit(lst->element.chk->expadr);
    emit(lst->element.chk->stmadr);
  }
  emit(EOF);

  return(tbladr);
}



/*======================================================================

  duchk()

  Dump a Check node.

 */
#ifdef _PROTOTYPES_
void duchk(ChkNod *chk)
#else
void duchk(chk)
     ChkNod *chk;
#endif
{
  if (chk == NULL) {
    put("NULL");
    return;
  }

  put("CHK: "); in();
  put("exp: "); duexp(chk->exp); nl();
  put("expadr: "); duint(chk->expadr); nl();
  put("stms: "); dulst(chk->stms, STMNOD); nl();
  put("stmadr: "); duint(chk->stmadr); out();
}

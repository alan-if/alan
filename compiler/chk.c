/*----------------------------------------------------------------------*\

				CHK.C
			     Check Nodes

\*----------------------------------------------------------------------*/


#include "util.h"
#include "options.h"

#include "lst_x.h"

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
ChkNod *newchk(ExpNod *exp,	/* IN - Expression for this CHECK */
	       List *stms)	/* IN - Statements for a false CHECK */
{
  ChkNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ChkNod);

  new->exp = exp;
  new->stms = stms;

  return(new);
}


/*----------------------------------------------------------------------

  anchk()

  Analyze one CHECK.

 */
static void anchk(ChkNod *chk,	/* IN - Check to analyze */
		  InsNod *ins,	/* IN - Possibly inside Instance? */
		  List *pars)	/* IN - Possible parameters */
{
    anexp(chk->exp, NULL, pars);
    anstms(chk->stms, ins, NULL, pars);
}



/*======================================================================

  anchks()

  Analyze all CHECKs in a list.

 */
void anchks(List *chks,		/* IN - Checks to analyze */
	    InsNod *ins,	/* IN - Possibly inside Instance? */
	    List *pars)		/* IN - Possible parameter list */
{
  while (chks != NULL) {
    anchk(chks->element.chk, ins, pars);
    chks = chks->next;
  }
}




/*======================================================================

  gechks()

  Generate code for the CHECKs for a verb or exit.

 */
Aword gechks(List *chks,	/* IN - The CHECKs to generate */
	     InsNod *ins)	/* IN - Inside any Instace? */
     /* RETURNS - Address to check table */
{
  List *lst;			/* Traversal pointer */
  Aword tbladr;			/* Save ACODE address to check table */


  /* First checks */
  if (chks->element.chk->exp == NULL) { /* An unconditional CHECK */
    chks->element.chk->expadr = 0;
    chks->element.chk->stmadr = emadr();
    gestms(chks->element.chk->stms, ins);
    emit0(C_STMOP, I_RETURN);
  } else
    for (lst = chks; lst != NULL; lst = lst->next) {
      lst->element.chk->expadr = emadr();
      geexp(lst->element.chk->exp);
      emit0(C_STMOP, I_RETURN);
      lst->element.chk->stmadr = emadr();
      gestms(lst->element.chk->stms, ins);
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
void duchk(ChkNod *chk)
{
  if (chk == NULL) {
    put("NULL");
    return;
  }

  put("CHK: "); in();
  put("exp: "); duexp(chk->exp); nl();
  put("expadr: "); duadr(chk->expadr); nl();
  put("stms: "); dulst(chk->stms, LIST_STM); nl();
  put("stmadr: "); duadr(chk->stmadr); out();
}

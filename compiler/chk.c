/*----------------------------------------------------------------------*\

				CHK.C
			     Check Nodes

\*----------------------------------------------------------------------*/

#include "chk_x.h"

/* USE: */

#include "lst_x.h"
#include "exp_x.h"
#include "stm_x.h"

#include "util.h"
#include "options.h"
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
static void anchk(ChkNod *chk,
		  Context *context)
{
  anexp(chk->exp, context);
  anstms(chk->stms, context);
}



/*======================================================================

  anchks()

  Analyze all CHECKs in a list.

 */
void anchks(List *chks,
	    Context *context)
{
  while (chks != NULL) {
    anchk(chks->element.chk, context);
    chks = chks->next;
  }
}




/*======================================================================

  gechks()

  Generate code for the CHECKs for a verb or exit.

 */
Aword gechks(List *chks, int currentInstance)
{
  List *lst;			/* Traversal pointer */
  Aword tbladr;			/* Save ACODE address to check table */


  /* First checks */
  if (chks->element.chk->exp == NULL) { /* An unconditional CHECK */
    chks->element.chk->expadr = 0;
    chks->element.chk->stmadr = emadr();
    gestms(chks->element.chk->stms, currentInstance);
    emit0(C_STMOP, I_RETURN);
  } else
    for (lst = chks; lst != NULL; lst = lst->next) {
      lst->element.chk->expadr = emadr();
      geexp(lst->element.chk->exp, currentInstance);
      emit0(C_STMOP, I_RETURN);
      lst->element.chk->stmadr = emadr();
      gestms(lst->element.chk->stms, currentInstance);
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
  put("exp: "); dumpExpression(chk->exp); nl();
  put("expadr: "); dumpAddress(chk->expadr); nl();
  put("stms: "); dulst(chk->stms, LIST_STM); nl();
  put("stmadr: "); dumpAddress(chk->stmadr); out();
}

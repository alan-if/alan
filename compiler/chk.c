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
#include "../interpreter/acode.h"
#include "dump.h"




/*======================================================================

  newchk()

  Allocates and initialises a chknod.

 */
ChkNod *newchk(Expression *exp,	/* IN - Expression for this CHECK */
	       List *stms)	/* IN - Statements for a false CHECK */
{
  ChkNod *new;			/* The newly allocated area */

  showProgress();

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
  analyzeExpression(chk->exp, context);
  analyzeStatements(chk->stms, context);
}



/*======================================================================

  anchks()

  Analyze all CHECKs in a list.

 */
void analyzeChecks(List *chks,
	    Context *context)
{
  while (chks != NULL) {
    anchk(chks->element.chk, context);
    chks = chks->next;
  }
}




/*======================================================================*/
Aword generateChecks(List *chks)
{
  List *lst;			/* Traversal pointer */
  Aword tbladr;			/* Save ACODE address to check table */

  if (chks == NULL) return 0;

  /* First checks */
  if (chks->element.chk->exp == NULL) { /* An unconditional CHECK */
    chks->element.chk->expadr = 0;
    chks->element.chk->stmadr = nextEmitAddress();
    generateStatements(chks->element.chk->stms);
    emit0(I_RETURN);
  } else
    for (lst = chks; lst != NULL; lst = lst->next) {
      lst->element.chk->expadr = nextEmitAddress();
      generateExpression(lst->element.chk->exp);
      emit0(I_RETURN);
      lst->element.chk->stmadr = nextEmitAddress();
      generateStatements(lst->element.chk->stms);
      emit0(I_RETURN);
    }

  /* Then generate a check table */
  tbladr = nextEmitAddress();
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

  put("CHECK: "); indent();
  put("exp: "); dumpExpression(chk->exp); nl();
  put("expadr: "); dumpAddress(chk->expadr); nl();
  put("stms: "); dumpList(chk->stms, STATEMENT_LIST); nl();
  put("stmadr: "); dumpAddress(chk->stmadr); out();
}

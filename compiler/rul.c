/*----------------------------------------------------------------------*\

				RUL.C
			     Rules Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "adv_x.h"
#include "exp_x.h"
#include "stm_x.h"


#include "lmList.h"

#include "../interpreter/acode.h"

#include "rul.h"                /* RUL-nodes */

#include "emit.h"
#include "dump.h"


/* PUBLIC */
int rulmin, rulmax;
int rulcount = 0;



/*======================================================================

  newrul()

  Allocates and initialises a new rulnod.

  */
RulNod *newrul(Srcp *srcp,	/* IN - Source Position */
	       Expression *exp,	/* IN - Expression to wait for */
	       List *stms)	/* IN - List of statements */
{
  RulNod *new;		/* The newly allocated node */

  showProgress();

  new = NEW(RulNod);

  new->srcp = *srcp;
  new->exp  = exp;
  new->stms = stms;

  rulcount++;
  return(new);
}



/*----------------------------------------------------------------------

  analyzeRule()

  Analyze one rule

  */
static void analyzeRule(RulNod *rul)
{
  Context context;

  showProgress();

  context.kind = RULE_CONTEXT;
  analyzeExpression(rul->exp, &context);
  analyzeStatements(rul->stms, &context);
}




/*======================================================================

  analyzeRules()

  Analyze the rules in this adventure;

  */
void analyzeRules(void)
{
  List *rul;		/* Traversal pointer */

  for (rul = adv.ruls; rul != NULL; rul = rul->next)
    analyzeRule(rul->element.rul);
}




/*======================================================================

  generateRules()

  Generate all rules to be checked between actor moves.

  */
Aaddr generateRules(void)
{
  List *lst;
  Aaddr adr;

  for (lst = adv.ruls; lst != NULL; lst = lst->next) {
    showProgress();
    lst->element.rul->expadr = nextEmitAddress();
    generateExpression(lst->element.rul->exp);
    emit0(I_RETURN);
    lst->element.rul->stmadr = nextEmitAddress();
    generateStatements(lst->element.rul->stms);
    emit0(I_RETURN);
  }

  adr = nextEmitAddress();
  for (lst = adv.ruls; lst != NULL; lst = lst->next) {
    emit(FALSE);		/* Rule run */
    emit(lst->element.rul->expadr); /* Address to expression code */
    emit(lst->element.rul->stmadr); /* Address to statements code */
  }
  emit(EOF);
  return(adr);
}



/*----------------------------------------------------------------------*/
void dumpRule(RulNod *rul)
{
  put("RUL: "); dumpSrcp(&rul->srcp); indent();
  put("exp: "); dumpExpression(rul->exp); nl();
  put("expadr: "); dumpAddress(rul->expadr); nl();
  put("stms: "); dumpList(rul->stms, STATEMENT_LIST); nl();
  put("stmadr: "); dumpAddress(rul->stmadr); out();
}




/*----------------------------------------------------------------------*\

				RUL.C
			     Rules Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "exp.h"                /* EXP-nodes */
#include "stm.h"		/* STM-nodes */
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
	       ExpNod *exp,	/* IN - Expression to wait for */
	       List *stms)	/* IN - List of statements */
{
  RulNod *new;		/* The newly allocated node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(RulNod);

  new->srcp = *srcp;
  new->exp  = exp;
  new->stms = stms;

  rulcount++;
  return(new);
}



/*----------------------------------------------------------------------

  anrul()

  Analyze one rule

  */
static void anrul(RulNod *rul)
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  anexp(rul->exp, NULL, NULL);
  anstms(rul->stms, NULL, NULL, NULL);
}




/*======================================================================

  anruls()

  Analyze the rules in this adventure;

  */
void anruls(void)
{
  List *rul;		/* Traversal pointer */

  for (rul = adv.ruls; rul != NULL; rul = rul->next)
    anrul(rul->element.rul);
}




/*======================================================================

  geruls()

  Generate all rules to be checked between actor moves.

  */
Aaddr geruls(void)
{
  List *lst;
  Aaddr adr;

  for (lst = adv.ruls; lst != NULL; lst = lst->next) {
    if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }
    lst->element.rul->expadr = emadr();
    geexp(lst->element.rul->exp);
    emit0(C_STMOP, I_RETURN);
    lst->element.rul->stmadr = emadr();
    gestms(lst->element.rul->stms, NULL);
    emit0(C_STMOP, I_RETURN);
  }

  adr = emadr();
  for (lst = adv.ruls; lst != NULL; lst = lst->next) {
    emit(FALSE);		/* Rule run */
    emit(lst->element.rul->expadr); /* Address to expression code */
    emit(lst->element.rul->stmadr); /* Address to statements code */
  }
  emit(EOF);
  return(adr);
}



/*----------------------------------------------------------------------

  durul()

  Dump a Rule node

  */
void durul(RulNod *rul)
{
  put("RUL: "); dusrcp(&rul->srcp); in();
  put("exp: "); duexp(rul->exp); nl();
  put("expadr: "); duadr(rul->expadr); nl();
  put("stms: "); dulst(rul->stms, STMNOD); nl();
  put("stmadr: "); duadr(rul->stmadr); out();
}




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
#ifdef _PROTOTYPES_
RulNod *newrul(Srcp *srcp, ExpNod *exp, List *stms)
                	/* IN - Source Position */
                 	/* IN - Expression to wait for */
                	/* IN - List of statements */
#else
RulNod *newrul(srcp, exp, stms)
     Srcp *srcp;	/* IN - Source Position */
     ExpNod *exp;	/* IN - Expression to wait for */
     List *stms;	/* IN - List of statements */
#endif
{
  RulNod *new;		/* The newly allocated node */

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
#ifdef _PROTOTYPES_
static void anrul(RulNod *rul)
#else
static void anrul(rul)
     RulNod *rul;
#endif
{
  anexp(rul->exp, NULL, NULL);
  anstms(rul->stms, NULL, NULL, NULL);
}




/*======================================================================

  anruls()

  Analyze the rules in this adventure;

  */
#ifdef _PROTOTYPES_
void anruls(void)
#else
void anruls()
#endif
{
  List *rul;		/* Traversal pointer */

  for (rul = adv.ruls; rul != NULL; rul = rul->next)
    anrul(rul->element.rul);
}




/*======================================================================

  geruls()

  Generate all rules to be checked between actor moves.

  */
#ifdef _PROTOTYPES_
Aaddr geruls(void)
#else
Aaddr geruls()
#endif
{
  List *lst;
  Aaddr adr;

  for (lst = adv.ruls; lst != NULL; lst = lst->next) {
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
#ifdef _PROTOTYPES_
void durul(RulNod *rul)
#else
void durul(rul)
     RulNod *rul;
#endif
{
  put("RUL: "); dusrcp(&rul->srcp); in();
  put("exp: "); duexp(rul->exp); nl();
  put("expadr: "); duint(rul->expadr); nl();
  put("stms: "); dulst(rul->stms, STMNOD); nl();
  put("stmadr: "); duint(rul->stmadr); out();
}




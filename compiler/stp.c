/*----------------------------------------------------------------------*\

				STP.C
			      Step Nodes

\*----------------------------------------------------------------------*/

#include "stp_x.h"

/* USE: */
#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "exp_x.h"
#include "stm_x.h"

#include "lmList.h"
#include "emit.h"
#include "../interpreter/acode.h"
#include "dump.h"




/*======================================================================

  newStep()

  Allocates and initialises a new Step node.

  */
Step *newStep(Srcp *srcp,	/* IN - Source Position */
	      int after,	/* IN - Ticks to wait */
	      ExpNod *exp,	/* IN - Expression to wait for */
	      List *stms)	/* IN - List of statements */
{
  Step *new;		/* The newly allocated node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(Step);

  new->srcp = *srcp;
  new->after = after;
  new->exp  = exp;
  new->stms = stms;

  return(new);
}


/*======================================================================

  analyzeSteps()

  Analyse a list of Steps

  */
void analyzeSteps(List *stps, Context *context)
{
  List *lst;

  for (lst = stps; lst != NULL; lst = lst->next) {
    if (lst->element.stp->exp != NULL)
      anexp(lst->element.stp->exp, context);
    analyzeStatements(lst->element.stp->stms, context);
  }
}



/*======================================================================

  generateSteps()

  Generate code for all steps in a script for a particular Instance.

  */
Aaddr generateSteps(List *stps, int currentInstance)
{
  List *lst;
  Aaddr adr;

  for (lst = stps; lst != NULL; lst = lst->next) {
    if (lst->element.stp->exp != NULL) { /* Expression specified */
      lst->element.stp->expadr = emadr();
      geexp(lst->element.stp->exp, currentInstance);
      emit0(C_STMOP, I_RETURN);
    } else
      lst->element.stp->expadr = 0;
    lst->element.stp->stmadr = emadr();
    gestms(lst->element.stp->stms, currentInstance);
    emit0(C_STMOP, I_RETURN);
  }
  
  /* Now generate a step table */
  adr = emadr();
  for (lst = stps; lst != NULL; lst = lst->next) {
    emit(lst->element.stp->after);
    emit(lst->element.stp->expadr);
    emit(lst->element.stp->stmadr);
  }
  emit(EOF);
  return(adr);
}



/*======================================================================

  dumpStep()

  */
void dumpStep(Step *stp)
{
  put("STP: "); dumpSrcp(&stp->srcp); in();
  put("after: "); dumpInt(stp->after); nl();
  put("exp: "); dumpExpression(stp->exp); nl();
  put("expadr: "); dumpAddress(stp->expadr); nl();
  put("stms: "); dumpList(stp->stms, LIST_STM); nl();
  put("stmadr: "); dumpAddress(stp->stmadr); out();
}




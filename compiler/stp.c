/*----------------------------------------------------------------------*\

				STP.C
			      Step Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "exp_x.h"
#include "stm_x.h"

#include "lmList.h"

#include "stp.h"                /* STP-nodes */
#include "ins.h"                /* INS-nodes */

#include "emit.h"
#include "acode.h"
#include "dump.h"




/*======================================================================

  newstp()

  Allocates and initialises a new Step node.

  */
StpNod *newstp(Srcp *srcp,	/* IN - Source Position */
	       int after,	/* IN - Ticks to wait */
	       ExpNod *exp,	/* IN - Expression to wait for */
	       List *stms)	/* IN - List of statements */
{
  StpNod *new;		/* The newly allocated node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(StpNod);

  new->srcp = *srcp;
  new->after = after;
  new->exp  = exp;
  new->stms = stms;

  return(new);
}


/*======================================================================

  anstps()

  Analyse a list of Steps

  */
void anstps(List *stps,
	    Context *context)
{
  List *lst;

  for (lst = stps; lst != NULL; lst = lst->next) {
    if (lst->element.stp->exp != NULL)
      anexp(lst->element.stp->exp, context);
    anstms(lst->element.stp->stms, context);
  }
}



/*======================================================================

  gestps()

  Generate code for all steps in a script for a particular Instance.

  */
Aaddr gestps(List *stps,	/* IN - The steps to generate */
	     InsNod *ins)	/* IN - Inside any Instance? */
{
  List *lst;
  Aaddr adr;

  for (lst = stps; lst != NULL; lst = lst->next) {
    if (lst->element.stp->exp != NULL) { /* Expression specified */
      lst->element.stp->expadr = emadr();
      geexp(lst->element.stp->exp);
      emit0(C_STMOP, I_RETURN);
    } else
      lst->element.stp->expadr = 0;
    lst->element.stp->stmadr = emadr();
    gestms(lst->element.stp->stms, ins);
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

  dustp()

  Dump a Step node.

  */
void dustp(StpNod *stp)
{
  put("STP: "); dumpSrcp(&stp->srcp); in();
  put("after: "); dumpInt(stp->after); nl();
  put("exp: "); dumpExpression(stp->exp); nl();
  put("expadr: "); duadr(stp->expadr); nl();
  put("stms: "); dulst(stp->stms, LIST_STM); nl();
  put("stmadr: "); duadr(stp->stmadr); out();
}




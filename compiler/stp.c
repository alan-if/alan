/*----------------------------------------------------------------------*\

				STP.C
			      Step Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "lst.h"		/* LST-nodes */
#include "exp.h"                /* EXP-nodes */
#include "stm.h"		/* STM-nodes */
#include "stp.h"                /* STP-nodes */
#include "act.h"                /* ACT-nodes */

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
void anstps(List *stps,		/* IN - The steps to analyse */
	    ActNod *act)	/* IN - Possibly inside Actor? */
{
  List *lst;

  for (lst = stps; lst != NULL; lst = lst->next) {
    if (lst->element.stp->exp != NULL)
      anexp(lst->element.stp->exp, NULL, NULL);
    anstms(lst->element.stp->stms, act, NULL, NULL);
  }
}



/*======================================================================

  gestps()

  Generate code for all steps in a script for a particular actor.

  */
Aaddr gestps(List *stps,	/* IN - The steps to generate */
	     ActNod *act)	/* IN - Inside any actor */
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
    gestms(lst->element.stp->stms, act);
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
  put("STP: "); dusrcp(&stp->srcp); in();
  put("after: "); duint(stp->after); nl();
  put("exp: "); duexp(stp->exp); nl();
  put("expadr: "); duadr(stp->expadr); nl();
  put("stms: "); dulst(stp->stms, STMNOD); nl();
  put("stmadr: "); duadr(stp->stmadr); out();
}




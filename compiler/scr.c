/*----------------------------------------------------------------------*\

				SCR.C
			     Script Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"                /* STM-nodes */
#include "stp.h"                /* STP-nodes */
#include "scr.h"                /* SCR-nodes */
#include "act.h"                /* ACT-nodes */

#include "emit.h"
#include "dump.h"




/*======================================================================

  newscr()

  Allocates and initialises a new script node.

  */
#ifdef _PROTOTYPES_
ScrNod *newscr(
     Srcp *srcp,		/* IN - Source Position */
     int code,			/* IN - Code for the script */
     List *descr,		/* IN - Optional description */
     List *stps                	/* IN - List of steps */
)
#else
ScrNod *newscr(srcp, code, descr, stps)
     Srcp *srcp;		/* IN - Source Position */
     int code;			/* IN - Code for the script */
     List *descr;		/* IN - Optional description */
     List *stps;		/* IN - List of steps */
#endif
{
  ScrNod *new;		/* The newly allocated node */

  new = NEW(ScrNod);

  new->srcp = *srcp;
  new->code = code;
  new->descr = descr;
  new->stps = stps;

  return(new);
}



/*======================================================================

  anscrs()

  Analyze the scripts for one actor.

  */
#ifdef _PROTOTYPES_
void anscrs(List *scrs, ActNod *act)
#else
void anscrs(scrs, act)
     List *scrs;
     ActNod *act;
#endif
{
  List *lst;
  List *scrlst;

  if (scrs != NULL && act->nam->code == 1)	/* Ignored for HERO */
      lmLog(&lst->element.scr->srcp, 411, sevWAR, "Script");
  for (lst = scrs; lst != NULL; lst = lst->next) {
    anstms(lst->element.scr->descr, act, NULL, NULL);
    for (scrlst = lst->next; scrlst != NULL; scrlst = scrlst->next) {
      if (lst->element.scr->code == scrlst->element.scr->code)
        lmLog(&scrlst->element.scr->srcp, 403, sevERR, act->nam->str);
    }
    anstps(lst->element.scr->stps, act);
  }
}





/*======================================================================

  gescrs()

  Generate script routines for an actor.

  */
#ifdef _PROTOTYPES_
Aword gescrs(ActNod *act)
#else
Aword gescrs(act)
  ActNod *act;
#endif
{
  List *lst;
  Aword scradr;

  if (act->scrs == NULL)
    return(0);

  for (lst = act->scrs; lst != NULL; lst = lst->next) {
    lst->element.scr->stpadr = gestps(lst->element.scr->stps, act);
    if (lst->element.scr->descr != NULL) {
      lst->element.scr->stmadr = emadr();
      gestms(lst->element.scr->descr, act);
      emit0(C_STMOP, I_RETURN);
    } else
      lst->element.scr->stmadr = 0;
  }

  /* Script table */
  scradr = emadr();
  for (lst = act->scrs; lst != NULL; lst = lst->next) {
    emit(lst->element.scr->code);
    emit(lst->element.scr->stmadr);
    emit(lst->element.scr->stpadr);
  }
  emit(EOF);
  return(scradr);
}



/*======================================================================

  duscr()

  Dump a Script node

  */
#ifdef _PROTOTYPES_
void duscr(ScrNod *scr)
#else
void duscr(scr)
     ScrNod *scr;
#endif
{
  put("SCR: "); dusrcp(&scr->srcp); in();
  put("code: "); duint(scr->code); nl();
  put("stps: "); dulst(scr->stps, STPNOD); nl();
  put("stpadr: "); duint(scr->stpadr); out();
}




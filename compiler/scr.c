/*----------------------------------------------------------------------*\

                                SCR.C
                             Script Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "lst.h"                /* LST-nodes */
#include "nam.h"                /* NAM-nodes */
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
     Srcp *srcp,                /* IN - Source Position */
     NamNod *nam,               /* IN - Name for the script */
     int code,                  /* IN - Code for the script */
     List *descr,               /* IN - Optional description */
     List *stps                 /* IN - List of steps */
)
#else
ScrNod *newscr(srcp, code, nam, descr, stps)
     Srcp *srcp;                /* IN - Source Position */
     NamNod *nam;               /* IN - Name for the script */
     int code;                  /* IN - Number of the script */
     List *descr;               /* IN - Optional description */
     List *stps;                /* IN - List of steps */
#endif
{
  ScrNod *new;          /* The newly allocated node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ScrNod);

  new->srcp = *srcp;
  new->code = code;
  new->nam = nam;
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
  int highest;                  /* Highest script code found so far */


  if (scrs == NULL) return;

  /* Error if defined for HERO */
  if (scrs != NULL && act->nam->code == 1)
      lmLog(&lst->element.scr->srcp, 411, sevWAR, "Script");

  /* First inspect the codes and save the highest */
  highest = scrs->element.scr->code;
  for (lst = scrs; lst != NULL; lst = lst->next)
    if (lst->element.scr->code > highest)
      highest = lst->element.scr->code;

  /* Look for redefinition of script names and numbers */
  for (lst = scrs; lst != NULL; lst = lst->next) {

    /* Analyze the statements */
    anstms(lst->element.scr->descr, act, NULL, NULL);

    /* Any multiple of this name or number ? */
    for (scrlst = lst->next; scrlst != NULL; scrlst = scrlst->next) {
      if (lst->element.scr->nam != NULL) {
        /* It was given a name, then try compare to the name, if any */
        if (scrlst->element.scr->nam != NULL &&
            eqnams(lst->element.scr->nam, scrlst->element.scr->nam))
          lmLog(&scrlst->element.scr->srcp, 403, sevERR, act->nam->str);
      } else /* No name, just the code */
        if (lst->element.scr->code == scrlst->element.scr->code)
          lmLog(&scrlst->element.scr->srcp, 403, sevERR, act->nam->str);
    }

    /* If only given a name, use the highest code + 1 as its code */
    if (lst->element.scr->nam != NULL)
      lst->element.scr->code = ++highest;

    /* Finally, analyse the steps inside the script */
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
  put("nam: "); dunam(scr->nam); nl();
  put("stps: "); dulst(scr->stps, STPNOD); nl();
  put("stpadr: "); duint(scr->stpadr); out();
}




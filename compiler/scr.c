/*----------------------------------------------------------------------*\

                                SCR.C
                             Script Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"

#include "lmList.h"
#include "acode.h"

#include "stm.h"                /* STM-nodes */
#include "stp.h"                /* STP-nodes */
#include "scr.h"                /* SCR-nodes */

#include "emit.h"
#include "dump.h"




/*======================================================================

  newscr()

  Allocates and initialises a new script node.

  */
ScrNod *newscr(
     Srcp *srcp,                /* IN - Source Position */
     IdNode *id,		/* IN - Name for the script */
     int code,                  /* IN - Code for the script */
     List *descr,               /* IN - Optional description */
     List *stps                 /* IN - List of steps */
)
{
  ScrNod *new;          /* The newly allocated node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ScrNod);

  new->srcp = *srcp;
  new->code = code;
  new->id = id;
  new->descr = descr;
  new->stps = stps;

  return(new);
}



/*======================================================================

  prepscrs()

  Prepare scripts for this actor (i.e. number them)

  */
void prepscrs(List *scrs, InsNod *ins)
{
  List *lst;
  List *scrlst;
  int highest;                  /* Highest script code found so far */

  if (scrs == NULL) return;

  /* First inspect the codes and save the highest */
  highest = scrs->element.scr->code;
  for (lst = scrs; lst != NULL; lst = lst->next)
    if (lst->element.scr->code > highest)
      highest = lst->element.scr->code;

  /* Look for redefinition of script names and numbers and give numbers to named scripts */
  for (lst = scrs; lst != NULL; lst = lst->next) {

    /* Any multiple of this name or number ? */
    for (scrlst = lst->next; scrlst != NULL; scrlst = scrlst->next) {
      if (lst->element.scr->id != NULL) {
        /* It was given a name, then try compare to the name, if any */
        if (scrlst->element.scr->id != NULL &&
            equalId(lst->element.scr->id, scrlst->element.scr->id))
          lmLog(&scrlst->element.scr->srcp, 403, sevERR, ins->slots->id->string);
      } else /* No name, just the code */
        if (lst->element.scr->code == scrlst->element.scr->code)
          lmLog(&scrlst->element.scr->srcp, 403, sevERR, ins->slots->id->string);
    }

    /* If only given a name, use the highest code + 1 as its code */
    if (lst->element.scr->id != NULL)
      lst->element.scr->code = ++highest;
  }

}



/*======================================================================

  anscrs()

  Analyze the scripts for one actor.

  */
void anscrs(List *scrs, InsNod *ins)
{
  List *lst;

  if (scrs == NULL) return;

  /* Error if defined for HERO */
  if (scrs != NULL && ins->slots->id->symbol->code == 1)
      lmLog(&lst->element.scr->srcp, 411, sevWAR, "Script");

  for (lst = scrs; lst != NULL; lst = lst->next) {
    /* Analyze the statements */
    anstms(lst->element.scr->descr, ins, NULL, NULL);

    /* Finally, analyse the steps inside the script */
    anstps(lst->element.scr->stps, ins);
  }
}





/*======================================================================

  gescrs()

  Generate script routines for an instance.

  */
Aword gescrs(InsNod *ins)
{
  List *lst;
  Aword scradr;

  if (ins == NULL) syserr("NULL in gescrs()");
  if (ins->slots == NULL || ins->slots->scrs == NULL)
    return(0);

  for (lst = ins->slots->scrs; lst != NULL; lst = lst->next) {
    lst->element.scr->stpadr = gestps(lst->element.scr->stps, ins);
    if (lst->element.scr->descr != NULL) {
      lst->element.scr->stmadr = emadr();
      gestms(lst->element.scr->descr, ins);
      emit0(C_STMOP, I_RETURN);
    } else
      lst->element.scr->stmadr = 0;
  }

  /* Script table */
  scradr = emadr();
  for (lst = ins->slots->scrs; lst != NULL; lst = lst->next) {
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
void duscr(ScrNod *scr)
{
  put("SCR: "); dumpSrcp(&scr->srcp); in();
  put("code: "); dumpInt(scr->code); nl();
  put("id: "); dumpId(scr->id); nl();
  put("stps: "); dulst(scr->stps, LIST_STP); nl();
  put("stpadr: "); duadr(scr->stpadr); out();
}




/*----------------------------------------------------------------------*\

				WHR.C
			     Where Nodes

\*----------------------------------------------------------------------*/

#include "whr_x.h"

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "wht_x.h"
#include "sym_x.h"
#include "id_x.h"

#include "lmList.h"

#include "acode.h"

#include "cnt.h"		/* CNT-nodes */
#include "elm.h"		/* ELM-nodes */

#include "dump.h"
#include "emit.h"



/*======================================================================

  newwhr()

  Create a new Where node.

  */
WhrNod *newwhr(Srcp *srcp,	/* IN - Source position */
	       WhrKind kind,	/* IN - Where kind */
	       WhtNod *wht)	/* IN - What */
{
  WhrNod *new;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(WhrNod);

  new->srcp = *srcp;
  new->kind = kind;
  new->wht = wht;

  return(new);
}


/*======================================================================

  symbolizeWhr()

  Symbolize a where reference.

  */
void symbolizeWhr(WhrNod *whr)
{
  if (whr == NULL) return;

  switch (whr->kind) {
  case WHR_NEAR:
  case WHR_AT:
  case WHR_IN:
    symbolizeWht(whr->wht);
    break;
  default:
    break;
  }
}


/*======================================================================

  anwhr()

  Analyse a where reference.

  */
void anwhr(WhrNod *whr,		/* IN - Where node */
	   EvtNod *evt,		/* IN - Inside Event? */
	   List *pars)		/* IN - Possible parameters */
{
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (whr->wht->kind) {
    case WHT_ID:
#ifndef FIXME
      syserr("UNIMPL: namcheck() -> idcheck()");
#else
      namcheck(&sym, &elm, whr->wht->id, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
#endif
      break;
    case WHT_LOC:
      whr->kind = WHR_HERE;
      break;
    case WHT_OBJ:
      if (pars == NULL)
	lmLog(&whr->wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
      if (evt != NULL)
	lmLog(&whr->wht->srcp, 412, sevERR, "");
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
    break;
  case WHR_IN:
#ifndef FIXME
    syserr("UNIMPL: cntcheck");
#else
    cntcheck(whr->wht, pars);
#endif
    break;
  default:
    syserr("Unrecognized switch in anwhr()");
    break;
  }
}


/*======================================================================

  gewhr()

  Generate a location reference according to the WHR.

  */
void gewhr(WhrNod *whr)		/* IN - Where node */
{
  switch (whr->kind) {

  case WHR_AT:
    switch (whr->wht->kind) {
    case WHT_ID:
      gewht(whr->wht);
#ifdef FIXME
      /* Do this instance have location properties?
	 Or should we locate at the same place? */
      if (whr->wht->id->kind != NAMLOC)
	emit0(C_STMOP, I_WHERE);
#endif
      break;
    case WHT_OBJ:
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_WHERE);
      break;
    case WHT_LOC:
      emit0(C_CURVAR, V_CURLOC);
      break;
    case WHT_ACT:
      emit0(C_CURVAR, V_CURACT);
      emit0(C_STMOP, I_WHERE);
      break;
    }
    break;

  case WHR_IN:
    geid(whr->wht->id);
    break;

  case WHR_HERE:
    emit0(C_CURVAR, V_CURLOC);
    break;

  default:
    syserr("unrecognised switch in gewhr()");
    break;
  }
}



/*----------------------------------------------------------------------

  duwhr()

  Dump a Where node

  */
void duwhr(WhrNod *whr)
{
  if (whr == NULL) {
    put("NULL");
    return;
  }

  put("WHR: "); dumpSrcp(&whr->srcp); in();
  put("whr: ");
  switch (whr->kind) {
  case WHR_DEFAULT: put("DEFAULT"); break;
  case WHR_HERE: put("HERE"); break;
  case WHR_AT: put("AT"); break;
  case WHR_IN: put("IN"); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("wht: "); duwht(whr->wht); out();
}

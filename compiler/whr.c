/*----------------------------------------------------------------------*\

				WHR.C
			     Where Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "nam.h"		/* NAM-nodes */
#include "sym.h"		/* SYM-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "elm.h"		/* ELM-nodes */
#include "whr.h"		/* WHR-nodes */

#include "dump.h"
#include "emit.h"



/*======================================================================

  newwhr()

  Create a new Where node.

  */
WhrNod *newwhr(Srcp *srcp,	/* IN - Source position */
	       WhrKind whr,	/* IN - Where kind */
	       WhtNod *wht)	/* IN - What */
{
  WhrNod *new;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(WhrNod);

  new->srcp = *srcp;
  new->whr = whr;
  new->wht = wht;

  return(new);
}


/*======================================================================

  anwhr()

  Analyse a where reference.

  */
void anwhr(WhrNod *whr,		/* IN - Where node */
	   EvtNod *evt,		/* IN - Inside Event? */
	   List *pars)		/* IN - Possible parameters */
{
  SymNod *sym;
  ElmNod *elm;

  switch (whr->whr) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (whr->wht->wht) {
    case WHT_ID:
#ifndef FIXME
      syserr("UNIMPL: namcheck() -> idcheck()");
#else
      namcheck(&sym, &elm, whr->wht->id, NAMLOC+NAMOBJ+NAMACT+NAMCOBJ+NAMCACT, NAMANY, pars);
#endif
      break;
    case WHT_LOC:
      whr->whr = WHR_HERE;
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
    cntcheck(whr->wht, pars);
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
  switch (whr->whr) {

  case WHR_AT:
    switch (whr->wht->wht) {
    case WHT_ID:
      gewht(whr->wht);
      if (whr->wht->id->kind != NAMLOC)
	emit0(C_STMOP, I_WHERE);
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

  put("WHR: "); dusrcp(&whr->srcp); in();
  put("whr: ");
  switch (whr->whr) {
  case WHR_DEFAULT: put("DEFAULT"); break;
  case WHR_HERE: put("HERE"); break;
  case WHR_AT: put("AT"); break;
  case WHR_IN: put("IN"); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("wht: "); duwht(whr->wht); out();
}

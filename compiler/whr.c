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
#include "cnt_x.h"

#include "lmList.h"
#include "acode.h"
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

  verifyAtLocation()

*/
void verifyAtLocation(WhrNod *whr)
{
  switch (whr->kind) {
  case WHR_AT:
    if (whr->wht->kind == WHT_ID) {
      inheritCheck(whr->wht->id, "an instance", "location");
    } else
      lmLogv(&whr->srcp, 351, sevERR, "an instance", "location", NULL);
    break;
  default:
      lmLogv(&whr->srcp, 351, sevERR, "an instance", "location", NULL);
    break;
  }
}


/*======================================================================

  analyzeWhere()

  Analyse a where reference.

  */
void analyzeWhere(WhrNod *whr,
		  Context *context)
{
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (whr->wht->kind) {
    case WHT_ID:
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
    break;
  case WHR_IN:
    verifyContainer(whr->wht, context);
    break;
  default:
    syserr("Unrecognized switch in analyzeWhere()");
    break;
  }
}





/*======================================================================

  anwhr()

  Analyse a where reference.

  */
void anwhr(WhrNod *whr,
	   Context *context)
{
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (whr->wht->kind) {
    case WHT_ID:
      (void) symcheck(whr->wht->id, INSTANCE_SYMBOL, context);
      break;
    case WHT_LOC:
      whr->kind = WHR_HERE;
      break;
    case WHT_OBJ:
      if (context->kind != VERB_CONTEXT)
	lmLog(&whr->wht->srcp, 409, sevERR, "");
      else if ( context->verb->fields.verb.parameterSymbols == NULL)
	lmLog(&whr->wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&whr->wht->srcp, 412, sevERR, "");
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
    break;
  case WHR_IN:
    verifyContainer(whr->wht, context);
    break;
  default:
    syserr("Unrecognized switch in anwhr()");
    break;
  }
}


/*======================================================================

  generateInitialLocation()

  Generate a location reference according to the WHR for initial locations.
  This means that it can only be an identifier. Can only be AT location or
  IN container.

  */
Aword generateInitialLocation(WhrNod *whr) /* IN - Where node */
{
  if (whr != NULL)
    switch (whr->kind) {
    case WHR_IN:
    case WHR_AT:
      return whr->wht->id->symbol->code;
    default: syserr("Unexpected where kind in generateInitialLocation()");
    }

  return 0;
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
    generateId(whr->wht->id);
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

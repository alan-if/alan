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
#include "../interpreter/acode.h"
#include "dump.h"
#include "emit.h"



/*======================================================================

  newwhr()

  Create a new Where node.

  */
Where *newwhr(Srcp *srcp,	/* IN - Source position */
	       WhrKind kind,	/* IN - Where kind */
	       What *wht)	/* IN - What */
{
  Where *new;

  showProgress();

  new = NEW(Where);

  new->srcp = *srcp;
  new->kind = kind;
  new->what = wht;

  return(new);
}


/*======================================================================  */
void symbolizeWhere(Where *whr)
{
  if (whr == NULL) return;

  switch (whr->kind) {
  case WHR_NEAR:
  case WHERE_AT:
  case WHR_IN:
    symbolizeWhat(whr->what);
    break;
  default:
    break;
  }
}



/*======================================================================

  verifyInitialLocation()

*/
void verifyInitialLocation(Where *whr)
{
  switch (whr->kind) {
  case WHERE_AT:
    if (whr->what->kind == WHAT_ID) {
      inheritCheck(whr->what->id, "an instance", "location");
    } else
      lmLogv(&whr->srcp, 351, sevERR, "an instance", "location", NULL);
    break;
  case WHR_IN:
    verifyContainer(whr->what, NULL);
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
void analyzeWhere(Where *whr,
		  Context *context)
{
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHERE_AT:
    switch (whr->what->kind) {
    case WHAT_ID:
      (void) symcheck(whr->what->id, INSTANCE_SYMBOL, context);
      break;
    case WHAT_LOCATION:
      break;
    default:
      syserr("Unrecognized switch in '%s()'", __FUNCTION__);
      break;
    }
    break;
  case WHR_IN:
    verifyContainer(whr->what, context);
    break;
  default:
    syserr("Unrecognized switch in '%s()'", __FUNCTION__);
    break;
  }
}





/*======================================================================

  anwhr()

  Analyse a where reference.

  */
void anwhr(Where *whr,
	   Context *context)
{
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHERE_AT:
    switch (whr->what->kind) {
    case WHAT_ID:
      (void) symcheck(whr->what->id, INSTANCE_SYMBOL, context);
      break;
    case WHAT_LOCATION:
      whr->kind = WHR_HERE;
      break;
    case WHAT_ACTOR:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&whr->what->srcp, 412, sevERR, "");
      break;
    default:
      syserr("Unrecognized switch in '%s()'", __FUNCTION__);
      break;
    }
    break;
  case WHR_IN:
    verifyContainer(whr->what, context);
    break;
  default:
    syserr("Unrecognized switch in '%s()'", __FUNCTION__);
    break;
  }
}


/*======================================================================

  generateInitialLocation()

  Generate a location reference according to the WHR for initial locations.
  This means that it can only be an identifier. Can only be AT location or
  IN container.

  */
Aword generateInitialLocation(Where *whr) /* IN - Where node */
{
  if (whr != NULL)
    switch (whr->kind) {
    case WHR_IN:
    case WHERE_AT:
      return whr->what->id->symbol->code;
    default: syserr("Unexpected where kind in '%s()'", __FUNCTION__);
    }

  return 0;
}



/*======================================================================

  gewhr()

  Generate a location reference according to the WHR.

  */
void generateWhere(Where *where)
{
  switch (where->kind) {

  case WHERE_AT:
    switch (where->what->kind) {
    case WHAT_ID:
      generateWhat(where->what);
      /* Instance inherit from location? Or is it at the location of it? */
      if (!inheritsFrom(where->what->id->symbol, locationSymbol))
	emit0(I_WHERE);
      break;
    case WHAT_LOCATION:
      emitVariable(V_CURLOC);
      break;
    case WHAT_ACTOR:
      emitVariable(V_CURACT);
      emit0(I_WHERE);
      break;
    default:
      syserr("Unexpected switch on whatKind in '%s()'", __FUNCTION__);
    }
    break;

  case WHR_IN:
    generateId(where->what->id);
    break;

  case WHR_HERE:
    emitVariable(V_CURLOC);
    break;

  default:
    syserr("unrecognised switch in '%s()'", __FUNCTION__);
    break;
  }
}



/*----------------------------------------------------------------------

  duwhr()

  Dump a Where node

  */
void duwhr(Where *whr)
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
  case WHERE_AT: put("AT"); break;
  case WHR_IN: put("IN"); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("wht: "); dumpWhat(whr->what); out();
}

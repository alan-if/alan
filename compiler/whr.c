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
#include "exp_x.h"

#include "lmList.h"
#include "../interpreter/acode.h"
#include "dump.h"
#include "emit.h"



/*======================================================================*/
Where *newWhere(Srcp *srcp, WhrKind kind, Expression *wht) {
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
    symbolizeExpression(whr->what);
    break;
  default:
    break;
  }
}


/*======================================================================*/
void verifyInitialLocation(Where *whr)
{
  switch (whr->kind) {
  case WHERE_AT:
    if (whr->what->fields.wht.wht->kind == WHAT_ID) {
      inheritCheck(whr->what->fields.wht.wht->id, "Initial location using AT", "an instance", "location");
    } else
      lmLog(&whr->srcp, 355, sevERR, "");
    break;
  case WHR_IN:
    verifyContainer(whr->what->fields.wht.wht, NULL, "Expression after IN");
    break;
  default:
    lmLogv(&whr->srcp, 355, sevERR, "");
    break;
  }
}


/*======================================================================*/
void analyzeWhere(Where *whr, Context *context) {
  switch (whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHERE_AT:
    analyzeExpression(whr->what, context);
    if (whr->what->type != ERROR_TYPE && whr->what->type != INSTANCE_TYPE)
      lmLogv(&whr->what->srcp, 428, sevERR, "Expression after AT", "an instance", NULL);
    break;
  case WHR_IN:
    verifyContainerExpression(whr->what, context, "Expression after IN");
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
      return whr->what->fields.wht.wht->id->symbol->code;
    default: syserr("Unexpected where kind in '%s()'", __FUNCTION__);
    }

  return 0;
}



/*======================================================================*/
void generateWhere(Where *where)
{
  switch (where->kind) {

  case WHERE_AT:
    generateExpression(where->what);
    if (!inheritsFrom(where->what->class, locationSymbol))
      emit0(I_WHERE);
    break;

  case WHR_IN:
    generateExpression(where->what);
    break;

  case WHR_HERE:
    emitVariable(V_CURLOC);
    break;

  default:
    syserr("Unrecognised switch in '%s()'", __FUNCTION__);
    break;
  }
}



/*======================================================================*/
void dumpWhere(Where *whr)
{
  if (whr == NULL) {
    put("NULL");
    return;
  }

  put("WHR: "); dumpSrcp(&whr->srcp); indent();
  put("whr: ");
  switch (whr->kind) {
  case WHR_DEFAULT: put("DEFAULT"); break;
  case WHR_HERE: put("HERE"); break;
  case WHERE_AT: put("AT"); break;
  case WHR_IN: put("IN"); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("wht: "); dumpExpression(whr->what); out();
}

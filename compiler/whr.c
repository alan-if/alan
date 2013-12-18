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
#include "dump_x.h"

#include "lmList.h"
#include "acode.h"
#include "emit.h"



/*======================================================================*/
Where *newWhere(Srcp *srcp, Bool directly, WhereKind kind, Expression *what) {
    Where *new;

    progressCounter();

    new = NEW(Where);

    new->srcp = *srcp;
    new->directly = directly;
    new->kind = kind;
    new->what = what;

    return(new);
}


/*======================================================================*/
void symbolizeWhere(Where *whr)
{
    if (whr == NULL) return;

    switch (whr->kind) {
    case WHERE_NEARBY:
    case WHERE_AT:
    case WHERE_IN:
        symbolizeExpression(whr->what);
        break;
    default:
        break;
    }
}


/*======================================================================*/
Bool verifyInitialLocation(Where *whr)
{
    if (whr->directly)
        lmLog(&whr->srcp, 422, sevERR, "Initial location");

    if (whr->what == NULL || whr->what->kind != WHAT_EXPRESSION)
        lmLogv(&whr->srcp, 355, sevERR, "", NULL);
    else
        switch (whr->kind) {
        case WHERE_AT:
            if (whr->what->fields.wht.wht->kind == WHAT_ID) {
                instanceCheck(whr->what->fields.wht.wht->id, "Initial location using AT", "location");
            } else {
                lmLog(&whr->srcp, 355, sevERR, "");
                return FALSE;
            }
            break;
        case WHERE_IN:
            verifyContainerForInitialLocation(whr->what->fields.wht.wht, NULL, "Expression after IN");
            break;
        default:
            lmLogv(&whr->srcp, 355, sevERR, "", NULL);
            return FALSE;
            break;
        }
    return TRUE;
}


/*======================================================================*/
void analyzeWhere(Where *whr, Context *context) {
    switch (whr->kind) {
    case WHERE_DEFAULT:
        break;
    case WHERE_HERE:
    case WHERE_NEARBY:
        if (context->kind == RULE_CONTEXT)
            lmLogv(&whr->srcp, 443, sevERR, "Rule context", "Here or Nearby", NULL);
        break;
    case WHERE_AT:
    case WHERE_NEAR:
        analyzeExpression(whr->what, context);
        if (whr->what->type != ERROR_TYPE && whr->what->type != INSTANCE_TYPE && whr->what->type != REFERENCE_TYPE)
            lmLogv(&whr->what->srcp, 428, sevERR, "Expression after AT", "an instance", NULL);
        break;
    case WHERE_IN:
        analyzeExpression(whr->what, context);
        if (whr->what->type != ERROR_TYPE) {
            if (whr->what->type == SET_TYPE)
                whr->kind = WHERE_INSET;
            else
                verifyContainerExpression(whr->what, context, "Expression after IN");
        }
        break;
    case WHERE_INSET:
        SYSERR("Unrecognized switch");
        break;
    }
}


/*======================================================================

  generateInitialLocation()

  Generate a location reference according to the WHR for initial locations.
  This means that it can only be an identifier. Can only be AT location or
  IN container.

  */
Aword generateInitialLocation(Properties *props)
{
    if (props->whr != NULL)
        switch (props->whr->kind) {
        case WHERE_IN:
        case WHERE_AT:
            return props->whr->what->fields.wht.wht->id->symbol->code;
        default: SYSERR("Unexpected Where kind as initial location");
        }

    if (inheritsFrom(props->id->symbol, locationSymbol))
        return 0;
    else
        return 1; /* Anything not a location should be at #nowhere if undefined */
}



/*======================================================================*/
void generateWhere(Where *where)
{
    switch (where->kind) {

    case WHERE_AT:
        generateExpression(where->what);
        if (!inheritsFrom(where->what->class, locationSymbol)) {
            emitConstant(where->directly);
            emit0(I_WHERE);
        }
        break;

    case WHERE_IN:
    case WHERE_INSET:
        generateExpression(where->what);
        break;

    case WHERE_HERE:
        emitVariable(V_CURLOC);
        break;

    default:
        SYSERR("Unrecognised switch");
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

    put("WHR: "); dumpSrcp(whr->srcp); indent();
    put("whr: "); if (whr->directly) put("DIRECTLY ");
    switch (whr->kind) {
    case WHERE_DEFAULT: put("DEFAULT"); break;
    case WHERE_HERE: put("HERE"); break;
    case WHERE_NEARBY: put("NEARBY"); break;
    case WHERE_NEAR: put("NEAR"); break;
    case WHERE_AT: put("AT"); break;
    case WHERE_IN: put("IN"); break;
    case WHERE_INSET: put("INSET"); break;
    }
    nl();
    switch (whr->kind) {
    case WHERE_HERE:
    case WHERE_NEARBY:
        break;
    default:
        put("wht: "); dumpExpression(whr->what);
        break;
    }
    out();
}

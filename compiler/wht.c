/*----------------------------------------------------------------------*\

				WHT.C
			      What Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "wht_x.h"
#include "id_x.h"
#include "sym_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "lmList.h"
#include "emit.h"


/*======================================================================*/
What *newWhat(Srcp *srcp,	/* IN - Source position */
	      WhatKind kind,	/* IN - What kind */
	      IdNode *id)	/* IN - ID or NULL */
{
  What *new;

  showProgress();

  new = NEW(What);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;

  return(new);
}

/*======================================================================*/
void symbolizeWhat(What *wht)
{
  switch (wht->kind) {
  case WHAT_ID:
    symbolizeId(wht->id);
    break;
  default:
    break;
  }
}


/*======================================================================*/
Symbol *symbolOfWhat(What *what, Context *context) {
  switch (what->kind) {
  case WHAT_THIS:
    return symbolOfContext(context);
  case WHAT_LOCATION:
    return locationSymbol;
  case WHAT_ACTOR:
    return actorSymbol;
  case WHAT_ID:
    return what->id->symbol;
  default:
    syserr("Unexpected What kind in '%s()'", __FUNCTION__);
  }
  return NULL;
}



/*======================================================================*/
void whatIsNotContainer(What *wht, Context *context, char construct[])
{
  Symbol *sym;

  if (wht == NULL)
    return;

  switch (wht->kind) {
  case WHAT_THIS:
    lmLog(&wht->srcp, 309, sevERR, "");
    break;
  case WHAT_ID:
    sym = wht->id->symbol;
    if (sym)
      switch (sym->kind) {
      case INSTANCE_SYMBOL:
	lmLogv(&wht->srcp, 318, sevERR, wht->id->string, construct, NULL);
	break;
      case PARAMETER_SYMBOL:
	lmLogv(&wht->srcp, 312, sevERR, "Parameter", wht->id->string, "a Container", "because it is not restricted to Container in the Syntax", NULL);
	break;
      default:
	syserr("Unexpected symbol kind in '%s()'", __FUNCTION__);
      }
    break;

  case WHAT_LOCATION:
    break;

  case WHAT_ACTOR:
    lmLogv(&wht->srcp, 428, sevERR, construct, "a Container, which the Current Actor is not since the class 'actor' does not have the Container property", NULL);
    break;

  default:
    syserr("Unrecognized switch in '%s()'", __FUNCTION__);
    break;
  }
}


/*======================================================================*/
Bool verifyWhatContext(What *what, Context *context) {
  switch (what->kind) {

  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT) {
      lmLog(&what->srcp, 412, sevERR, "");
      return FALSE;
    }
    break;

  case WHAT_LOCATION:
  case WHAT_ID:
    break;

  case WHAT_THIS:
    if (!inEntityContext(context)) {
      lmLog(&what->srcp, 421, sevERR, "");
      return FALSE;
    }
    break;

  default:
    syserr("Unexpected What kind in '%s()'.", __FUNCTION__);
    break;
  }
  return TRUE;
}


/*======================================================================*/
void generateWhat(What *wht)
{
  switch (wht->kind) {
  case WHAT_LOCATION:
    emitVariable(V_CURLOC);
    break;
  case WHAT_ACTOR:
    emitVariable(V_CURACT);
    break;
  case WHAT_ID:
    generateId(wht->id);
    break;
  case WHAT_THIS:
    emitVariable(V_CURRENT_INSTANCE);
    break;
  default:
    syserr("Unexpected case in '%s()'", __FUNCTION__);
  }
}


/*======================================================================

  dumpWhat()

  Dump a What node.

  */
void dumpWhat(What *wht)
{
  if (wht == NULL) {
    put("NULL");
    return;
  }

  put("WHAT: "); dumpSrcp(&wht->srcp); indent();
  put("kind: ");
  switch (wht->kind) {
  case WHAT_LOCATION: put("LOCATION"); break;
  case WHAT_ACTOR: put("ACTOR"); break;
  case WHAT_ID: put("ID "); break;
  case WHAT_THIS: put("THIS"); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("id: "); dumpId(wht->id); out();
}



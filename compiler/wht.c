/*----------------------------------------------------------------------*\

				WHT.C
			      What Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "wht_x.h"
#include "id_x.h"

#include "lmList.h"
#include "emit.h"
#include "dump.h"


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

/*======================================================================

  symbolizeWhat()

  Symbolize a What reference.

  */
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



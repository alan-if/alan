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


/*======================================================================

  newwht()

  Create a new What node.

  */
WhtNod *newwht(Srcp *srcp,	/* IN - Source position */
	       WhtKind kind,	/* IN - What kind */
	       IdNode *id)	/* IN - ID or NULL */
{
  WhtNod *new;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(WhtNod);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;

  return(new);
}

/*======================================================================

  symbolizeWht()

  Symbolize a What reference.

  */
void symbolizeWht(WhtNod *wht)
{
  switch (wht->kind) {
  case WHT_ID:
    symbolizeId(wht->id);
    break;
  default:
    break;
  }
}


/*======================================================================

  gewht()

  Generate code for a reference to What.

  */
void gewht(WhtNod *wht)		/* IN - What to generate */
{
  switch (wht->kind) {
  case WHT_OBJ:
    emit0(C_CONST, 1);
    emit0(C_CURVAR, V_PARAM);
    break;
  case WHT_LOC:
    emit0(C_CURVAR, V_CURLOC);
    break;
  case WHT_ACT:
    emit0(C_CURVAR, V_CURACT);
    break;
  case WHT_ID:
    generateId(wht->id);
    break;
  }
}


/*======================================================================

  duwht()

  Dump a WHT node.

  */
void duwht(WhtNod *wht)
{
  if (wht == NULL) {
    put("NULL");
    return;
  }

  put("WHT: "); dumpSrcp(&wht->srcp); in();
  put("wht: ");
  switch (wht->kind) {
  case WHT_OBJ: put("OBJECT"); break;
  case WHT_LOC: put("LOCATION"); break;
  case WHT_ACT: put("ACTOR"); break;
  case WHT_ID: put("ID "); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("id: "); dumpId(wht->id); out();
}



/*----------------------------------------------------------------------*\

				WHT.C
			      What Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "wht.h"		/* WHT-nodes */

#include "emit.h"


/*======================================================================

  newwht()

  Create a new What node.

  */
#ifdef _PROTOTYPES_
WhtNod *newwht(Srcp *srcp, WhtKind wht, NamNod *nam)
                		/* IN - Source position */
                 		/* IN - What kind */
                 		/* IN - Name or NULL */
#else
WhtNod *newwht(srcp, wht, nam)
     Srcp *srcp;		/* IN - Source position */
     WhtKind wht;		/* IN - What kind */
     NamNod *nam;		/* IN - Name or NULL */
#endif
{
  WhtNod *new;

  new = NEW(WhtNod);

  new->srcp = *srcp;
  new->wht = wht;
  new->nam = nam;

  return(new);
}



/*======================================================================

  gewht()

  Generate code for a reference to What.

  */
#ifdef _PROTOTYPES_
void gewht(WhtNod *wht)
                 		/* IN - What to generate */
#else
void gewht(wht)
     WhtNod *wht;		/* IN - What to generate */
#endif
{
  switch (wht->wht) {
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
    genam(wht->nam);
    break;
  }
}


/*======================================================================

  duwht()

  Dump a WHT node.

  */
#ifdef _PROTOTYPES_
void duwht(WhtNod *wht)
#else
void duwht(wht)
     WhtNod *wht;
#endif
{
  if (wht == NULL) {
    put("NULL");
    return;
  }

  put("WHT: "); dusrcp(&wht->srcp); in();
  put("wht: ");
  switch (wht->wht) {
  case WHT_OBJ: put("OBJECT"); break;
  case WHT_LOC: put("LOCATION"); break;
  case WHT_ACT: put("ACTOR"); break;
  case WHT_ID: put("ID "); break;
  default: put("*** ERROR ***"); break;
  }
  nl();
  put("nam: "); dunam(wht->nam); out();
}



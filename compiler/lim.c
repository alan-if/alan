/*----------------------------------------------------------------------*\

				LIM.C
			     Limit Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "exp.h"                /* EXP-nodes */
#include "atr.h"                /* ATR-nodes */
#include "stm.h"		/* STM-nodes */
#include "chk.h"                /* CHK-nodes */
#include "vrb.h"                /* VRB-nodes */
#include "lim.h"		/* LIM-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "ext.h"                /* EXT-nodes */
#include "loc.h"                /* LOC-nodes */
#include "act.h"                /* ACT-nodes */
#include "evt.h"                /* EVT-nodes */
#include "rul.h"                /* RUL-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"




/*======================================================================

  newlim()

  Allocates and initialises a limnod.

  */
LimNod *newlim(Srcp *srcp,	/* IN - Source Position */
	       AtrNod *atr,	/* IN - The attribute */
	       List *stms)	/* IN - Statments */
{
  LimNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(LimNod);

  new->srcp = *srcp;
  new->atr = atr;
  new->stms = stms;

  return(new);
}




/*======================================================================

  anlim()

  Analyze one limit. The attributes that defines the limits must be
  default attributes beacuse we must be able to check them at run-time.
  The predefined attribute COUNT is also allowed.

  */
void anlim(LimNod *lim)		/* IN - The container to analyze */
{
  AtrNod *atr, *a;		/* Attribute nodes */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Analyze the attribute */
  atr = lim->atr;
  if (strcmp(atr->nam->str, "count") == 0)
    atr->nam->code = 0;		/* Use zero for the COUNT attribute */
  else {
    a = findatr(atr->nam->str, adv.oatrs, adv.atrs);
    if (a == NULL)
      lmLog(&atr->srcp, 407, sevERR, "");
    else if (atr->typ != TYPINT)
      unimpl(&atr->srcp, "Analyzer");
    else
      atr->nam->code = a->nam->code;
  }

  /* Analyze statments */
  anstms(lim->stms, NULL, NULL, NULL);
}



/*----------------------------------------------------------------------

  gelim()

  Generate one limit for a container.

  */
static void gelim(LimNod *lim,	/* IN - The limit */
		  int cnt)	/* IN - the code of the container */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Generate statements */
  lim->stmadr = emadr();	/* Save ACODE address to statements */
  gestms(lim->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geliment()

  Generate a limit table entry for one container.

  */
static void geliment(LimNod *lim) /* IN - The limit to generate for */
{
  emit(lim->atr->nam->code);
  emit(lim->atr->val);
  emit(lim->stmadr);
}


/*======================================================================

  gelims()

  Generate limit checks for one container.

  */
Aword gelims(CntNod *cnt)	/* IN - The container to generate for */
{
  List *lst;		/* List of limits */
  Aword limadr;

  if (cnt->lims == NULL)
    return(0);

  /* First code for all limits */
  for (lst = cnt->lims; lst != NULL; lst = lst->next)
    gelim(lst->element.lim, cnt->code);

  limadr = emadr();		/* Save ACODE address to limit table */
  for (lst = cnt->lims; lst != NULL; lst = lst->next)
    geliment(lst->element.lim);
  emit(EOF);
  return(limadr);
}



/*======================================================================

  dulim()

  Dump a limit node.

  */
void dulim(LimNod *lim)
{
  put("LIM: "); dusrcp(&lim->srcp); in();
  put("atr: "); duatr(lim->atr); nl();
  put("stms: "); dulst(lim->stms, LIST_STM); out();
}



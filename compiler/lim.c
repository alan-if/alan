/*----------------------------------------------------------------------*\

				LIM.C
			     Limit Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "atr_x.h"
#include "lst_x.h"
#include "sym_x.h"
#include "stm_x.h"

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "exp.h"                /* EXP-nodes */
#include "chk.h"                /* CHK-nodes */
#include "vrb.h"                /* VRB-nodes */
#include "lim.h"		/* LIM-nodes */
#include "ext.h"                /* EXT-nodes */
#include "ins.h"                /* INS-nodes */
#include "evt.h"                /* EVT-nodes */
#include "rul.h"                /* RUL-nodes */

#include "emit.h"

#include "../interpreter/acode.h"

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
  if (strcmp(atr->id->string, "count") == 0)
    atr->id->code = I_COUNT;		/* Use instruction code for COUNT meta attribute */
  else {
    a = findAttribute(NULL, atr->id);
    if (a == NULL)
      lmLog(&atr->srcp, 407, sevERR, "");
    else if (atr->type != INTEGER_TYPE)
      unimpl(&atr->srcp, "Analyzer");
    else
      atr->id->symbol->code = a->id->symbol->code;
  }

  /* Analyze statments */
  analyzeStatements(lim->stms, NULL);
}



/*----------------------------------------------------------------------

  gelim()

  Generate one limit for a container.

  */
static void gelim(LimNod *lim, int cnt, int currentInstance)
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Generate statements */
  lim->stmadr = emadr();	/* Save ACODE address to statements */
  gestms(lim->stms, currentInstance);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geliment()

  Generate a limit table entry for one container.

  */
static void geliment(LimNod *lim) /* IN - The limit to generate for */
{
  if (lim->atr->id->symbol == NULL) {
    if (lim->atr->id->code == I_COUNT)
      emit(I_COUNT);
    else
      syserr("Generating a limit attribute without symbol.");
  } else
    emit(lim->atr->id->symbol->code);
  emit(lim->atr->value);
  emit(lim->stmadr);
}


/*======================================================================

  gelims()

  Generate limit checks for one container.

  */
Aword gelims(CntNod *cnt, int currentInstance)
{
  List *lst;		/* List of limits */
  Aword limadr;

  if (cnt->lims == NULL)
    return(0);

  /* First code for all limits */
  for (lst = cnt->lims; lst != NULL; lst = lst->next)
    gelim(lst->element.lim, cnt->code, currentInstance);

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
  put("LIM: "); dumpSrcp(&lim->srcp); in();
  put("atr: "); dumpAttribute(lim->atr); nl();
  put("stms: "); dumpList(lim->stms, LIST_STM); out();
}



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
	       Attribute *atr,	/* IN - The attribute */
	       List *stms)	/* IN - Statments */
{
  LimNod *new;			/* The newly allocated area */

  showProgress();

  new = NEW(LimNod);

  new->srcp = *srcp;
  new->atr = atr;
  new->stms = stms;

  return(new);
}



/*======================================================================*/
void analyzeLimit(LimNod *lim)
{
  /* Analyze one limit. The attributes that defines the limits must be
     attributes for all instances the container accepts because we
     must be able to check them at run-time.  The predefined attribute
     COUNT is also allowed.
  */

  Attribute *atr, *a;		/* Attribute nodes */

  showProgress();

  /* Analyze the attribute */
  atr = lim->atr;
  if (compareStrings(atr->id->string, "count") == 0)
    atr->id->code = I_COUNT;	/* Use instruction code for COUNT meta attribute */
  else {
    a = findAttribute(objectSymbol->fields.entity.props->attributes, atr->id);
    if (a == NULL)
      lmLog(&atr->srcp, 407, sevERR, "object");
    else if (atr->type != INTEGER_TYPE)
      unimpl(&atr->srcp, "Analyzer");
    else
      atr->id->code = a->id->code;
  }

  /* Analyze statments */
  analyzeStatements(lim->stms, NULL);
}



/*----------------------------------------------------------------------*/
static void generateLimit(LimNod *lim)
{
  showProgress();

  /* Generate statements */
  lim->stmadr = nextEmitAddress();	/* Save ACODE address to statements */
  generateStatements(lim->stms);
  emit0(I_RETURN);
}



/*----------------------------------------------------------------------*/
static void generateLimitEntry(LimNod *lim)
{
  if (lim->atr->id->code == I_COUNT)
    emit(1-I_COUNT);
  else
    emit(lim->atr->id->code);
  emit(lim->atr->value);
  emit(lim->stmadr);
}


/*======================================================================*/
Aword generateLimits(ContainerBody *info)
{
  List *lst;		/* List of limits */
  Aword limadr;

  if (info->limits == NULL)
    return(0);

  /* First code for all limits */
  for (lst = info->limits; lst != NULL; lst = lst->next)
    generateLimit(lst->element.lim);

  limadr = nextEmitAddress();		/* Save ACODE address to limit table */
  for (lst = info->limits; lst != NULL; lst = lst->next)
    generateLimitEntry(lst->element.lim);
  emit(EOF);
  return(limadr);
}



/*======================================================================*/
void dumpLimit(LimNod *lim)
{
  put("LIM: "); dumpSrcp(&lim->srcp); indent();
  put("atr: "); dumpAttribute(lim->atr); nl();
  put("stms: "); dumpList(lim->stms, STATEMENT_LIST); out();
}



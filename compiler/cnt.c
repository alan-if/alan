/*----------------------------------------------------------------------*\

				CNT.C
			   Container Nodes

\*----------------------------------------------------------------------*/

#include "cnt_x.h"

/* IMPORTS: */
#include "alan.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "stm_x.h"

#include "adv.h"		/* ADV-node */
#include "stm.h"		/* STM-nodes */
#include "elm.h"		/* ELM-nodes */
#include "lim.h"		/* LIM-nodes */

#include "lmList.h"
#include "acode.h"
#include "util.h"
#include "emit.h"
#include "dump.h"



/* PUBLIC: */

int containerCount = 0;



/*======================================================================

  newContainer()

 */
CntNod *newContainer(Srcp *srcp, /* IN - Source Position */
		     List *lims, /* IN - Limits */
		     List *hstms, /* IN - Header statements */
		     List *estms) /* IN - Else (empty) statements */
{
  CntNod *new;		/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(CntNod);

  new->srcp = *srcp;
  new->lims = lims;
  new->hstms = hstms;
  new->estms = estms;

  new->code = ++containerCount;

  return(new);
}




/*======================================================================

  verifyContainer()

 */
void verifyContainer(WhtNod *wht,
		     Context *context)
{
  Symbol *sym;

  if (wht == NULL)
    return;

  switch (wht->kind) {
  case WHT_ID:
    sym = symcheck(wht->id, INSTANCE_SYMBOL, context);
    if (sym)
      switch (sym->kind) {
      case INSTANCE_SYMBOL:
	if (sym->fields.claOrIns.slots->container == NULL)
	  lmLog(&wht->srcp, 318, sevERR, wht->id->string);
	break;
      case PARAMETER_SYMBOL:
	if (sym->fields.parameter.element->res == NULL
	    || sym->fields.parameter.element->res->kind != CONTAINER_RESTRICTION)
	  lmLogv(&wht->srcp, 312, sevERR, wht->id->string, "a container", NULL);
	break;
      default:
	syserr("Unexpected symbol kind in verifyContainer()");
      }
    break;

  case WHT_LOC:
  case WHT_ACT:
    lmLog(&wht->srcp, 311, sevERR, "a Container");
    break;

  default:
    syserr("Unrecognized switch in cntcheck()");
    break;
  }
}



/*======================================================================

  analyzeContainer()

  Analyze one container.

  */
void analyzeContainer(CntNod *cnt, Context *context)
{
  List *lims;			/* List of limits */

  if (cnt == NULL) return;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if (cnt->ownerSlots == NULL)
    syserr("Container without an owner.");

  /* Analyze the limits */
  for (lims = cnt->lims; lims != NULL; lims = lims->next)
    anlim(lims->element.lim);

  /* Analyze header and empty statments */
  anstms(cnt->hstms, context);
  anstms(cnt->estms, context);
}


/*----------------------------------------------------------------------

  gecnt()

  Generate code for one container.

  */
static void gecnt(CntNod *cnt)	/* IN - The container to generate */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  cnt->limadr = gelims(cnt);

  if (cnt->hstms != NULL) {
    cnt->hadr = emadr();
    gestms(cnt->hstms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    cnt->hadr = 0;
  
  if (cnt->estms != NULL) {
    cnt->eadr = emadr();
    gestms(cnt->estms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    cnt->eadr = 0;
}



/*----------------------------------------------------------------------

  gecntent()

  Generate an entry in the global container list.

  */
static void gecntent(CntNod *cnt)
{
  ContainerEntry entry;

  entry.limits = cnt->limadr;
  entry.header = cnt->hadr;
  entry.empty = cnt->eadr;
  entry.owner = cnt->ownerSlots->id->symbol->code;
  emitEntry(&entry, sizeof(entry));
}


/*======================================================================

  generateContainers()

  */
Aaddr generateContainers(AcdHdr *header)
{
  List *lst;			/* The list of containers */
  Aaddr adr;

  if (adv.cnts == NULL)		/* Any containers at all? */
    adr = emadr();
  else {
    /* Limits, header and empty statements for the container */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      gecnt(lst->element.cnt);
  
    adr = emadr();		/* Save ACODE address to container list */
    /* Container list */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      gecntent(lst->element.cnt);
  }
  emit(EOF);

  header->containerMax = containerCount;

  return(adr);
}



/*======================================================================

  dumpContainer()

  */
void dumpContainer(CntNod *container)
{
  if (container == NULL) {
    put("NULL");
    return;
  }

  put("CONTAINER: "); dumpPointer(container); dumpSrcp(&container->srcp); in();
  put("code: "); dumpInt(container->code); nl();
  put("ownerSlots: "); dumpPointer(container->ownerSlots); nl();
  put("lims: "); dulst(container->lims, LIST_LIM); nl();
  put("limadr: "); dumpAddress(container->limadr); nl();
  put("hstms: "); dulst(container->hstms, LIST_STM); nl();
  put("hadr: "); dumpAddress(container->hadr); nl();
  put("estms: "); dulst(container->estms, LIST_STM); nl();
  put("eadr: "); dumpAddress(container->eadr); out();
}



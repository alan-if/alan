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
#include "../interpreter/acode.h"
#include "util.h"
#include "emit.h"
#include "dump.h"



/* PUBLIC: */

int containerCount = 0;



/*======================================================================

  newContainer()

 */
Container *newContainer(Srcp *srcp, /* IN - Source Position */
		     List *lims, /* IN - Limits */
		     List *hstms, /* IN - Header statements */
		     List *estms) /* IN - Else (empty) statements */
{
  Container *new;		/* The newly allocated area */

  showProgress();

  new = NEW(Container);

  new->srcp = *srcp;
  new->lims = lims;
  new->hstms = hstms;
  new->estms = estms;

  new->code = ++containerCount;

  return(new);
}




/*======================================================================*/
void verifyContainer(What *wht,
		     Context *context)
{
  Symbol *sym;

  if (wht == NULL)
    return;

  switch (wht->kind) {
  case WHAT_ID:
    sym = symcheck(wht->id, INSTANCE_SYMBOL, context);
    if (sym)
      switch (sym->kind) {
      case INSTANCE_SYMBOL:
	if (sym->fields.entity.props->container == NULL)
	  lmLog(&wht->srcp, 318, sevERR, wht->id->string);
	break;
      case PARAMETER_SYMBOL:
	if (!sym->fields.parameter.restrictedToContainer)
	  lmLogv(&wht->srcp, 312, sevERR, wht->id->string, "a container", NULL);
	break;
      default:
	syserr("Unexpected symbol kind in verifyContainer()");
      }
    break;

  case WHAT_LOCATION:
  case WHAT_ACTOR:
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
void analyzeContainer(Container *cnt, Context *context)
{
  List *lims;			/* List of limits */

  if (cnt == NULL) return;

  showProgress();

  if (cnt->ownerProperties == NULL)
    syserr("Container without an owner.");

  /* Analyze the limits */
  for (lims = cnt->lims; lims != NULL; lims = lims->next)
    anlim(lims->element.lim);

  /* Analyze header and empty statments */
  analyzeStatements(cnt->hstms, context);
  analyzeStatements(cnt->estms, context);
}


/*----------------------------------------------------------------------*/
static void generateContainer(Container *cnt)
{
  showProgress();

  cnt->limadr = generateLimits(cnt);

  if (cnt->hstms != NULL) {
    cnt->hadr = emadr();
    generateStatements(cnt->hstms);
    emit0(I_RETURN);
  } else
    cnt->hadr = 0;
  
  if (cnt->estms != NULL) {
    cnt->eadr = emadr();
    generateStatements(cnt->estms);
    emit0(I_RETURN);
  } else
    cnt->eadr = 0;
}



/*----------------------------------------------------------------------

  gecntent()

  Generate an entry in the global container list.

  */
static void gecntent(Container *cnt)
{
  ContainerEntry entry;

  entry.limits = cnt->limadr;
  entry.header = cnt->hadr;
  entry.empty = cnt->eadr;
  entry.owner = cnt->ownerProperties->id->symbol->code;
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
      generateContainer(lst->element.cnt);
  
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
void dumpContainer(Container *container)
{
  if (container == NULL) {
    put("NULL");
    return;
  }

  put("CONTAINER: "); dumpPointer(container); dumpSrcp(&container->srcp); in();
  put("code: "); dumpInt(container->code); nl();
  put("ownerProperties: "); dumpPointer(container->ownerProperties); nl();
  put("lims: "); dumpList(container->lims, LIMIT_LIST); nl();
  put("limadr: "); dumpAddress(container->limadr); nl();
  put("hstms: "); dumpList(container->hstms, STATEMENT_LIST); nl();
  put("hadr: "); dumpAddress(container->hadr); nl();
  put("estms: "); dumpList(container->estms, STATEMENT_LIST); nl();
  put("eadr: "); dumpAddress(container->eadr); out();
}



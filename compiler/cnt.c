/*----------------------------------------------------------------------*\

				CNT.C
			   Container Nodes

\*----------------------------------------------------------------------*/

#include "cnt_x.h"

/* IMPORTS: */
#include "alan.h"

#include "srcp_x.h"
#include "adv_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "chk_x.h"

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



/*======================================================================*/
ContainerBody *newContainerBody(Srcp *srcp,
				Bool opaque,
				List *lims,
				List *hstms,
				List *estms,
				List *extractChecks,
				List *extractStatements)
{
  ContainerBody *new;		/* The newly allocated area */

  new = NEW(ContainerBody);

  new->srcp = *srcp;
  new->analyzed = FALSE;
  new->generated = FALSE;
  new->opaque = opaque;
  new->limits = lims;
  new->hstms = hstms;
  new->estms = estms;
  new->extractChecks = extractChecks;
  new->extractStatements = extractStatements;

  return(new);
}


/*======================================================================*/
Container *newContainer(ContainerBody *body)
{
  Container *new;		/* The newly allocated area */

  showProgress();

  new = NEW(Container);
  new->ownerProperties = NULL;
  new->body = body;

  adv.cnts = concat(adv.cnts, new, CONTAINER_LIST);

  return(new);
}

/*----------------------------------------------------------------------*/
Bool thisIsaContainer(Context *context)
{
  Properties *props;

  if (context->instance != NULL)
    props = context->instance->props;
  else if (context->class != NULL)
    props = context->class->props;
  else
    return FALSE;

  return props->container != NULL;
}


/*======================================================================*/
void verifyContainer(What *wht,
		     Context *context)
{
  Symbol *sym;

  if (wht == NULL)
    return;

  switch (wht->kind) {
  case WHAT_THIS:
    if (!thisIsaContainer(context))
      lmLog(&wht->srcp, 309, sevERR, "");
    break;
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
	syserr("Unexpected symbol kind in '%s()'", __FUNCTION__);
      }
    break;

  case WHAT_LOCATION:
  case WHAT_ACTOR:
    lmLog(&wht->srcp, 311, sevERR, "a Container");
    break;

  default:
    syserr("Unrecognized switch in '%s()'", __FUNCTION__);
    break;
  }
}



/*======================================================================*/
void analyzeContainer(Container *theContainer, Context *context)
{
  List *lims;			/* List of limits */

  if (theContainer == NULL) return;

  showProgress();

  if (context->kind == INSTANCE_CONTEXT)
    theContainer->ownerProperties = context->instance->props;

  if (!theContainer->body->analyzed) {
    /* Analyze the limits */
    for (lims = theContainer->body->limits; lims != NULL; lims = lims->next)
      analyzeLimit(lims->element.lim);

    /* Analyze header and empty statments */
    analyzeStatements(theContainer->body->hstms, context);
    analyzeStatements(theContainer->body->estms, context);
    theContainer->body->analyzed = TRUE;

    /* Analyze the extract checks and statements */
    analyzeChecks(theContainer->body->extractChecks, context);
    analyzeStatements(theContainer->body->extractStatements, context);
  }
}



/*======================================================================*/
void numberContainers(void)
{
  List *lst;			/* The list of containers */

  /* We must number the containers in the order that they have in the
     adv-list since that is the order the container bodies will be
     generated into the ContainerEntry table */
  for (lst = adv.cnts; lst != NULL; lst = lst->next)
    if (lst->element.cnt->ownerProperties != NULL)
      lst->element.cnt->code = ++containerCount;
}


/*----------------------------------------------------------------------*/
static void generateContainerBody(ContainerBody *body)
{
  showProgress();

#ifndef DONT_OPTIMIZE_CONTAINER_BODY_GENERATION
  if (!body->generated) {
#endif
    body->limadr = generateLimits(body);

    if (body->hstms != NULL) {
      body->hadr = nextEmitAddress();
      generateStatements(body->hstms);
      emit0(I_RETURN);
    } else
      body->hadr = 0;

    if (body->estms != NULL) {
      body->eadr = nextEmitAddress();
      generateStatements(body->estms);
      emit0(I_RETURN);
    } else
      body->eadr = 0;

    if (body->extractChecks != NULL) {
      body->extractChecksAddress = generateChecks(body->extractChecks);
    } else
      body->extractChecksAddress = 0;

    if (body->extractStatements != NULL) {
      body->extractStatementsAddress = nextEmitAddress();
      generateStatements(body->extractStatements);
      emit0(I_RETURN);
    } else
      body->extractStatementsAddress = 0;
    body->generated = TRUE;
  }
}



/*----------------------------------------------------------------------*/
static void generateContainerEntry(Container *cnt)
{
  ContainerEntry entry;

  entry.limits = cnt->body->limadr;
  entry.header = cnt->body->hadr;
  entry.empty = cnt->body->eadr;
  entry.extractChecks = cnt->body->extractChecksAddress;
  entry.extractStatements = cnt->body->extractStatementsAddress;
  entry.owner = cnt->ownerProperties->id->symbol->code;
  emitEntry(&entry, sizeof(entry));
}


/*======================================================================*/
Aaddr generateContainers(AcdHdr *header)
{
  List *lst;			/* The list of containers */
  Aaddr adr;

  if (adv.cnts == NULL)		/* Any containers at all? */
    adr = nextEmitAddress();
  else {
    /* Limits, header and empty statements for the container */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      if (lst->element.cnt->ownerProperties != NULL)
	generateContainerBody(lst->element.cnt->body);
  
    adr = nextEmitAddress();	/* Save ACODE address to container list */
    /* Container list */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      if (lst->element.cnt->ownerProperties != NULL)
	generateContainerEntry(lst->element.cnt);
  }
  emit(EOF);

  header->containerMax = containerCount;

  return(adr);
}



/*======================================================================*/
void dumpContainer(Container *container)
{
  if (container == NULL) {
    put("NULL");
    return;
  }

  put("CONTAINER: "); dumpPointer(container); dumpSrcp(&container->body->srcp); indent();
  put("code: "); dumpInt(container->code); nl();
  put("ownerProperties: "); dumpPointer(container->ownerProperties); nl();
  put("body: "); dumpPointer(container->body); nl();
  put("body.lims: "); dumpList(container->body->limits, LIMIT_LIST); nl();
  put("body.limadr: "); dumpAddress(container->body->limadr); nl();
  put("body.hstms: "); dumpList(container->body->hstms, STATEMENT_LIST); nl();
  put("body.hadr: "); dumpAddress(container->body->hadr); nl();
  put("body.estms: "); dumpList(container->body->estms, STATEMENT_LIST); nl();
  put("body.eadr: "); dumpAddress(container->body->eadr); out();
}



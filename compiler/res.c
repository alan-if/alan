/*----------------------------------------------------------------------*\

                               RES.C
		   Syntax Element Restriction Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "res_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "cla_x.h"
#include "context_x.h"

#include "lmList.h"
#include "emit.h"
#include "../interpreter/acode.h"
#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newRestriction()

  Allocates and initialises an element class restriction node.

 */
ResNod *newRestriction(
    Srcp *srcp,
    IdNode *parameterId,
    RestrictionKind kind,
    IdNode *classId,
    List *stms)
{
  ResNod *new;			/* The newly created node */

  showProgress();

  new = NEW(ResNod);

  new->srcp = *srcp;
  new->parameterId = parameterId;
  new->kind = kind;
  new->classId = classId;
  new->stms = stms;

  return(new);
}


/*----------------------------------------------------------------------*/
static void resolveParameterClass(ResNod *res, Symbol *parameter)
{
  Symbol *classSymbol;

  /* Analyse the class list and evaluate possibly to a class symbol ref. */
  switch (res->kind) {
  case ID_RESTRICTION: 
    classSymbol = lookup(res->classId->string);
    if (classSymbol != NULL)
      if (classSymbol->kind != CLASS_SYMBOL) {
	lmLog(&res->classId->srcp, 317, sevERR, "");
	classSymbol = NULL;
      } else {
	res->classId->symbol = classSymbol;
	res->classId->code = classSymbol->code;
      }
    else
      lmLog(&res->classId->srcp, 317, sevERR, "");

    /* Set the class in the corresponding parameter symbol */
    if (parameter->fields.parameter.class != NULL)
      /* It already has a class, warn if new restriction is not a subclass */
      if (!inheritsFrom(classSymbol, parameter->fields.parameter.class))
	lmLogv(&res->classId->srcp, 427, sevWAR, parameter->string,
	       parameter->fields.parameter.class->string, NULL);
    parameter->fields.parameter.class = classSymbol;

    if (classSymbol == stringSymbol)
      parameter->fields.parameter.type = STRING_TYPE;
    else if (classSymbol == integerSymbol)
      parameter->fields.parameter.type = INTEGER_TYPE;
    else
      parameter->fields.parameter.type = INSTANCE_TYPE;
    break;

  case CONTAINER_RESTRICTION:
    parameter->fields.parameter.restrictedToContainer = TRUE;
    parameter->fields.parameter.type = INSTANCE_TYPE;
    break;

  case STRING_RESTRICTION:
    parameter->fields.parameter.class = NULL;
    parameter->fields.parameter.type = STRING_TYPE;
    break;

  case INTEGER_RESTRICTION:
    parameter->fields.parameter.class = NULL;
    parameter->fields.parameter.type = INTEGER_TYPE;
    break;

  default:
    syserr("Unimplemented restriction kind in '%s()'", __FUNCTION__);
    break;
  }

}

/*----------------------------------------------------------------------

  analyzeRestriction()

  Analyzes one element class restriction node.

 */
static void analyzeRestriction(
  ResNod *res,			/* IN - Restriction node to analyze */
  Symbol *theVerb		/* IN - The verb symbol */
)
{
  Symbol *parameter;
  Context *context = newContext(VERB_CONTEXT);

  parameter = lookupParameter(res->parameterId, theVerb->fields.verb.parameterSymbols);
  if (parameter == NULL)
    lmLog(&res->parameterId->srcp, 222, sevERR, res->parameterId->string);
  else
    resolveParameterClass(res, parameter);

  /* Analyse the statements to execute if the restrictions was not met */
  /* FIXME: we need to send the restriction inverted in the context also */
  analyzeStatements(res->stms, context);
}



/*======================================================================*/
void analyzeRestrictions(
    List *restrictions,		/* IN - List of nodes to analyze */
    Symbol *theVerb		/* IN - The verb */
)
{
  List *lst;

  for (lst = restrictions; lst != NULL; lst = lst->next)
    analyzeRestriction(lst->element.res, theVerb);
}



/*----------------------------------------------------------------------*/
static void generateRestrictionStatements(ResNod *res)
{
  res->stmadr = nextEmitAddress();
  generateStatements(res->stms);
  emit0(I_RETURN);
}



/*----------------------------------------------------------------------*/
static void generateRestrictionEntry(ResNod *res)
{
  RestrictionEntry restriction;

  restriction.parameter = res->parameterId->code;
  switch (res->kind) {
  case ID_RESTRICTION:
    restriction.class = res->classId->code;
    break;
  case CONTAINER_RESTRICTION:
    restriction.class = RESTRICTIONCLASS_CONTAINER;
    break;
  case INTEGER_RESTRICTION:
    restriction.class = RESTRICTIONCLASS_INTEGER;
    break;
  case STRING_RESTRICTION:
    restriction.class = RESTRICTIONCLASS_STRING;
    break;
  default:
    syserr("Unexpected RestrictionKind in '%s()'", __FUNCTION__);
    break;
  }
  restriction.stms = res->stmadr;

  emitEntry(&restriction, sizeof(restriction));
}



/*======================================================================*/
Aaddr generateRestrictions(List *restrictions, Syntax *stx)
{
  List *lst;
  Aaddr address;

  for (lst = restrictions; lst != NULL; lst = lst->next)
    generateRestrictionStatements(lst->element.res);

  address = nextEmitAddress();
  for (lst = restrictions; lst != NULL; lst = lst->next)
    generateRestrictionEntry(lst->element.res);
  emit(EOF);

  return(address);
}



/*----------------------------------------------------------------------*/
static void dumpRestrictionKind(RestrictionKind kind)
{
  switch (kind) {
  case ID_RESTRICTION: put("ID"); break;
  case INTEGER_RESTRICTION: put("INTEGER"); break;
  case STRING_RESTRICTION: put("STRING"); break;
  case CONTAINER_RESTRICTION: put("CONTAINER"); break;
  default: put("*** UNKNOWN ***"); break;
  }
}



/*======================================================================*/
void dumpRestriction(ResNod *res)
{
  if (res == NULL) {
    put("NULL");
    return;
  }

  put("RES: "); dumpSrcp(&res->srcp); indent();
  put("kind: "); dumpRestrictionKind(res->kind); nl();
  put("parameterId: "); dumpId(res->parameterId); nl();
  put("classId: "); dumpId(res->classId); nl();
  put("stms: "); dumpList(res->stms, STATEMENT_LIST); out();
}

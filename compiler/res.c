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
#include "dump_x.h"

#include "lmList.h"
#include "emit.h"
#include "../interpreter/acode.h"


/* PUBLIC: */


/*======================================================================*/
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
	lmLog(&res->classId->srcp, 323, sevERR, "");
	classSymbol = NULL;
      } else {
	res->classId->symbol = classSymbol;
	res->classId->code = classSymbol->code;
      }
    else
      lmLog(&res->classId->srcp, 323, sevERR, "");

    /* Set the class in the corresponding parameter symbol */
    if (parameter->fields.parameter.class != NULL && classSymbol != NULL)
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
    SYSERR("Unimplemented restriction kind");
    break;
  }

}

/*----------------------------------------------------------------------*/
static void analyzeRestriction(ResNod *res, Symbol *theVerb) {
  Symbol *parameter;
  Context *context = newContext(VERB_CONTEXT, theVerb);

  /* Analyze the statements to execute if the restrictions was not
     met. The parameter classes are set incrementally for each
     subsequent restriction analyze above, so each parameter is now
     set to the class of all previous restrictions. During run-time
     those restrictions have all been passed and ensure each parameter
     class, so all we have to do is to set up a relevant verb
     context. */
  analyzeStatements(res->stms, context);

  /* Analyze this restriction and set the new class of the parameter */
  parameter = lookupParameter(res->parameterId,
			      theVerb->fields.verb.parameterSymbols);
  if (parameter == NULL)
    lmLog(&res->parameterId->srcp, 222, sevERR, res->parameterId->string);
  else
    resolveParameterClass(res, parameter);
}



/*======================================================================*/
void analyzeRestrictions(
    List *restrictions,		/* IN - List of nodes to analyze */
    Symbol *theVerb		/* IN - The verb */
)
{
  List *lst;

  TRAVERSE(lst, restrictions)
    /* Analyze the actual restriction conditions. */
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
    SYSERR("Unexpected RestrictionKind");
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

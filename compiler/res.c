/*----------------------------------------------------------------------*\

                               RES.C
		   Syntax Element Restriction Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lmList.h"

#include "res_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "cla_x.h"


#include "emit.h"
#include "acode.h"
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

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ResNod);

  new->srcp = *srcp;
  new->parameterId = parameterId;
  new->kind = kind;
  new->classId = classId;
  new->stms = stms;

  return(new);
}



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
    parameter->fields.parameter.class = classSymbol;
    parameter->fields.parameter.type = INSTANCE_TYPE;
    break;

  case CONTAINER_RESTRICTION:
    parameter->fields.parameter.class = NULL;
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
    syserr("Unimplemented restriction kind in resolveParameterClass()");
    break;
  }

}

/*----------------------------------------------------------------------

  analyzeRestriction()

  Analyzes one element class restriction node.

 */
static void analyzeRestriction(
  ResNod *res,			/* IN - Restriction node to analyze */
  List *parameterSymbols	/* IN - The syntax parameters as symbols */
)
{
  Symbol *parameter;
  Context context;

  parameter = findParameter(res->parameterId, parameterSymbols);
  if (parameter == NULL)
    lmLog(&res->parameterId->srcp, 222, sevERR, res->parameterId->string);

  resolveParameterClass(res, parameter);

  /* Analyse the statements to execute if the restrictions was not met */
  /* FIXME: we need to send the restriction inverted in the context also */
  context.kind = VERB_CONTEXT;
  anstms(res->stms, &context);
}



/*======================================================================

  analyzeRestrictions()

  Analyzes all class restriction nodes in a list by calling the analyzer
  for each.

 */
void analyzeRestrictions(
    List *restrictions,		/* IN - List of nodes to analyze */
    List *parameterSymbols	/* IN - The parameters */
)
{
  List *lst;

  for (lst = restrictions; lst != NULL; lst = lst->next)
    analyzeRestriction(lst->element.res, parameterSymbols);
}



/*----------------------------------------------------------------------

  generateRestrictionParts()

 */
static void generateRestrictionParts(ResNod *res)
{
  res->stmadr = emadr();
  gestms(res->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  generateRestrictionEntry()

 */
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
    syserr("Unexpected RestrictionKind in generateRestrictionEntry()");
    break;
  }
  restriction.stms = res->stmadr;

  emitEntry(&restriction, sizeof(restriction));
}



/*======================================================================

  generateRestrictions()

 */
Aaddr generateRestrictions(
    List *restrictions,		/* IN - The element class restriction nodes */
    StxNod *stx)		/* IN - Syntax node containing the res */
{
  List *lst;
  Aaddr address;

  for (lst = restrictions; lst != NULL; lst = lst->next)
    generateRestrictionParts(lst->element.res);

  address = emadr();
  for (lst = restrictions; lst != NULL; lst = lst->next)
    generateRestrictionEntry(lst->element.res);

  /* End it with an End of file and the code for the verb */
  emit(EOF);
  generateId(stx->id);

  return(address);
}



/*----------------------------------------------------------------------

  dumpRestrictionKind()

*/
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



/*======================================================================

  dumpRestriction()

  Dump a syntax parameter restriction node.

 */
void dumpRestriction(ResNod *res)
{
  if (res == NULL) {
    put("NULL");
    return;
  }

  put("RES: "); dumpSrcp(&res->srcp); in();
  put("kind: "); dumpRestrictionKind(res->kind); nl();
  put("parameterId: "); dumpId(res->parameterId); nl();
  put("classId: "); dumpId(res->classId); nl();
  put("stms: "); dulst(res->stms, LIST_STM); out();
}

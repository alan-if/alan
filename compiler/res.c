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
    Srcp *srcp,			/* IN - Source Position */
    IdNode *id,			/* IN - The name */
    IdNode *classId,		/* IN - Allowed class */
    List *stms)			/* IN - Statements to execute otherwise */
{
  ResNod *new;			/* The newly created node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ResNod);

  new->srcp = *srcp;
  new->id = id;
  new->classId = classId;
  new->stms = stms;

  return(new);
}



static SymNod *findParameter(ResNod *res, List *parameterSymbols)
{
  List *p;

  for (p = parameterSymbols; p != NULL; p = p->next) {
    if (p->element.sym->kind != PARAMETER_SYMBOL)
      syserr("Not a parameter symbol in analyzeRestriction()");
    if (equalId(res->id, p->element.sym->fields.parameter.element->id))
      return p->element.sym;
  }
  return NULL;
}


static void resolveParameterClass(ResNod *res, SymNod *parameter)
{
  SymNod *classSymbol;

  /* Analyse the class list and evaluate possibly to a class symbol ref. */
  if (res->classId == NULL) {
    classSymbol = object->slots->id->symbol;
  } else {
    /* FIXME: to handle literal types restriction, INTEGER, STRING, ... */

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
  }

  /* Set the class in the corresponding parameter symbol */
  parameter->fields.parameter.class = classSymbol;
  parameter->fields.parameter.type = INSTANCE_TYPE;
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
  SymNod *parameter;
  Context context;

  parameter = findParameter(res, parameterSymbols);
  if (parameter == NULL)
    lmLog(&res->id->srcp, 222, sevERR, res->id->string);

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

  restriction.parameter = res->id->code;
  restriction.class = res->classId->code;
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


/*======================================================================

  dures()

  Dump a syntax element Class restriction node.

 */
void dures(ResNod *res)
{
  if (res == NULL) {
    put("NULL");
    return;
  }

  put("RES: "); dumpSrcp(&res->srcp); in();
  put("id: "); dumpId(res->id); nl();
  put("class: "); dumpId(res->classId); nl();
  put("stms: "); dulst(res->stms, LIST_STM); out();
}

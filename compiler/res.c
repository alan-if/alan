/*----------------------------------------------------------------------*\

                               RES.C
		   Element Class Restriction Nodes

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

  newres()

  Allocates and initialises an element class restriction node.

 */
ResNod *newres(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,	/* IN - The name */
	       IdNode *classId,	/* IN - Allowed class */
	       List *stms)	/* IN - Statements to execute otherwise */
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



/*----------------------------------------------------------------------

  anres()

  Analyzes one element class restriction node.

 */
static void anres(
     ResNod *res,		/* IN - Restriction node to analyze */
     List *params		/* IN - The syntax parameters as symbols */
)
{
  Bool found = FALSE;
  List *p;
  Context context;
  SymNod *classSymbol;

  /* Check for the id in the parameter list */
  for (p = params; p != NULL; p = p->next) {
    if (p->element.sym->kind != PARAMETER_SYMBOL)
      syserr("Not a parameter symbol in anres()");
    if (equalId(res->id, p->element.sym->fields.parameter.element->id)) {
      found = TRUE;
      break;
    }
  }
  if (!found)
    lmLog(&res->id->srcp, 222, sevERR, res->id->string);

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
  p->element.sym->fields.parameter.class = classSymbol;
  p->element.sym->fields.parameter.type = INSTANCE_TYPE;


  /* Analyse the statements to execute if the restrictions was not met */
  /* FIXME: we need to send the restriction inverted in the context also */
  context.kind = VERB_CONTEXT;
  anstms(res->stms, &context);
}



/*======================================================================

  anress()

  Analyzes all class restriction nodes in a list by calling the analyzer
  for each.

 */
void anress(
    List *ress,			/* IN - List of nodes to analyze */
    List *params		/* IN - The parameters */
)
{
  List *lst;

  for (lst = ress; lst != NULL; lst = lst->next)
    anres(lst->element.res, params);
}



/*----------------------------------------------------------------------

  geres()

  Generate code for one syntax class restriction node.

 */
static void geres(ResNod *res)	/* IN - Node to generate */
{
  res->stmadr = emadr();
  gestms(res->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geresent()

  Generate an entry for one restriction node.

 */
static void geresent(ResNod *res) /* IN - Node to generate */
{
  RestrictionEntry restriction;

  restriction.parameter = res->id->code;
  restriction.class = res->classId->code;
  restriction.stms = res->stmadr;

  emitEntry(&restriction, sizeof(restriction));
}



/*======================================================================

  geress()

  Generate the data structure for the element class restrictions.

 */
Aaddr geress(List *ress,	/* IN - The element class restriction nodes */
	     StxNod *stx)	/* IN - Syntax node containing the res */
{
  List *lst;
  Aaddr resadr;

  for (lst = ress; lst != NULL; lst = lst->next)
    geres(lst->element.res);

  resadr = emadr();
  for (lst = ress; lst != NULL; lst = lst->next)
    geresent(lst->element.res);

  /* End it with an End of file and the code for the verb */
  emit(EOF);
  generateId(stx->id);

  return(resadr);
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

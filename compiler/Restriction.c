/*----------------------------------------------------------------------*\

				Restriction.c

			      Restriction Nodes

\*----------------------------------------------------------------------*/


#include "Restriction.h"

#include "Element.h"
#include "Statement.h"
#include "Symbol.h"

#include "dump.h"
#include "lmList.h"




/*======================================================================

  newRestriction()

  Allocates and initialises a Restriction.

 */
#ifdef _PROTOTYPES_
Restriction *newRestriction(Srcp *srcp,	/* IN - Source position */
			    Id *id,	/* IN - The parameter identifier */
			    RestrictionKind kind, /* IN - Kind of Restriction */
			    Id *class,	/* IN - The class identifier */
			    List *statements) /* IN - Statements to execute if not */
#else
Restriction *newRestriction(srcp, id, kind, class, statements)
     Srcp *srcp;
     Id *id;
     RestrictionKind kind;
     Id *class;
     List *statements;
#endif
{
  Restriction *new;			/* The newly allocated area */

  new = NEW(Restriction);

  new->srcp = *srcp;
  new->id = id;
  new->kind = kind;
  new->class = class;
  new->statements = statements;

  return(new);
}



/*----------------------------------------------------------------------

  analyseRestriction()

  Analyze a Restriction.

 */
#ifdef _PROTOTYPES_
static void analyseRestriction(Restriction *restriction, List *parameters)
#else
static void analyseRestriction(restriction, parameters)
     Restriction *restriction;
     List *parameters;
#endif
{
  Bool found = FALSE;
  List *list;
  Symbol *symbol;

  /* Check for the id in the parameter list */
  for (list = parameters; list != NULL; list = list->next)
    if (equalIds(restriction->id, list->the.element->id)) {
      found = TRUE;
      break;
    }
  if (!found)
    lmLog(&restriction->id->srcp, 222, sevERR, restriction->id->string);
  if (restriction->kind == ID_RESTRICTION)
    symbol = symbolCheck(restriction->class, CLASS_SYMBOL);
  analyseStatements(restriction->statements, NULL, parameters);
}



/*======================================================================

  analyseRestrictions()

  Analyse a list of Restrictions.

 */
#ifdef _PROTOTYPES_
void analyseRestrictions(List *restrictions, List *parameters)
#else
void analyseRestrictions(restrictions, parameters)
     List *restrictions;
     List *parameters;
#endif
{
  List *list;

  /* Check for the id in the parameter list */
  for (list = restrictions; list != NULL; list = list->next)
    analyseRestriction(list->the.restriction, parameters);
}



/*----------------------------------------------------------------------

  dumpRestrictionKind()

  Dump a Restriction kind.

  */
#ifdef _PROTOTYPES_
static void dumpRestrictionKind(RestrictionKind kind)
#else
static void dumpRestrictionKind(kind)
     RestrictionKind kind;
#endif
{
  switch (kind) {
  case ID_RESTRICTION: put("ID"); break;
  case INTEGER_RESTRICTION: put("INTEGER"); break;
  case STRING_RESTRICTION: put("STRING"); break;
  default: put("*** ERROR unknown restriction kind ***");
  }
}



/*======================================================================

  dumpRestriction()

  Dump a Restriction node.

 */
#ifdef _PROTOTYPES_
void dumpRestriction(Restriction *restriction)
#else
void dumpRestriction(restriction)
     Restriction *restriction;
#endif
{
  if (restriction == NULL) {
    put("NULL");
    return;
  }

  put("RESTRICTION: "); dumpSrcp(&restriction->srcp); in();
  put("kind: "); dumpRestrictionKind(restriction->kind); nl();
  put("id: "); dumpId(restriction->id); nl();
  put("statements: "); dumpList(restriction->statements, STATEMENT_NODE); out();
}



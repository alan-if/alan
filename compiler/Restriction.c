/*----------------------------------------------------------------------*\

				Restriction.c

			      Restriction Nodes

\*----------------------------------------------------------------------*/


#include "Restriction.h"

#include "dump.h"




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


/*======================================================================

  analyseRestrictions()

  Analyze a list of Restrictions.

 */
#ifdef _PROTOTYPES_
void analyseRestrictions(List *restrictions, List *parameters)
#else
void analyseRestrictions(restrictions, parameters)
     List *restrictions;
     List *parameters;
#endif
{
  /* 4f - analyse restrictions */
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



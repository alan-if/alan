/*----------------------------------------------------------------------*\

				Does.c

			      Does Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Does.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newDoes()

  Allocates and initialises a Does.

 */
#ifdef _PROTOTYPES_
Does *newDoes(Srcp *srcp,	/* IN - Source position */
	      DoesKind kind,	/* IN - Kind of Does */
	      List *statements) /* IN - Statements */
#else
Does *newDoes(srcp, kind, statements)
     Srcp *srcp;
     DoesKind kind;
     List *statements;
#endif
{
  Does *new;			/* The newly allocated area */

  new = NEW(Does);

  new->srcp = *srcp;
  new->kind = kind;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseDoes()

  Analyze a DOES.

 */
#ifdef _PROTOTYPES_
void analyseDoes(Does *does, List *parameters)
#else
void analyseDoes(does, parameters)
     Does *does;
     List *parameters;
#endif
{
  if (does)
    analyseStatements(does->statements, NULL, parameters);
}



/*----------------------------------------------------------------------

  dumpQualifier()

  Dump a DOES qualifier.

  */
#ifdef _PROTOTYPES_
static void dumpQualifier(DoesKind qualifier)
#else
static void dumpQualifier(qualifier)
     DoesKind qualifier;
#endif
{
  switch (qualifier) {
  case DOES_AFTER:   put("AFTER"); break;
  case DOES_BEFORE:  put("BEFORE"); break;
  case DOES_ONLY:    put("ONLY"); break;
  case DOES_DEFAULT: put("DEFAULT"); break;
  default: put("*** ERROR ***");
  }
}



/*======================================================================

  dumpDoes()

  Dump a Does node.

 */
#ifdef _PROTOTYPES_
void dumpDoes(Does *does)
#else
void dumpDoes(does)
     Does *does;
#endif
{
  if (does == NULL) {
    put("NULL");
    return;
  }

  put("DOES: "); dumpSrcp(&does->srcp); in();
  put("kind: "); dumpQualifier(does->kind); nl();
  put("statememts: "); dumpList(does->statements, STATEMENT_NODE); out();
}



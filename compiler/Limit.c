/*----------------------------------------------------------------------*\

				Limit.c

			      Limit Nodes

\*----------------------------------------------------------------------*/


#include "Limit.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newLimit()

  Allocates and initialises a Limit.

 */
#ifdef _PROTOTYPES_
Limit *newLimit(Srcp *srcp,	/* IN - Source position */
		Attribute *attribute, /* IN - The limiting attribute & value */
		List *statements) /* IN - Statements */
#else
Limit *newLimit(srcp, attribute, statements)
     Srcp *srcp;
     Attribute *attribute;
     List *statements;
#endif
{
  Limit *new;			/* The newly allocated area */

  new = NEW(Limit);

  new->srcp = *srcp;
  new->attribute = attribute;
  new->statements = statements;

  return(new);
}


/*======================================================================

  analyseLimit()

  Analyze a LIMIT.

 */
#ifdef _PROTOTYPES_
void analyseLimit(Limit *limit)
#else
void analyseLimit(limit)
     Limit *limit;
#endif
{
  /* 4f - analyse the limiting attribute */
  analyseStatements(limit->statements, NULL, NULL);
}



/*======================================================================

  dumpLimit()

  Dump a Limit node.

 */
#ifdef _PROTOTYPES_
void dumpLimit(Limit *limit)
#else
void dumpLimit(limit)
     Limit *limit;
#endif
{
  if (limit == NULL) {
    put("NULL");
    return;
  }

  put("LIMIT: "); dumpSrcp(&limit->srcp); in();
  put("attribute: "); dumpAttribute(limit->attribute); nl();
  put("statememts: "); dumpList(limit->statements, STATEMENT_NODE); out();
}

/*----------------------------------------------------------------------*\

				Limit.c

			      Limit Nodes

\*----------------------------------------------------------------------*/


#include "Limit.h"

#include "Statement.h"

#include "lmList.h"
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
void analyseLimit(Limit *limit, Slot *slot)
#else
void analyseLimit(limit, slot)
     Limit *limit;
     Slot *slot;
#endif
{
  Attribute *attribute, *a;

  /* Analyse the limiting attribute */
  attribute = limit->attribute;
  if (strcmp(attribute->id->string, "count") == 0)
    attribute->code = 0;		/* Use zero for the COUNT attribute */
  else {
    a = findInheritedAttribute(attribute->id, slot);
    if (a == NULL)
      lmLog(&attribute->srcp, 407, sevERR, "");
    else if (attribute->type != INTEGER_TYPE)
      unimplemented(&attribute->srcp, "Analyzer");
    else
      attribute->code = a->code;
  }

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

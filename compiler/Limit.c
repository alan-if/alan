/*----------------------------------------------------------------------*\

				Limit.c

			      Limit Nodes

\*----------------------------------------------------------------------*/


#include "Limit.h"

#include "Statement.h"
#include "Class.h"

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


/*----------------------------------------------------------------------

  analyseLimit()

  Analyze a LIMIT. Only OBJECTS can be in a container, so limits can
  only refer attributes for all OBJECTS.

 */
#ifdef _PROTOTYPES_
static void analyseLimit(Limit *limit)
#else
static void analyseLimit(limit)
     Limit *limit;
#endif
{
  Attribute *attribute, *a;

  /* Analyse the limiting attribute */
  attribute = limit->attribute;
  if (strcmp(attribute->id->string, "count") == 0)
    attribute->code = 0;		/* Use zero for the COUNT attribute */
  else {
    /* Only OBJECT attributes are allowed in the limits */
    a = findAttribute(attribute->id, objectClass->slot->attributes,
		      objectClass->slot->inheritedAttributeLists);
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

  analyseLimits()

  Analyze a list of LIMITs.

 */
#ifdef _PROTOTYPES_
void analyseLimits(List *limits)
#else
void analyseLimits(limits)
     List *limits;
#endif
{
  List *list, *other;
  

  for (list = limits; list != NULL; list = list->next) {
    /* Analyse the limit */
    analyseLimit(list->element.limit);
    /* Verify that the attributes are not multiply used */
    for (other = list->next; other; other = other->next)
      if (equalIds(list->element.limit->attribute->id,
		   other->element.limit->attribute->id)) {
	lmLog(&other->element.limit->attribute->id->srcp, 219, sevERR,
	      other->element.limit->attribute->id->string);
	break;
      }
  }
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

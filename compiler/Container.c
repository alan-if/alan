/*----------------------------------------------------------------------*\

				Container.c

			      Container Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Container.h"
#include "Limit.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newContainer()

  Allocates and initialises a Container.

 */
#ifdef _PROTOTYPES_
Container *newContainer(Srcp *srcp, /* IN - Source position */
			List *limits, /* IN - The limits */
			List *header, /* IN - Header statements */
			List *empty) /* IN - Statements when empty */
#else
Container *newContainer(srcp, limits, header, empty)
     Srcp *srcp;
     List *limits;
     List *header;
     List *empty;
#endif
{
  Container *new;			/* The newly allocated area */

  new = NEW(Container);

  new->srcp = *srcp;
  new->limits = limits;
  new->header = header;
  new->empty = empty;

  return(new);
}


/*======================================================================

  analyseContainer()

  Analyze a Container.

 */
#ifdef _PROTOTYPES_
void analyseContainer(Container *container, List *attributes, List *inheritedAttributeLists)
#else
void analyseContainer(container, attributes, inheritedAttributeLists)
     Container *container;
     List *attributes;
     List *inheritedAttributeLists;
#endif
{
  /* Analyse container properties */
  analyseLimits(container->limits);
  analyseStatements(container->header, NULL, NULL);
  analyseStatements(container->empty, NULL, NULL);
}



/*======================================================================

  dumpContainer()

  Dump a Container node.

 */
#ifdef _PROTOTYPES_
void dumpContainer(Container *container)
#else
void dumpContainer(container)
     Container *container;
#endif
{
  if (container == NULL) {
    put("NULL");
    return;
  }

  put("CONTAINER: "); dumpSrcp(&container->srcp); in();
  put("limits: "); dumpList(container->limits, LIMIT_NODE); nl();
  put("header: "); dumpList(container->header, STATEMENT_NODE); nl();
  put("empty: "); dumpList(container->empty, STATEMENT_NODE); out();
}

/*----------------------------------------------------------------------*\

				Alternative.c

			      Alternative Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Alternative.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newAlternative()

  Allocates and initialises a Alternative.

 */
#ifdef _PROTOTYPES_
Alternative *newAlternative(Srcp *srcp,	/* IN - Source position */
			    Id *id,	/* IN - Name of parameter */
			    List *checks, /* IN - checks */
			    Does *does) /* IN - Does clause */
#else
Alternative *newAlternative(srcp, id, checks, does)
     Srcp *srcp;
     Id *id;
     List *checks;
     Does *does;
#endif
{
  Alternative *new;			/* The newly allocated area */

  new = NEW(Alternative);

  new->srcp = *srcp;
  new->id = id;
  new->checks = checks;
  new->does = does;

  return(new);
}


/*======================================================================

  analyseAlternative()

  Analyze a ALTERNATIVE.

 */
#ifdef _PROTOTYPES_
void analyseAlternative(Alternative *alternative, List *parameters)
#else
void analyseAlternative(alternative, parameters)
     Alternative *alternative;
     List *parameters;
#endif
{
  /* 4f - Analyse alternative */
}



/*======================================================================

  dumpAlternative()

  Dump a Alternative node.

 */
#ifdef _PROTOTYPES_
void dumpAlternative(Alternative *alternative)
#else
void dumpAlternative(alternative)
     Alternative *alternative;
#endif
{
  if (alternative == NULL) {
    put("NULL");
    return;
  }

  put("ALTERNATIVE: "); dumpSrcp(&alternative->srcp); in();
  put("id: "); dumpId(alternative->id); nl();
  put("checks: "); dumpList(alternative->checks, CHECK_NODE); nl();
  put("does: "); dumpDoes(alternative->does); out();
}



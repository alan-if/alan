/*----------------------------------------------------------------------*\

				Exit.c

			      Exit Nodes

\*----------------------------------------------------------------------*/


#include "Exit.h"

#include "Statement.h"

#include "dump.h"




/*======================================================================

  newExit()

  Allocates and initialises a Exit.

 */
#ifdef _PROTOTYPES_
Exit *newExit(Srcp *srcp,	/* IN - Source Position */
	      List *directions,	/* IN - The directions */
	      Id *to,		/* IN - Where to? */
	      List *checks,	/* IN - List of checks */
	      Does *does)	/* IN - The does clause */
#else
Exit *newExit(srcp, directions, to, checks, does)
     Srcp *srcp;
     List *directions;
     Id *to;
     List *checks;
     Does *does;
#endif
{
  Exit *new;			/* The newly allocated area */

  new = NEW(Exit);

  new->srcp = *srcp;
  new->directions = directions;
  new->to = to;
  new->checks = checks;
  new->does = does;

  /* 4f - look up all directions in a exit and enter them as direction symbols */

  return(new);
}


/*======================================================================

  analyseExit()

  Analyze an Exit.

 */
#ifdef _PROTOTYPES_
void analyseExit(Exit *exit)
#else
void analyseExit(exit)
     Exit *exit;
#endif
{
  /* 4f - analyse exits */
}



/*======================================================================

  dumpExit()

  Dump a Exit node.

 */
#ifdef _PROTOTYPES_
void dumpExit(Exit *exit)
#else
void dumpExit(exit)
     Exit *exit;
#endif
{
  if (exit == NULL) {
    put("NULL");
    return;
  }

  put("EXIT: "); in();
  put("directions: "); dumpList(exit->directions, ID_NODE); nl();
  put("to: "); dumpId(exit->to); nl();
  put("checks: "); dumpList(exit->checks, CHECK_NODE); nl();
  put("does: "); dumpDoes(exit->does); out();
}

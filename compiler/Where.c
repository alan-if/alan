/*----------------------------------------------------------------------*\

				Where.c

			      Where Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Where.h"
#include "List.h"

#include "dump.h"




/*======================================================================

  newWhere()

  Allocates and initialises a Where.

 */
#ifdef _PROTOTYPES_
Where *newWhere(Srcp *srcp,	/* IN - Source position */
	      WhereKind kind,	/* IN - Kind of Where */
	      What *what)	/* IN - What is where? */
#else
Where *newWhere(srcp, kind, what)
     Srcp *srcp;
     WhereKind kind;
     What *what;
#endif
{
  Where *new;			/* The newly allocated area */

  new = NEW(Where);

  new->srcp = *srcp;
  new->kind = kind;
  new->what = what;

  return(new);
}


/*======================================================================

  analyseWhere()

  Analyze a WHERE.

 */
#ifdef _PROTOTYPES_
void analyseWhere(Where *where, List *parameters)
#else
void analyseWhere(where, parameters)
     Where *where;
     List *parameters;
#endif
{
  /* 4f - analyse Where */
}



/*----------------------------------------------------------------------

  dumpWhereKind()

  Dump the name of a Where Kind.

  */
#ifdef _PROTOTYPES_
static void dumpWhereKind(WhereKind kind)
#else
static void dumpWhereKind(kind)
     WhereKind kind;
#endif
{
  switch (kind) {
  case WHERE_DEFAULT: put("DEFAULT"); break;
  case WHERE_HERE: put("HERE"); break;
  case WHERE_NEAR: put("NEAR"); break;
  case WHERE_AT: put("AT"); break;
  case WHERE_IN: put("IN"); break;
  default: put("*** ERROR unknown Where Kind ***");
  }
}



/*======================================================================

  dumpWhere()

  Dump a Where node.

 */
#ifdef _PROTOTYPES_
void dumpWhere(Where *where)
#else
void dumpWhere(where)
     Where *where;
#endif
{
  if (where == NULL) {
    put("NULL");
    return;
  }

  put("WHERE: "); dumpSrcp(&where->srcp); in();
  put("kind: "); dumpWhereKind(where->kind); nl();
  put("what: "); dumpWhat(where->what); out();
}



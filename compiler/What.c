/*----------------------------------------------------------------------*\

				What.c

			      What Nodes

\*----------------------------------------------------------------------*/


#include "types.h"

#include "What.h"
#include "List.h"

#include "dump.h"




/*======================================================================

  newWhat()

  Allocates and initialises a What.

 */
#ifdef _PROTOTYPES_
What *newWhat(Srcp *srcp,	/* IN - Source position */
	      WhatKind kind,	/* IN - Kind of What */
	      Id *id)		/* IN - fora What Id */
#else
What *newWhat(srcp, kind, id)
     Srcp *srcp;
     WhatKind kind;
     Id *id;
#endif
{
  What *new;			/* The newly allocated area */

  new = NEW(What);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;

  return(new);
}


/*======================================================================

  analyseWhat()

  Analyze a WHAT.

 */
#ifdef _PROTOTYPES_
void analyseWhat(What *what, List *parameters)
#else
void analyseWhat(what, parameters)
     What *what;
     List *parameters;
#endif
{
  /* 4f - Analyse What */
}



/*----------------------------------------------------------------------

  dumpWhatKind()

  Dump the name of a What Kind.

  */
#ifdef _PROTOTYPES_
static void dumpWhatKind(WhatKind kind)
#else
static void dumpWhatKind(kind)
     WhatKind kind;
#endif
{
  switch (kind) {
  case WHAT_LOCATION: put("LOCATION"); break;
  case WHAT_ACTOR: put("ACTOR"); break;
  case WHAT_THIS: put("THIS"); break;
  case WHAT_ID: put("ID"); break;
  default: put("*** ERROR unknown What Kind ***");
  }
}



/*======================================================================

  dumpWhat()

  Dump a What node.

 */
#ifdef _PROTOTYPES_
void dumpWhat(What *what)
#else
void dumpWhat(what)
     What *what;
#endif
{
  if (what == NULL) {
    put("NULL");
    return;
  }

  put("WHAT: "); dumpSrcp(&what->srcp); in();
  put("kind: "); dumpWhatKind(what->kind); nl();
  put("id: "); dumpId(what->id); out();
}



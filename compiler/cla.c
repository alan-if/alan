/*----------------------------------------------------------------------*\

				CLA.C
			     Class Nodes

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#include "alan.h"
#include "srcp.h"
#include "cla.h"



/*======================================================================

  newcla()

  Dynamically allocate and initialise a Class Node

  */
#ifdef _PROTOTYPES_
ClaNod *newcla(
	       Srcp srcp,
	       NamNod *nam,
	       List *heritage,
	       List *nams,
	       WhrNod *whr,
	       List *atrs,
	       CntNod *props,
	       List *surr,
	       List *descr,
	       List *mentioned,
	       List *does,
	       List *exits,
	       List *verbs,
	       List *script);
#else
ClaNod *newcla(srcp, nam, heritage, nams, whr, atrs, props, surr, descr, mentioned, does, exits, verbs, script)
     Srcp srcp;			/* IN - Source position */
     NamNod *nam;		/* IN - Name of the class */
     List *heritage;		/* IN - The list of inherited classes */
     List *nams;		/* IN - Name Slot: List of adjectives and a noun */
     WhrNod *whr;		/* IN - Where Slot: Initial location */
     List *atrs;		/* IN - Attributes Slot: */
     CntNod *props;		/* IN - Container Slot: limits, header, empty */
     List *surr;		/* IN - Surroundings Slot: inside description */
     List *descr;		/* IN - Description Slot: outside description */
     List *mentioned;		/* IN - Mentioned Slot: short form */
     List *does;		/* IN - Does Slot: when entered */
     List *exits;		/* IN - Exits Slot: how to exit */
     List *verbs;		/* IN - Verbs Slot: local verbs */
     List *script;		/* IN - Script Slot: what it does */
#endif
{

  ClaNod *new = NEW(ClaNod);

  new->srcp = srcp;
  new->nam = nam;
  new->heritage = heritage;
  new->nams = nams;
  new->whr = whr;
  new->atrs = atrs;
  new->props = props;
  new->surr = surr;
  new->descr = descr;
  new->mentioned = mentioned;
  new->does = does;
  new->exits = exits;
  new->verbs = verbs;
  new->script = script;

  return new;
}

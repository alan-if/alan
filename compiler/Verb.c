/*----------------------------------------------------------------------*\

				Verb.c

			      Verb Nodes

\*----------------------------------------------------------------------*/


#include "Verb.h"
#include "Statement.h"

#include "dump.h"




/*======================================================================

  newVerb()

  Allocates and initialises a Verb.

 */
#ifdef _PROTOTYPES_
Verb *newVerb(Srcp *srcp,	/* IN - Source position */
	      List *verbs,	/* IN - List of verbs to perform action */
	      List *alternatives) /* IN - The alternative bodies */
#else
Verb *newVerb(srcp, verbs, alternatives)
     Srcp *srcp;
     List *verbs;
     List *alternatives;
#endif
{
  Verb *new;			/* The newly allocated area */

  new = NEW(Verb);

  new->srcp = *srcp;
  new->verbs = verbs;
  new->alternatives = alternatives;

  return(new);
}


/*======================================================================

  analyseVerb()

  Analyze a VERB.

 */
#ifdef _PROTOTYPES_
void analyseVerb(Verb *verb, List *parameters)
#else
void analyseVerb(verb, parameters)
     Verb *verb;
     List *parameters;
#endif
{
  /* 4f - analyse verb */
}



/*======================================================================

  dumpVerb()

  Dump a Verb node.

 */
#ifdef _PROTOTYPES_
void dumpVerb(Verb *verb)
#else
void dumpVerb(verb)
     Verb *verb;
#endif
{
  if (verb == NULL) {
    put("NULL");
    return;
  }

  put("VERB: "); dumpSrcp(&verb->srcp); in();
  put("verbs: "); dumpList(verb->verbs, ID_NODE); nl();
  put("alternatives: "); dumpList(verb->alternatives, ALTERNATIVE_NODE); out();
}

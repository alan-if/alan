/*----------------------------------------------------------------------*\

			      CONTEXT.C
			    Context Nodes

\*----------------------------------------------------------------------*/

#include "context_x.h"

#include "util.h"


/*======================================================================

  newContext()

*/
Context *newContext(ContextKind kind)
{
  Context *new = NEW(Context);

  new->kind = kind;

  return new;
}


/*======================================================================

  copyContext()

*/
Context *copyContext(Context *original)
{
  Context *copy = NEW(Context);

  if (original)
    memcpy(copy, original, sizeof(Context));
  return copy;
}


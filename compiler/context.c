/*----------------------------------------------------------------------*\

			      CONTEXT.C
			    Context Nodes

\*----------------------------------------------------------------------*/

#include "context_x.h"
#include "stddef.h"
#include "util.h"


/*======================================================================*/
Context *newContext(ContextKind kind)
{
  Context *new = NEW(Context);

  new->kind = kind;
  new->previous = NULL;

  return new;
}


/*======================================================================*/
Context *pushContext(Context *previous)
{
  Context *new = NEW(Context);

  if (previous)
    memcpy(new, previous, sizeof(Context));
  new->previous = previous;
  return new;
}

/*======================================================================*/
Bool inEntityContext(Context *context)
{
  return (context->instance != NULL || context->class != NULL);
}

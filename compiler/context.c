/*----------------------------------------------------------------------*\

			      CONTEXT.C
			    Context Nodes

\*----------------------------------------------------------------------*/

#include "context_x.h"
#include "string.h"
#include "util.h"
#include "sym_x.h"


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

/*======================================================================*/
Bool inLocationContext(Context *context)
{
  return (context->instance != NULL
	  && inheritsFrom(context->instance->props->id->symbol, locationSymbol))
    || (context->class != NULL
	&& inheritsFrom(context->class->props->id->symbol, locationSymbol));
}

/*======================================================================*/
IdNode *classNameIn(Context *context)
{
  IdNode *classId = NULL;

  switch (context->kind) {
  case INSTANCE_CONTEXT:
    if (context->instance->props->parentId)
      classId = context->instance->props->parentId;
    break;
  case CLASS_CONTEXT:
    classId = context->class->props->id;
    break;
  default:
    syserr("Unexpected context in '%s()'.", __FUNCTION__);
  }

  return(classId);
}





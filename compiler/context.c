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
IdNode *classIdInContext(Context *context)
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
  case VERB_CONTEXT:
    if (context->instance)
      classId = context->instance->props->parentId;
    else if (context->class)
      classId = context->class->props->id;
    else
      syserr("No instance or class in context in '%s()'", __FUNCTION__);
    break;

  default:
    syserr("Unexpected context in '%s()'.", __FUNCTION__);
  }

  return(classId);
}

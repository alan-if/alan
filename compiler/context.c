/*----------------------------------------------------------------------*\

			      CONTEXT.C
			    Context Nodes

\*----------------------------------------------------------------------*/

#include "context_x.h"
#include "string.h"
#include "util.h"
#include "sym_x.h"


/*======================================================================*/
Context *newContext(ContextKind kind, void *item)
{
  Context *new = NEW(Context);

  new->kind = kind;
  new->previous = NULL;

  switch (kind) {
  case INSTANCE_CONTEXT: new->instance = (Instance *)item; break;
  case CLASS_CONTEXT: new->class = (ClaNod *)item; break;
  case VERB_CONTEXT: new->verb = (Symbol *)item; break;
  case EVENT_CONTEXT: new->event = (EvtNod *)item; break;
  case RULE_CONTEXT:
  case NULL_CONTEXT:
    break;
  default:
    SYSERR("Unexpected Context kind");
  }
  return new;
}


/*======================================================================*/
Symbol *symbolOfContext(Context *context) {
  switch (context->kind) {
  case INSTANCE_CONTEXT: return context->instance->props->id->symbol;
  case CLASS_CONTEXT: return context->class->props->id->symbol;
  case VERB_CONTEXT: return symbolOfContext(context->previous);
  case EVENT_CONTEXT:
  case RULE_CONTEXT:
  case NULL_CONTEXT:
    return NULL;
  default:
    SYSERR("Unexpected Context kind");
  }
  return NULL;
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
      SYSERR("No instance or class in context");
    break;

  default:
    SYSERR("Unexpected context");
  }

  return(classId);
}

/*======================================================================*/
Bool thisIsaContainer(Context *context)
{
  Symbol *symbol;

  if (context->instance != NULL)
    symbol = context->instance->props->id->symbol;
  else if (context->class != NULL)
    symbol = context->class->props->id->symbol;
  else
    return FALSE;

  return symbolIsContainer(symbol);
}



#ifndef _CONTEXT_H_
#define _CONTEXT_H_
/*----------------------------------------------------------------------*\

				CONTEXT.H
		     Execution Context Structure

\*----------------------------------------------------------------------*/

/* USE: */
#include "lst.h"
#include "evt.h"
#include "ins.h"


/* TYPES: */

typedef enum {
  NULL_CONTEXT,
  INSTANCE_CONTEXT,
  EVENT_CONTEXT,
  VERB_CONTEXT,
  RULE_CONTEXT
} ContextKind;


typedef struct Context {
  ContextKind kind;
  List *parameters;		/* For VERB_CONTEXT */
  InsNod *instance;
  EvtNod *event;
} Context;


#endif

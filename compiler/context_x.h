#ifndef _CONTEXT_X_H_
#define _CONTEXT_X_H_
/*----------------------------------------------------------------------*\

				CONTEXT_X.H
		     Execution Context Structure Exports

\*----------------------------------------------------------------------*/

#include "context.h"

/* USE: */
#include "id.h"

/* DATA: */

/* FUNCTIONS: */

extern Context *newContext(ContextKind kind, void *item);
extern Context *pushContext(Context *context);
extern Symbol *symbolOfContext(Context *context);
extern Bool inEntityContext(Context *context);
extern Bool inLocationContext(Context *context);
extern IdNode *classIdInContext(Context *context);
extern Bool thisIsaContainer(Context *context);
#endif

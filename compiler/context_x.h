#ifndef _CONTEXT_X_H_
#define _CONTEXT_X_H_
/*----------------------------------------------------------------------*\

				CONTEXT_X.H
		     Execution Context Structure Exports

\*----------------------------------------------------------------------*/

#include "context.h"


/* USE: */

/* DATA: */

/* FUNCTIONS: */

extern Context *newContext(ContextKind kind);
extern Context *pushContext(Context *context);
extern Bool inEntityContext(Context *context);

#endif

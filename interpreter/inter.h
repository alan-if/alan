#ifndef _INTER_H_
#define _INTER_H_
/*----------------------------------------------------------------------*\

  inter.h

  The interpreter of Acode.

\*----------------------------------------------------------------------*/

#include "types.h"
#include "Stack.h"

/* TYPES: */


/* DATA: */

extern Bool stopAtNextLine;
extern int currentLine;
extern int depth;


/* FUNCTIONS: */

extern void setInterpreterStack(Stack stack);
extern void interpret(Aaddr adr);
extern Aword evaluate(Aaddr adr);

#endif

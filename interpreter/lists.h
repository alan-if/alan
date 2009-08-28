/*----------------------------------------------------------------------

  lists.h

  Various utility functions for handling lists and arrays

  ----------------------------------------------------------------------*/
#ifndef LISTS_H

#include "acode.h"
#include "types.h"

#define isEndOfList(x) implementationOfIsEndOfList((Aword *) (x))
extern Bool implementationOfIsEndOfList(Aword *adr);
#define setEndOfList(x) implementationOfSetEndOfList((Aword *) (x))
extern void implementationOfSetEndOfList(Aword *adr);
#define clearList(x) implementationOfSetEndOfList((Aword *) &(x[0]))
extern void implementationOfSetEndOfList(Aword *adr);

#endif

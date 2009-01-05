/*----------------------------------------------------------------------*\

  params.h

  Various utility functions for handling parameters

\*----------------------------------------------------------------------*/
#ifndef PARAMS_H
#include "types.h"

extern Parameter *allocateParameterArray(Parameter *currentList);
extern Parameter *findEndOfList(Parameter *parameters);
extern int findMultiplePosition(Parameter parameters[]);
extern void compress(Parameter *a);
extern int listLength(Parameter *a);
extern Bool inList(Parameter *l, Aword e);
extern void copyParameterList(Parameter *to, Parameter *from);
extern void subtractListFromList(Parameter *a, Parameter *b);
extern void mergeLists(Parameter *a, Parameter *b);
extern void intersect(Parameter *a, Parameter *b);
extern void copyReferences(Parameter *p, Aword *r);
#endif

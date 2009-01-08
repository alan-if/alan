#ifndef PARAMS_H
#define PARAMS_H
/*----------------------------------------------------------------------*\

  params.h

  Various utility functions for handling parameters

\*----------------------------------------------------------------------*/

/* IMPORTS */
#include "types.h"


/* DATA */
extern Parameter *parameters;   /* List of parameters */


/* FUNCTIONS */
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

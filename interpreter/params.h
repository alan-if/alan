#ifndef PARAMS_H
#define PARAMS_H
/*----------------------------------------------------------------------*\

  params.h

  Various utility functions for handling parameters

  \*----------------------------------------------------------------------*/

/* IMPORTS */
#include "types.h"
#include "acode.h"


/* TYPES */
typedef struct ParamEntry { /* PARAMETER */
    Aint instance;  /* Instance code for the parameter (0=multiple) */
    Abool useWords; /* Indicate to use words instead of instance code when saying */
    Aint firstWord; /* Index to first word used by player */
    Aint lastWord;  /* d:o to last */
    struct ParamEntry *candidates; /* Array of instances possibly filling this parameter position */
} Parameter;


/* DATA */
extern Parameter *parameters;   /* List of parameters */


/* FUNCTIONS */
extern Parameter *allocateParameterArray(Parameter *currentList, int size);
extern Parameter *findEndOfList(Parameter *parameters);
extern int findMultiplePosition(Parameter parameters[]);
extern void compress(Parameter *a);
extern int listLength(Parameter *a);
extern Bool inList(Parameter *l, Aword e);
extern void copyParameterList(Parameter *to, Parameter *from);
extern void subtractListFromList(Parameter *a, Parameter *b);
extern void mergeLists(Parameter *a, Parameter *b);
extern void intersect(Parameter *a, Parameter *b);
extern void copyReferences(Parameter *parameters, Aint *references);
extern void addParameterForInteger(Parameter *parameters, int value);
extern void addParameterForString(Parameter *parameters, char *value);

#endif

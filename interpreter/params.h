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
    int instance;   /* Instance code for the parameter (0=multiple) */
    Bool useWords; /* Indicate to use words instead of instance code when saying */
    int firstWord; /* Index to first word used by player */
    int lastWord;  /* d:o to last */
    struct ParamEntry *candidates; /* Array of instances possibly matching this parameter depending on player input */
} Parameter;


/* DATA */
extern Parameter *globalParameters;


/* FUNCTIONS */
extern void setParameters(Parameter parameters[]);
extern void setParameter(int parameterIndex, Parameter parameter);
extern Parameter *getParameters(void);
extern Parameter *getParameter(int parameterIndex);
extern Parameter *allocateParameterArray(Parameter *currentList, int size);
extern Parameter *findEndOfParameterList(Parameter *parameters);
extern int findMultiplePosition(Parameter parameters[]);
extern void compressParameterList(Parameter *a);
extern int lengthOfParameterList(Parameter *a);
extern Bool inParameterList(Parameter *l, Aword e);
extern void copyParameterList(Parameter *to, Parameter *from);
extern void clearParameterList(Parameter *list);
extern void subtractListFromParameterList(Parameter *a, Parameter *b);
extern void mergeParameterLists(Parameter *a, Parameter *b);
extern void intersectParameterLists(Parameter *a, Parameter *b);
extern void copyReferencesToParameterList(Aint *references, Parameter *parameters);
extern void addParameterForInstance(Parameter *parameter, int instance);
extern void addParameterForInteger(Parameter *parameters, int value);
extern void addParameterForString(Parameter *parameters, char *value);

#endif

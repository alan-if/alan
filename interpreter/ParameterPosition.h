#ifndef PARAMETERPOSITION_H_
#define PARAMETERPOSITION_H_
/*----------------------------------------------------------------------*\

  ParameterPosition

\*----------------------------------------------------------------------*/

/* Imports: */
#include "acode.h"
#include "types.h"
#include "params.h"


/* Constants: */


/* Types: */
typedef struct ParameterPosition {
    Bool endOfList;
    Bool explicitMultiple;
    Bool all;
    Bool checked;
    Parameter *parameters;
    Parameter *exceptions;
} ParameterPosition;


/* Data: */
ParameterPosition *parameterPositions;


/* Functions: */
extern void uncheckAllParameterPositions(ParameterPosition parameterPositions[]);
extern void copyParameterPositions(ParameterPosition originalParameterPositions[], ParameterPosition parameterPositions[]);
extern int findMultipleParameterPosition(ParameterPosition parameterPositions[]);
extern void markExplicitMultiple(ParameterPosition parameterPositions[], Parameter parameters[]);
extern void convertPositionsToParameters(ParameterPosition parameterPositions[], Parameter parameters[]);

#endif /* PARAMETERPOSITION_H_ */

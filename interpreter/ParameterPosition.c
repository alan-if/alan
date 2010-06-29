#include "parameterPosition.h"

/* Imports */
#include "memory.h"
#include "lists.h"


/* Public Data */


/* Methods */


/*======================================================================*/
void uncheckAllParameterPositions(ParameterPosition parameterPositions[]) {
    int position;
    for (position = 0; position < MAXPARAMS; position++) {
        parameterPositions[position].checked = FALSE;
    }
}


/*======================================================================*/
void copyParameterPositions(ParameterPosition originalParameterPositions[], ParameterPosition parameterPositions[]) {
    Aint parameterNumber;
    for (parameterNumber = 0; !originalParameterPositions[parameterNumber].endOfList; parameterNumber++)
        parameterPositions[parameterNumber] = originalParameterPositions[parameterNumber];
    parameterPositions[parameterNumber].endOfList = TRUE;
}


/*======================================================================*/
int findMultipleParameterPosition(ParameterPosition parameterPositions[]) {
    Aint parameterNumber;
    for (parameterNumber = 0; !parameterPositions[parameterNumber].endOfList; parameterNumber++)
        if (parameterPositions[parameterNumber].explicitMultiple)
            return parameterNumber;
    return -1;
}


/*======================================================================*/
void markExplicitMultiple(ParameterPosition parameterPositions[], Parameter parameters[]) {
    int parameterCount;
    for (parameterCount = 0; !parameterPositions[parameterCount].endOfList; parameterCount++)
        if (parameterPositions[parameterCount].explicitMultiple)
            parameters[parameterCount].instance = 0;
}

/*======================================================================*/
void convertPositionsToParameters(ParameterPosition parameterPositions[], Parameter parameters[]) {
    int parameterCount;
    for (parameterCount = 0; !parameterPositions[parameterCount].endOfList; parameterCount++)
	parameters[parameterCount] = parameterPositions[parameterCount].parameters[0];
    setEndOfList(&parameters[parameterCount]);
}

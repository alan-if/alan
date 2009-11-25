#include "parameterPosition.h"

/* Imports */
#include "memory.h"

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

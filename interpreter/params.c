/*----------------------------------------------------------------------

  params.c

  Various utility functions for handling parameters

  ----------------------------------------------------------------------*/
#include "params.h"

/* IMPORTS */
#include "lists.h"
#include "memory.h"
#include "literal.h"
#include "syserr.h"

/* PUBLIC DATA */
Parameter *globalParameters = NULL;



/*======================================================================*/
Parameter *newParameter(int id) {
    Parameter *parameter = NEW(Parameter);
    parameter->instance = id;
    parameter->candidates = NULL;

    return parameter;
}


/*======================================================================*/
Parameter *newParameterArray(void) {
    Parameter *newArray = allocate((MAXINSTANCE+1)*sizeof(Parameter));
    setEndOfArray(newArray);
    return newArray;
}


/*======================================================================*/
void freeParameterArray(ParameterArray arrayPointer) {
    Parameter *parameter = arrayPointer;

    while(!isEndOfArray(parameter)) {
        if (parameter->candidates != NULL)
            freeParameterArray(parameter->candidates);
        parameter++;
    }
    free(arrayPointer);
}


/*======================================================================*/
Parameter *ensureParameterArrayAllocated(ParameterArray currentArray) {
    if (currentArray == NULL)
        currentArray = newParameterArray();
    else
	clearParameterArray(currentArray);
    return currentArray;
}


/*======================================================================*/
Parameter *allocateParameterArray(int n) {
    Parameter *newArray = allocate((n+1)*sizeof(Parameter));
    setEndOfArray(newArray);
    return newArray;
}


/*======================================================================*/
bool parameterArrayIsEmpty(ParameterArray parameters) {
    return parameters == NULL || lengthOfParameterArray(parameters) == 0;
}


/*======================================================================*/
void clearParameter(Parameter *parameter, Parameter *candidates) {
    memset(parameter, 0, sizeof(Parameter));
    parameter->candidates = candidates;
    if (parameter->candidates != NULL)
	setEndOfArray(&parameter->candidates[0]);
}


/*======================================================================*/
void setGlobalParameters(ParameterArray newParameters) {
    if (globalParameters == NULL)
        globalParameters = newParameterArray();
    copyParameterArray(globalParameters, newParameters);
}


/*======================================================================*/
Parameter *getGlobalParameters(void) {
    if (globalParameters == NULL)
        globalParameters = allocateParameterArray(MAXINSTANCE);
    return globalParameters;
}


/*======================================================================*/
Parameter *getGlobalParameter(int parameterIndex) {
    return &globalParameters[parameterIndex];
}


/*======================================================================*/
Parameter *findEndOfParameterArray(Parameter *parameters) {
    Parameter *parameter;
    for (parameter = parameters; !isEndOfArray(parameter); parameter++);
    return parameter;
}


/*======================================================================*/
/* A parameter position with code == 0 means this is a multiple position.
 * We must loop over this position (and replace it by each present in the
 * matched list)
 */
int findMultiplePosition(Parameter parameters[]) {
    // TODO: this should look at the isAll and isExplicitMultiple flags instead
    int multiplePosition;
    for (multiplePosition = 0; !isEndOfArray(&parameters[multiplePosition]); multiplePosition++)
	if (parameters[multiplePosition].instance == 0)
	    return multiplePosition;
    return -1;
}


/*======================================================================*/
void compressParameterArray(Parameter theArray[])
{
    int i, j;

    for (i = 0, j = 0; theArray[j].instance != EOF; j++)
		if (theArray[j].instance != 0)
			theArray[i++] = theArray[j];
	// TODO Use addParameter()
    setEndOfArray(&theArray[i]);
}


/*======================================================================*/
int lengthOfParameterArray(Parameter theArray[])
{
    int i = 0;

    if (theArray == NULL) return 0;

    while (!isEndOfArray(&theArray[i]))
        i++;
    return i;
}


/*======================================================================*/
bool equalParameterArrays(Parameter parameters1[], Parameter parameters2[])
{
    int i;

    if ((parameters1 == NULL) != (parameters2 == NULL))
        return FALSE;
    if (parameters1 == NULL) // Because then parameter2 is also NULL
        return TRUE;
    for (i = 0; i < lengthOfParameterArray(parameters1); i++) {
        if (isEndOfArray(&parameters2[i])) return FALSE;
        if (parameters1[i].instance != parameters2[i].instance) return FALSE;
    }
    return isEndOfArray(&parameters2[i]);
}


/*======================================================================*/
bool inParameterArray(Parameter theArray[], Aword theCode)
{
    int i;

    for (i = 0; !isEndOfArray(&theArray[i]) && theArray[i].instance != theCode; i++);
    return (theArray[i].instance == theCode);
}


/*======================================================================*/
void copyParameter(Parameter *to, Parameter *from) {
    Parameter *theCopyCandidates = to->candidates;
    *to = *from;
    if (lengthOfParameterArray(theCopyCandidates) < lengthOfParameterArray(from->candidates))
		// TODO Should we free the from->candidates here
		to->candidates = allocateParameterArray(MAXINSTANCE);
    copyParameterArray(to->candidates, from->candidates);
}


/*======================================================================*/
void addParameter(Parameter theArrayPosition[], Parameter *theParameter)
{
    if (theArrayPosition == NULL) syserr("Adding to null parameter array");

	if (isEndOfArray(&theArrayPosition[0]))
		clearParameter(&theArrayPosition[0], NULL);
	copyParameter(&theArrayPosition[0], theParameter);
    setEndOfArray(&theArrayPosition[1]);
}


/*======================================================================*/
void addParameterToParameterArray(ParameterArray theArray, Parameter *theParameter)
{
    if (theArray == NULL) syserr("Adding to null parameter array");

    int i;

    for (i = 0; !isEndOfArray(&theArray[i]) && i > MAXINSTANCE; i++)
	;
    if (isEndOfArray(&theArray[i])) {
	clearParameter(&theArray[i], NULL);
	copyParameter(&theArray[i], theParameter);
	setEndOfArray(&theArray[i+1]);
    } else
	syserr("Couldn't find end of ParameterArray");
}


/*======================================================================*/
void copyParameterArray(ParameterArray to, ParameterArray from)
{
    int i;

    if (to == NULL && from == NULL) return;

    if (to == NULL)
	syserr("Copying to null parameter array");
    else {
	setEndOfArray(&to[0]);
	for (i = 0; !isEndOfArray(&from[i]); i++)
	    addParameter(&to[i], &from[i]);
    }
}


/*======================================================================*/
void subtractParameterArrays(Parameter theArray[], Parameter remove[])
{
    int i;
    
    if (remove == NULL) return;

    for (i = 0; !isEndOfArray(&theArray[i]); i++)
        if (inParameterArray(remove, theArray[i].instance))
            theArray[i].instance = 0;		/* Mark empty */
    compressParameterArray(theArray);
}


/*======================================================================*/
void clearParameterArray(Parameter theArray[]) {
    clearParameter(&theArray[0], theArray[0].candidates);
    setEndOfArray(theArray);
}


/*======================================================================*/
void intersectParameterArrays(Parameter one[], Parameter other[])
{
    int i, last = 0;


    for (i = 0; !isEndOfArray(&one[i]); i++)
		if (inParameterArray(other, one[i].instance))
			one[last++] = one[i];
    setEndOfArray(&one[last]);
}


/*======================================================================*/
void copyReferencesToParameterArray(Aint references[], Parameter parameterArray[])
{
    int i;

    for (i = 0; !isEndOfArray(&references[i]); i++) {
        parameterArray[i].instance = references[i];
        parameterArray[i].firstWord = EOF; /* Ensure that there is no word that can be used */
    }
    setEndOfArray(&parameterArray[i]);
}


/*======================================================================*/
void addParameterForInstance(Parameter *parameters, int instance) {
    Parameter *parameter = findEndOfParameterArray(parameters);

    parameter->instance = instance;
    parameter->useWords = FALSE;

    setEndOfArray(parameter+1);
}


/*======================================================================*/
void addParameterForInteger(ParameterArray parameters, int value) {
    Parameter *parameter = findEndOfParameterArray(parameters);

    createIntegerLiteral(value);
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = FALSE;

    setEndOfArray(parameter+1);
}

/*======================================================================*/
void addParameterForString(Parameter *parameters, char *value) {
    Parameter *parameter = findEndOfParameterArray(parameters);

    createStringLiteral(value);
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = FALSE;

    setEndOfArray(parameter+1);
}

/*======================================================================*/
void printParameterArray(Parameter parameters[]) {
    int i;
    printf("[");
    for (i = 0; !isEndOfArray(&parameters[i]); i++) {
	printf("%d ", (int)parameters[i].instance);
    }
    printf("]\n");
}


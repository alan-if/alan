/*----------------------------------------------------------------------

  params.c

  Various utility functions for handling parameters

  ----------------------------------------------------------------------*/
#include "params.h"

/* IMPORTS */
#include "lists.h"
#include "memory.h"
#include "literal.h"
#include "ParameterPosition.h"


/* PUBLIC DATA */
Parameter *globalParameters = NULL;



/*======================================================================*/
void setParameters(Parameter *newParameters) {
    int i;

    if (globalParameters == NULL)
        globalParameters = allocateParameterArray(MAXPARAMS);
    for (i = 0; i < MAXPARAMS; i++)
        globalParameters[i] = newParameters[i];
}


/*======================================================================*/
Parameter *getParameters(void) {
    if (globalParameters == NULL)
        globalParameters = allocateParameterArray(MAXPARAMS);
    return globalParameters;
}


/*======================================================================*/
void setParameter(int parameterIndex, Parameter parameter) {
    globalParameters[parameterIndex] = parameter;
}


/*======================================================================*/
Parameter *getParameter(int parameterIndex) {
    return &globalParameters[parameterIndex];
}


/*======================================================================*/
Parameter *ensureParameterArrayAllocated(Parameter *currentList, int size) {
    if (currentList == NULL)
        currentList = allocate(sizeof(Parameter)*(size+1));
    clearParameterList(currentList);
    return currentList;
}


/*======================================================================*/
Parameter *allocateParameterArray(int size) {
    Parameter *newList = allocate(sizeof(Parameter)*(size+1));
    clearParameterList(newList);
    return newList;
}


/*======================================================================*/
Parameter *findEndOfParameterList(Parameter *parameters) {
    Parameter *parameter;
    for (parameter = parameters; !isEndOfList(parameter); parameter++);
    return parameter;
}


/*======================================================================*/
/* A parameter position with code == 0 means this is a multiple position.
 * We must loop over this position (and replace it by each present in the
 * matched list)
 */
int findMultiplePosition(Parameter parameters[]) {
    int multiplePosition;
    for (multiplePosition = 0; !isEndOfList(&parameters[multiplePosition]); multiplePosition++)
	if (parameters[multiplePosition].instance == 0)
	    return multiplePosition;
    return -1;
}


/*======================================================================*/
void compressParameterList(Parameter theList[])
{
    int i, j;

    for (i = 0, j = 0; theList[j].instance != EOF; j++)
	if (theList[j].instance != 0)
	    theList[i++] = theList[j];
    setEndOfList(&theList[i]);
}


/*======================================================================*/
int lengthOfParameterList(Parameter theList[])
{
    int i = 0;

    while (!isEndOfList(&theList[i]))
        i++;
    return i;
}


/*======================================================================*/
Bool inParameterList(Parameter theList[], Aword theCode)
{
    int i;

    for (i = 0; !isEndOfList(&theList[i]) && theList[i].instance != theCode; i++);
    return (theList[i].instance == theCode);
}


/*======================================================================*/
void copyParameterList(Parameter to[], Parameter from[])
{
    int i;

    for (i = 0; !isEndOfList(&from[i]); i++)
	to[i] = from[i];
    setEndOfList(&to[i]);
}


/*======================================================================*/
void subtractListFromParameterList(Parameter theList[], Parameter remove[])
{
    int i;

    for (i = 0; !isEndOfList(&theList[i]); i++)
	if (inParameterList(remove, theList[i].instance))
	    theList[i].instance = 0;		/* Mark empty */
    compressParameterList(theList);
}


/*======================================================================*/
void mergeParameterLists(Parameter one[], Parameter other[])
{
    int i,last;

    for (last = 0; !isEndOfList(&one[last]); last++); /* Find end of list */
    for (i = 0; !isEndOfList(&other[i]); i++)
	if (!inParameterList(one, other[i].instance)) {
	    one[last++] = other[i];
	    setEndOfList(&one[last]);
	}
}


/*======================================================================*/
void clearParameterList(Parameter list[]) {
    implementationOfSetEndOfList((Aword *) &(list[0]));
}


/*======================================================================*/
void intersectParameterLists(Parameter one[], Parameter other[])
{
    int i, last = 0;

    for (i = 0; !isEndOfList(&one[i]); i++)
	if (inParameterList(other, one[i].instance))
	    one[last++] = one[i];
    setEndOfList(&one[last]);
}


/*======================================================================*/
void copyReferencesToParameterList(Aint references[], Parameter parameterList[])
{
    int i;

    for (i = 0; !isEndOfList(&references[i]); i++) {
        parameterList[i].instance = references[i];
        parameterList[i].firstWord = EOF; /* Ensure that there is no word that can be used */
    }
    setEndOfList(&parameterList[i]);
}


/*======================================================================*/
void addParameterForInstance(Parameter *parameters, int instance) {
    Parameter *parameter = findEndOfParameterList(parameters);

    parameter->instance = instance;
    parameter->useWords = FALSE;

    setEndOfList(parameter+1);
}


/*======================================================================*/
void addParameterForInteger(Parameter *parameters, int value) {
    Parameter *parameter = findEndOfParameterList(parameters);

    createIntegerLiteral(value);
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = FALSE;

    setEndOfList(parameter+1);
}

/*======================================================================*/
void addParameterForString(Parameter *parameters, char *value) {
    Parameter *parameter = findEndOfParameterList(parameters);

    createStringLiteral(value);
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = FALSE;

    setEndOfList(parameter+1);
}

/*======================================================================*/
void printParameterArray(Parameter parameters[]) {
    int i;
    printf("[");
    for (i = 0; !isEndOfList(&parameters[i]); i++) {
	printf("%d ", parameters[i].instance);
    }
    printf("]\n");
}


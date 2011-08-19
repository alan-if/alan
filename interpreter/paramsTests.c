#include "cgreen/cgreen.h"

#include "params.c"

static void setUp(void) {
	header->maxParameters = 4;
	header->instanceMax = 6;
}


/*----------------------------------------------------------------------*/
Ensure canFindLastParameterInAList(void) {
    Parameter parameters[10];

    memset(parameters, 45, sizeof(parameters));
    setEndOfArray(&parameters[5]);
    assert_equal(&parameters[5], findEndOfParameterArray(parameters));
}


/*----------------------------------------------------------------------*/
Ensure canSetAndGetParameters(void) {
    int numberOfParameters = 4;
    Parameter *parameters;
    int p;

    header->instanceMax = 10;
    parameters = allocateParameterArray(5);
    
    parameters[0].instance = 0;	/* Not end of parameters... */
    setEndOfArray(&parameters[numberOfParameters]);
    assert_equal(lengthOfParameterArray(parameters), numberOfParameters);

    for (p = 0; p<numberOfParameters; p++)
        parameters[p].instance = p;
    
    setParameters(parameters);

    assert_equal(lengthOfParameterArray(getParameters()), lengthOfParameterArray(parameters));
    
    for (p = 0; !isEndOfArray(&parameters[p]); p++)
        assert_equal(getParameter(p)->instance, p);
}


/*----------------------------------------------------------------------*/
Ensure getWillAllocateStoredParameters(void) {
    assert_not_equal(getParameters(), NULL);
}


/*----------------------------------------------------------------------*/
Ensure can_find_multiple_position(void) {
    Parameter parameters[10];
    int i;

    for (i=0; i<10; i++)
        parameters[i].instance = 2;
    parameters[7].instance = 0;
    setEndOfArray(&parameters[9]);

    assert_equal(findMultiplePosition(parameters), 7);
}


/*----------------------------------------------------------------------*/
Ensure returns_minus_one_for_no_multiple_position(void) {
    Parameter parameters[10];
    int i;

    for (i=0; i<10; i++)
        parameters[i].instance = 2;
    setEndOfArray(&parameters[9]);

    assert_equal(findMultiplePosition(parameters), -1);
}


/*----------------------------------------------------------------------*/
static Parameter *givenAnyParameterArrayOfLength(int length) {
    Parameter *parameters = allocateParameterArray(length);
    parameters->instance = 1;
    setEndOfArray(&parameters[length]);
    return parameters;
}


/*----------------------------------------------------------------------*/
Ensure unequal_length_parameter_arrays_are_not_equal(void) {
    Parameter *parameters1 = givenAnyParameterArrayOfLength(4);
    Parameter *parameters2 = givenAnyParameterArrayOfLength(5);

    assert_false(equalParameterArrays(parameters1, parameters2));
}

/*----------------------------------------------------------------------*/
Ensure subtractParameterArraysCanSubtractNullArray(void) {
    Parameter *parameters1 = givenAnyParameterArrayOfLength(4);
    Parameter *parameters2 = NULL;

    subtractParameterArrays(parameters1, parameters2);
    assert_equal(4, lengthOfParameterArray(parameters1));
}

/*----------------------------------------------------------------------*/
Ensure lengthOfParameterArrayReturnsZeroForNULLArray(void) {
    assert_equal(0, lengthOfParameterArray(NULL));
}

/*----------------------------------------------------------------------*/
Ensure copyParameterCopiesCandidates(void) {
    Parameter theOriginal;
    Parameter theCopy;

    clearParameter(&theOriginal, NULL);
    clearParameter(&theCopy, NULL);

    theOriginal.instance = 2;
    theOriginal.candidates = allocate(4*sizeof(Parameter));
    setEndOfArray(&theOriginal.candidates[3]);

    copyParameter(&theCopy, &theOriginal);

    assert_equal(theOriginal.instance, theCopy.instance);
    assert_true(equalParameterArrays(theOriginal.candidates, theCopy.candidates));
    assert_not_equal(theOriginal.candidates, theCopy.candidates);
}


/*----------------------------------------------------------------------*/
Ensure copyParameterArrayCanCopyNullToNull(void) {
    copyParameterArray(NULL, NULL);
}


/*----------------------------------------------------------------------*/
static Parameter *newParameter(int id) {
	Parameter *parameter = NEW(Parameter);
	parameter->instance = id;
	parameter->candidates = NULL;

	return parameter;
}


/*----------------------------------------------------------------------*/
Ensure addParameterSetsEndOfArray(void) {
	Parameter *parameters = allocateParameterArray(5);
	Parameter *parameter = newParameter(1);
	
	setEndOfArray(&parameters[0]);
	assert_true(lengthOfParameterArray(parameters) == 0);
	addParameter(&parameters[0], parameter);
	assert_true(lengthOfParameterArray(parameters) == 1);
	addParameter(&parameters[1], parameter);
	assert_true(lengthOfParameterArray(parameters) == 2);
}


/*----------------------------------------------------------------------*/
Ensure intersectParameterArraysReturnsAnEmptyResultForTwoEmpty(void) {
	Parameter *first = allocateParameterArray(5);
	Parameter *second = allocateParameterArray(5);

	intersectParameterArrays(first, second);

	assert_true(lengthOfParameterArray(first) == 0);
}


static Parameter *givenAParameterArrayWithOneParameter(Parameter *theParameter) {
	Parameter *theArray = allocateParameterArray(5);
	addParameter(theArray, theParameter);
	return theArray;
}

static Parameter *givenAParameterArrayWithTwoParameters(Parameter *theFirstParameter, Parameter *theSecondParameter) {
	Parameter *theArray = allocateParameterArray(5);
	addParameter(theArray, theFirstParameter);
	addParameter(&theArray[1], theSecondParameter);
	return theArray;
}


/*----------------------------------------------------------------------*/
Ensure intersectParameterArraysReturnsAnEmptyIfEitherIsEmpty(void) {
	Parameter *theParameter = newParameter(1);
	Parameter *oneParameterArray = givenAParameterArrayWithOneParameter(theParameter);
	Parameter *emptyParameterArray = allocateParameterArray(5);

	intersectParameterArrays(oneParameterArray, emptyParameterArray);

	assert_true(lengthOfParameterArray(oneParameterArray) == 0);
}


/*----------------------------------------------------------------------*/
Ensure intersectParameterArraysReturnsTheSameIfBothAreEqual(void) {
	Parameter *theParameter = newParameter(1);
	Parameter *first = givenAParameterArrayWithOneParameter(theParameter);
	Parameter *second = givenAParameterArrayWithOneParameter(theParameter);

	intersectParameterArrays(first, second);

	assert_true(equalParameterArrays(first, second));
}


/*----------------------------------------------------------------------*/
Ensure intersectParameterArraysReturnsTheCommonParameter(void) {
	Parameter *aParameter = newParameter(1);
	Parameter *anotherParameter = newParameter(2);
	Parameter *aThirdParameter = newParameter(3);
	Parameter *first = givenAParameterArrayWithTwoParameters(aParameter, anotherParameter);
	Parameter *second = givenAParameterArrayWithTwoParameters(anotherParameter, aThirdParameter);

	intersectParameterArrays(first, second);

	assert_equal(lengthOfParameterArray(first), 1);
}


/*======================================================================*/
TestSuite *paramsTests(void)
{
    TestSuite *suite = create_test_suite();

	setup(suite, setUp);
    add_test(suite, canFindLastParameterInAList);
    add_test(suite, canSetAndGetParameters);
    add_test(suite, getWillAllocateStoredParameters);
    add_test(suite, can_find_multiple_position);
    add_test(suite, returns_minus_one_for_no_multiple_position);
    add_test(suite, unequal_length_parameter_arrays_are_not_equal);
    add_test(suite, subtractParameterArraysCanSubtractNullArray);
    add_test(suite, lengthOfParameterArrayReturnsZeroForNULLArray);
    add_test(suite, copyParameterCopiesCandidates);
    add_test(suite, copyParameterArrayCanCopyNullToNull);
	add_test(suite, addParameterSetsEndOfArray);
	add_test(suite, intersectParameterArraysReturnsAnEmptyResultForTwoEmpty);
	add_test(suite, intersectParameterArraysReturnsAnEmptyIfEitherIsEmpty);
	add_test(suite, intersectParameterArraysReturnsTheSameIfBothAreEqual);
	add_test(suite, intersectParameterArraysReturnsTheCommonParameter);
    return suite;
}

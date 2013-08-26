#include "cgreen/cgreen.h"

#include "params.c"
#include <malloc.h>

Describe(ParameterArray);
BeforeEach(ParameterArray) {
	header->maxParameters = 4;
	header->instanceMax = 6;
}
AfterEach(ParameterArray) {}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, canFindLastParameterInAList) {
    Parameter parameters[10];

    memset(parameters, 45, sizeof(parameters));
    setEndOfArray(&parameters[5]);
    assert_equal(&parameters[5], findEndOfParameterArray(parameters));
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, canSetAndGetParameters) {
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
    
    setGlobalParameters(parameters);

    assert_equal(lengthOfParameterArray(getGlobalParameters()), lengthOfParameterArray(parameters));
    
    for (p = 0; !isEndOfArray(&parameters[p]); p++)
        assert_equal(getGlobalParameter(p)->instance, p);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, getWillAllocateStoredParameters) {
    assert_not_equal(getGlobalParameters(), NULL);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, can_find_multiple_position) {
    Parameter parameters[10];
    int i;

    for (i=0; i<10; i++)
        parameters[i].instance = 2;
    parameters[7].instance = 0;
    setEndOfArray(&parameters[9]);

    assert_equal(findMultiplePosition(parameters), 7);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, returns_minus_one_for_no_multiple_position) {
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
Ensure(ParameterArray, unequal_length_parameter_arrays_are_not_equal) {
    Parameter *parameters1 = givenAnyParameterArrayOfLength(4);
    Parameter *parameters2 = givenAnyParameterArrayOfLength(5);

    assert_false(equalParameterArrays(parameters1, parameters2));
}

/*----------------------------------------------------------------------*/
Ensure(ParameterArray, subtractParameterArraysCanSubtractNullArray) {
    Parameter *parameters1 = givenAnyParameterArrayOfLength(4);
    Parameter *parameters2 = NULL;

    subtractParameterArrays(parameters1, parameters2);
    assert_equal(4, lengthOfParameterArray(parameters1));
}

/*----------------------------------------------------------------------*/
Ensure(ParameterArray, lengthOfParameterArrayReturnsZeroForNULLArray) {
    assert_equal(0, lengthOfParameterArray(NULL));
}

/*----------------------------------------------------------------------*/
Ensure(ParameterArray, copyParameterCopiesCandidates) {
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
Ensure(ParameterArray, canCopyNullToNull) {
    copyParameterArray(NULL, NULL);
}

static bool syserrCalled = FALSE;
static void syserrHandler(char *message) {
    syserrCalled = TRUE;
}

/*----------------------------------------------------------------------*/
Ensure(ParameterArray, bailsOutOnCopyToNull) {
    ParameterArray theCopy = newParameterArray();

    setSyserrHandler(syserrHandler);
    copyParameterArray(NULL, theCopy);
    assert_that(syserrCalled);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, canCopyArrayToEmpty) {
    ParameterArray theOriginal = newParameterArray();
    ParameterArray theCopy = newParameterArray();
    Parameter *aParameter = newParameter(12);

    addParameterToParameterArray(theOriginal, aParameter);

    assert_that(lengthOfParameterArray(theCopy), is_equal_to(0));
    copyParameterArray(theCopy, theOriginal);
    assert_that(lengthOfParameterArray(theCopy), is_equal_to(lengthOfParameterArray(theOriginal)));
    assert_that(theCopy[0].instance, is_equal_to(aParameter->instance));
}



/*----------------------------------------------------------------------*/
Ensure(ParameterArray, addParameterSetsEndOfArray) {
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
Ensure(ParameterArray, intersectParameterArraysReturnsAnEmptyResultForTwoEmpty) {
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
Ensure(ParameterArray, intersectParameterArraysReturnsAnEmptyIfEitherIsEmpty) {
	Parameter *theParameter = newParameter(1);
	Parameter *oneParameterArray = givenAParameterArrayWithOneParameter(theParameter);
	Parameter *emptyParameterArray = allocateParameterArray(5);

	intersectParameterArrays(oneParameterArray, emptyParameterArray);

	assert_true(lengthOfParameterArray(oneParameterArray) == 0);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, intersectParameterArraysReturnsTheSameIfBothAreEqual) {
	Parameter *theParameter = newParameter(1);
	Parameter *first = givenAParameterArrayWithOneParameter(theParameter);
	Parameter *second = givenAParameterArrayWithOneParameter(theParameter);

	intersectParameterArrays(first, second);

	assert_true(equalParameterArrays(first, second));
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, intersectParameterArraysReturnsTheCommonParameter) {
	Parameter *aParameter = newParameter(1);
	Parameter *anotherParameter = newParameter(2);
	Parameter *aThirdParameter = newParameter(3);
	Parameter *first = givenAParameterArrayWithTwoParameters(aParameter, anotherParameter);
	Parameter *second = givenAParameterArrayWithTwoParameters(anotherParameter, aThirdParameter);

	intersectParameterArrays(first, second);

	assert_equal(lengthOfParameterArray(first), 1);
}


/*----------------------------------------------------------------------*/
Ensure(ParameterArray, freesSubordinateParameterArrays) {
    struct mallinfo mallocinfo;
    size_t used = mallinfo().uordblks;

    Parameter *parameterArray = newParameterArray();
    Parameter *parameter = newParameter(7);
    parameter->candidates = newParameterArray();
    addParameterToParameterArray(parameterArray, parameter);
    free(parameter);

    freeParameterArray(parameterArray);

    mallocinfo = mallinfo();
    assert_that(mallocinfo.uordblks, is_equal_to(used));
}

/*======================================================================*/
TestSuite *paramsTests(void) {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, ParameterArray, bailsOutOnCopyToNull);

    return suite;
}

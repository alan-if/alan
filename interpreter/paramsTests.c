#include "cgreen/cgreen.h"

#include "params.c"


/*----------------------------------------------------------------------*/
Ensure canFindLastParameterInAList() {
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
    parameters = allocateParameterArray();
    
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


static Parameter *givenAnyParameterArrayOfLength(int length) {
    Parameter *parameters = allocateParameterArray();
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
    Parameter *theOriginal = NEW(Parameter);
    Parameter *theCopy = NEW(Parameter);

    theOriginal->instance = 2;
    theOriginal->candidates = allocate(4*sizeof(Parameter));
    setEndOfArray(&theOriginal->candidates[3]);

    setEndOfArray(&theCopy[0]);

    copyParameter(theCopy, theOriginal);

    assert_equal(theOriginal->instance, theCopy->instance);
    assert_true(equalParameterArrays(theOriginal->candidates, theCopy->candidates));
    assert_not_equal(theOriginal->candidates, theCopy->candidates);
}


/*----------------------------------------------------------------------*/
Ensure copyParameterArrayCanCopyNullToNull(void) {
    copyParameterArray(NULL, NULL);
}

/*======================================================================*/
TestSuite *paramsTests(void)
{
    TestSuite *suite = create_test_suite();

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

    return suite;
}

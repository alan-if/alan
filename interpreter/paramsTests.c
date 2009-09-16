#include "cgreen/cgreen.h"

#include "params.c"


/*----------------------------------------------------------------------*/
Ensure canFindLastParameterInAList() {
	Parameter parameters[10];

	memset(parameters, 45, sizeof(parameters));
	setEndOfList(&parameters[5]);
	assert_equal(&parameters[5], findEndOfList(parameters));
}


/*----------------------------------------------------------------------*/
Ensure canSetAndGetParameters(void) {
    int numberOfParameters = 4;
    Parameter parameters[numberOfParameters+1];
    int p;

    header->maxParameters = 10;
    
    setEndOfList(&parameters[numberOfParameters]);
    assert_equal(listLength(parameters), numberOfParameters);

    for (p = 0; p<numberOfParameters; p++)
        parameters[p].instance = p;
    
    setParameters(parameters);

    assert_equal(listLength(getParameters()), listLength(parameters));
    
    for (p = 0; !isEndOfList(&parameters[p]); p++)
        assert_equal(getParameter(p)->instance, p);
}


/*======================================================================*/
Ensure getWillAllocateStoredParameters(void) {
    assert_not_equal(getParameters(), NULL);
}

Ensure can_find_multiple_position(void) {
	Parameter parameters[10];
	int i;

	for (i=0; i<10; i++)
		parameters[i].instance = 2;
	parameters[7].instance = 0;
	setEndOfList(&parameters[9]);

	assert_equal(findMultiplePosition(parameters), 7);
}


Ensure returns_minus_one_for_no_multiple_position(void) {
	Parameter parameters[10];
	int i;

	for (i=0; i<10; i++)
		parameters[i].instance = 2;
	parameters[9].instance = -1;

	assert_equal(findMultiplePosition(parameters), -1);
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

    return suite;
}

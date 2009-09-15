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


/*======================================================================*/
TestSuite *paramsTests(void)
{
    TestSuite *suite = create_test_suite();

    add_test(suite, canFindLastParameterInAList);
    add_test(suite, canSetAndGetParameters);
    add_test(suite, getWillAllocateStoredParameters);

    return suite;
}

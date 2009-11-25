#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "parameterPosition.c"


/* Private Data */

static ACodeHeader acdHeader;

static void setUp(void) {
    header = &acdHeader;
    header->maxParameters = 10;
}


/*----------------------------------------------------------------------*/
Ensure canUncheckAllParameterPositions(void) {
    int i;
    ParameterPosition *parameterPositions = allocate((MAXPARAMS+1)*sizeof(ParameterPosition));

    for (i = 0; i < MAXPARAMS; i++)
        parameterPositions[i].checked = i;;
    uncheckAllParameterPositions(parameterPositions);
    for (i = 0; i < MAXPARAMS; i++) {
        assert_false(parameterPositions[i].checked);
    }
}


/*----------------------------------------------------------------------*/
Ensure canFindMultipleParameterPosition(void) {
    ParameterPosition parameterPositions[10];
    int i;

    for (i=0; i<10; i++) {
        parameterPositions[i].endOfList = FALSE;
        parameterPositions[i].explicitMultiple = FALSE;
    }
    parameterPositions[7].explicitMultiple = TRUE;
    parameterPositions[9].endOfList = TRUE;

    assert_equal(findMultipleParameterPosition(parameterPositions), 7);
}


/*----------------------------------------------------------------------*/
Ensure copyParameterPositionsCopyTheWholeListIncludingTheEndMarker(void) {
    ParameterPosition original[2];
    ParameterPosition copy[2];

    original[0].endOfList = FALSE;
    original[0].all = TRUE;
    original[0].explicitMultiple = FALSE;
    original[1].endOfList = TRUE;
    copy[0].endOfList = FALSE;
    copy[1].endOfList = FALSE;

    copyParameterPositions(original, copy);

    assert_equal(copy[0].all, original[0].all);
    assert_equal(copy[0].explicitMultiple, original[0].explicitMultiple);
    assert_true(copy[1].endOfList);
}


/*======================================================================*/
TestSuite *parameterPositionTests(void)
{
    TestSuite *suite = create_test_suite();

    setup(suite, setUp);

    add_test(suite, canUncheckAllParameterPositions);
    add_test(suite, canFindMultipleParameterPosition);
    add_test(suite, copyParameterPositionsCopyTheWholeListIncludingTheEndMarker);

    return suite;
}

#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "debug.c"

Ensure parseDebugCommandReturnsHELP_COMMANDForHelp() {
	assert_equal(HELP_COMMAND, parseDebugCommand("Help"));
}

Ensure parseDebugCommandReturnsHELP_COMMANDForQuestionMark() {
	assert_equal(HELP_COMMAND, parseDebugCommand("?"));
}

Ensure parseDebugCommandReturnsHELP_COMMANDForH() {
	assert_equal(HELP_COMMAND, parseDebugCommand("H"));
}

Ensure parseDebugCommandReturnsEXIT_COMMANDForX() {
	assert_equal(EXIT_COMMAND, parseDebugCommand("X"));
}

TestSuite *debugTests(void)
{
    TestSuite *suite = create_test_suite();

    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForHelp);
    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForQuestionMark);
    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForH);
    add_test(suite, parseDebugCommandReturnsEXIT_COMMANDForX);

    return suite;
}

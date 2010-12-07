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

Ensure parseDebugCommandReturnsBREAK_COMMANDForBr() {
	assert_equal(BREAK_COMMAND, parseDebugCommand("Br"));
}

Ensure parseDebugCommandReturnsAMBIGUOUS_COMMANDForE() {
	assert_equal(AMBIGUOUS_COMMAND, parseDebugCommand("E"));
}

TestSuite *debugTests(void)
{
    TestSuite *suite = create_test_suite();

    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForHelp);
    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForQuestionMark);
    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForH);
    add_test(suite, parseDebugCommandReturnsEXIT_COMMANDForX);
    add_test(suite, parseDebugCommandReturnsBREAK_COMMANDForBr);
    add_test(suite, parseDebugCommandReturnsAMBIGUOUS_COMMANDForE);

    return suite;
}

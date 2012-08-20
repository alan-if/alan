#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "debug.c"

Ensure (parseDebugCommandReturnsHELP_COMMANDForHelp) {
	assert_equal(HELP_COMMAND, parseDebugCommand("Help"));
}

Ensure (parseDebugCommandReturnsHELP_COMMANDForQuestionMark) {
	assert_equal(HELP_COMMAND, parseDebugCommand("?"));
}

Ensure (parseDebugCommandReturnsHELP_COMMANDForH) {
	assert_equal(HELP_COMMAND, parseDebugCommand("H"));
}

Ensure (parseDebugCommandReturnsEXIT_COMMANDForX) {
	assert_equal(EXIT_COMMAND, parseDebugCommand("X"));
}

Ensure (parseDebugCommandReturnsBREAK_COMMANDForBr) {
	assert_equal(BREAK_COMMAND, parseDebugCommand("Br"));
}

Ensure (parseDebugCommandReturnsAMBIGUOUS_COMMANDForE) {
	assert_equal(AMBIGUOUS_COMMAND, parseDebugCommand("E"));
}

Ensure (findSourceLineIndexFindsSameLineInOtherFiles) {
	SourceLineEntry lineTable[] = {
		{0, 3},
		{0, 5},
		{0, 35},
		{1, 33},
		{2, 35},
		{EOF, EOF}
	};
	assert_equal(findSourceLineIndex(lineTable, 0, 3), 0);
	assert_equal(findSourceLineIndex(lineTable, 0, 5), 1);
	assert_equal(findSourceLineIndex(lineTable, 0, 35), 2);
	assert_equal(findSourceLineIndex(lineTable, 1, 33), 3);
	assert_equal(findSourceLineIndex(lineTable, 2, 35), 4);
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

	add_test(suite, findSourceLineIndexFindsSameLineInOtherFiles);

    return suite;
}

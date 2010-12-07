#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "debug.c"

Ensure parseDebugCommandReturnsHELP_COMMANDForHelp() {
	assert_equal(HELP_COMMAND, parseDebugCommand("Help"));
}

TestSuite *debugTests(void)
{
    TestSuite *suite = create_test_suite();

    add_test(suite, parseDebugCommandReturnsHELP_COMMANDForHelp);

    return suite;
}

#include "cgreen/cgreen.h"

#include "act.c"


Ensure can_find_multiple_position(void) {
	ParamEntry parameters[10];
	int i;

	for (i=0; i<10; i++)
		parameters[i].instance = 2;
	parameters[7].instance = 0;
	parameters[9].instance = -1;

	assert_equal(findMultiplePosition(parameters), 7);
}

Ensure returns_minus_one_for_no_multiple_position(void) {
	ParamEntry parameters[10];
	int i;

	for (i=0; i<10; i++)
		parameters[i].instance = 2;
	parameters[9].instance = -1;

	assert_equal(findMultiplePosition(parameters), -1);
}

TestSuite *actTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, can_find_multiple_position);
  add_test(suite, returns_minus_one_for_no_multiple_position);
  return suite;
}

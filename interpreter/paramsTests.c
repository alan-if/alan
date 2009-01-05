#include "cgreen/cgreen.h"

#include "params.c"


Ensure canFindLastParameterInAList() {
	Parameter parameters[10];

	memset(parameters, 45, sizeof(parameters));
	setEndOfList(&parameters[5]);
	assert_equal(&parameters[5], findEndOfList(parameters));
}

/*======================================================================*/
TestSuite *paramsTests(void)
{
  TestSuite *suite = create_test_suite();

  add_test(suite, canFindLastParameterInAList);

  return suite;
}

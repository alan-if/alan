/*======================================================================*\

  descriptionTest.c

  Unit tests for DESCRIPTION node in the Alan compiler

\*======================================================================*/

#include "description.c"

#include <cgreen/cgreen.h>

#include "unit.h"


Ensure(testChecksOf) {
  Description *description = newDescription(nulsrcp, NULL, nulsrcp, NULL);
  List list;

  assert_true(checksOf(NULL) == NULL);
  assert_true(checksOf(description) == NULL);
  description->checks = &list;
  assert_true(checksOf(description) == &list);
}


TestSuite *descriptionTests() {
    TestSuite *suite = create_test_suite();

    add_test(suite, testChecksOf);

    return suite;
}


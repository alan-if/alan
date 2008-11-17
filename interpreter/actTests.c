#include "cgreen.h"

#include "act.c"


void this_test_should_pass() {
  assert_true(1);
}

TestSuite *actTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, this_test_should_pass);
  return suite;
}

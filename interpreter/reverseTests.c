#include "cgreen/cgreen.h"

#include "reverse.c"


Ensure(testAlreadyDone) {
  addressesDone = NULL;
  doneSize = 0;
  numberDone = 0;

  assert_true(alreadyDone(0));
  assert_equal(0, doneSize);

  assert_false(alreadyDone(1));
  assert_equal(100, doneSize);
  assert_equal(1, numberDone);

  assert_false(alreadyDone(2));
  assert_equal(100, doneSize);
  assert_equal(2, numberDone);

  assert_true(alreadyDone(1));
  assert_equal(100, doneSize);
  assert_equal(2, numberDone);
}


TestSuite *reverseTests()
{
  TestSuite *suite = create_test_suite();
  add_test(suite, testAlreadyDone);
  return suite;
}

#include "cgreen/cgreen.h"

#include "reverse.c"

Describe(Reverse);
BeforeEach(Reverse){};
AfterEach(Reverse){};

Ensure(Reverse, canSeeIfReversalIsAlreadyDone) {
  addressesDone = NULL;
  doneSize = 0;
  numberDone = 0;

  assert_that(alreadyDone(0));
  assert_that(doneSize, is_equal_to(0));

  assert_that(alreadyDone(1), is_false);
  assert_that(doneSize, is_equal_to(100));
  assert_that(numberDone, is_equal_to(1));

  assert_that(alreadyDone(2), is_false);
  assert_that(doneSize, is_equal_to(100));
  assert_that(numberDone, is_equal_to(2));

  assert_that(alreadyDone(1));
  assert_that(doneSize, is_equal_to(100));
  assert_that(numberDone, is_equal_to(2));
}


TestSuite *reverseTests()
{
  TestSuite *suite = create_test_suite();
  add_test_with_context(suite, Reverse, canSeeIfReversalIsAlreadyDone);
  return suite;
}

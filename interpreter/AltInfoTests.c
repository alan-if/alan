#include "cgreen.h"

#include "altInfo.c"


static void canPrimeAltInfo() {
  AltInfo altInfo;
  const int level = 17;
  const int parameter = 18;
  const int instance = 19;
  const int class = 20;

  primeAltInfo(&altInfo, level, parameter, instance, class);
  assert_equal(level, altInfo.level);
  assert_equal(parameter, altInfo.parameter);
  assert_equal(instance, altInfo.instance);
  assert_equal(class, altInfo.class);
}




TestSuite *altInfoTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, canPrimeAltInfo);
  return suite;
}

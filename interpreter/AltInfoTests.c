#include "cgreen/cgreen.h"

#include "AltInfo.c"

#include "AltInfoArray.h"


static void canPrimeAltInfo() {
  AltInfo altInfos[2];
  const int level = 17;
  const int parameter = 18;
  const int instance = 19;
  const int class = 20;

  primeAltInfo(altInfos, level, parameter, instance, class);

  assert_equal(level, altInfos[0].level);
  assert_equal(parameter, altInfos[0].parameter);
  assert_equal(instance, altInfos[0].instance);
  assert_equal(class, altInfos[0].class);
  assert_equal(TRUE, altInfos[1].end);
}




TestSuite *altInfoTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, canPrimeAltInfo);
  return suite;
}

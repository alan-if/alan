/*======================================================================*\

  ifidTest.c

  Unit tests for IFID node in the Alan compiler

\*======================================================================*/

#include "ifid.c"

#include <cgreen/cgreen.h>


Ensure(canVerifyUUID) {
  assert_true(!isValidUUID(""));
  assert_true(!isValidUUID("UUID://"));
  assert_true(!isValidUUID("UUID://12312123-1231-1231-123t-12367813desd//"));
  assert_true(isValidUUID("UUID://ef306e25-5f83-cc43-995b-fadba9df90f5//"));
}


TestSuite *ifidTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test(suite, canVerifyUUID);

    return suite;
}

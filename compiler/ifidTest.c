/*======================================================================*\

  ifidTest.c

  Unit tests for IFID node in the Alan compiler

\*======================================================================*/

#include "ifid.c"

#include <cgreen/cgreen.h>


Describe(Ifid);
BeforeEach(Ifid) {}
AfterEach(Ifid) {}


Ensure(Ifid, canVerifyUUID) {
  assert_true(!isValidUUID(""));
  assert_true(!isValidUUID("UUID://"));
  assert_true(!isValidUUID("UUID://12312123-1231-1231-123t-12367813desd//"));
  assert_true(isValidUUID("UUID://ef306e25-5f83-cc43-995b-fadba9df90f5//"));
}

/* randomUUID() used to sign-extend any byte from 0x80 up, printing eight hex
   digits where two were expected, which left a malformed UUID of varying
   length. Because the IFID goes into the acode string table, that moved every
   instruction address after it and made regression/debug/trace fail at random.
   Many iterations, because roughly a third of the bytes trigger it and a
   single draw can easily be clean. */
Ensure(Ifid, generatesWellFormedUUIDsEveryTime) {
  for (int i = 0; i < 200; i++) {
    char *uuid = randomUUID();
    assert_that(strlen(uuid), is_equal_to(45));
    assert_true(isValidUUID(uuid));
  }
}


TestSuite *ifidTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test_with_context(suite, Ifid, canVerifyUUID);
    add_test_with_context(suite, Ifid, generatesWellFormedUUIDsEveryTime);

    return suite;
}

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


/* The valid UUID above with one character replaced, so that each check in
   isValidUUID() can be made to fail on its own. */
static char *validUUIDWith(int position, char character) {
  static char uuid[46];
  strcpy(uuid, "UUID://ef306e25-5f83-cc43-995b-fadba9df90f5//");
  uuid[position] = character;
  return uuid;
}

Ensure(Ifid, rejectsUUIDWithoutItsPrefix) {
  assert_false(isValidUUID(validUUIDWith(0, 'X')));
}

/* Both ends of every group, since those are what the loop bounds decide */
Ensure(Ifid, rejectsNonHexDigitAtEitherEndOfEveryGroup) {
  int positions[] = {7, 14, 16, 19, 21, 24, 26, 29, 31, 42};
  for (size_t i = 0; i < sizeof(positions)/sizeof(positions[0]); i++)
    assert_false_with_message(isValidUUID(validUUIDWith(positions[i], 'g')),
                              "non-hex digit at position %d was accepted", positions[i]);
}

Ensure(Ifid, rejectsAnythingButADashBetweenGroups) {
  int positions[] = {15, 20, 25, 30};
  for (size_t i = 0; i < sizeof(positions)/sizeof(positions[0]); i++)
    assert_false_with_message(isValidUUID(validUUIDWith(positions[i], '_')),
                              "'_' instead of '-' at position %d was accepted", positions[i]);
}

Ensure(Ifid, rejectsUUIDWithoutTrailingSlashes) {
  assert_false(isValidUUID(validUUIDWith(44, 'x')));
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
    add_test_with_context(suite, Ifid, rejectsUUIDWithoutItsPrefix);
    add_test_with_context(suite, Ifid, rejectsNonHexDigitAtEitherEndOfEveryGroup);
    add_test_with_context(suite, Ifid, rejectsAnythingButADashBetweenGroups);
    add_test_with_context(suite, Ifid, rejectsUUIDWithoutTrailingSlashes);
    add_test_with_context(suite, Ifid, generatesWellFormedUUIDsEveryTime);

    return suite;
}

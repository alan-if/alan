/*======================================================================*\

  ifidTest.c

  Unit tests for IFID node in the Alan compiler

\*======================================================================*/

#include "ifid.c"

static void testIsValidUUID() {
  ASSERT(!isValidUUID(""));
  ASSERT(!isValidUUID("UUID://"));
  ASSERT(!isValidUUID("UUID://12312123-1231-1231-123t-12367813desd//"));
  ASSERT(isValidUUID("UUID://ef306e25-5f83-cc43-995b-fadba9df90f5//"));
}


void registerIfidUnitTests()
{
  registerUnitTest(testIsValidUUID);
}

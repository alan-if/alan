/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

void testInitialLocation()
{
  unitAssert(generateInitialLocation(NULL) == 0);
}


void registerWhrUnitTests()
{
  registerUnitTest(testInitialLocation);
}


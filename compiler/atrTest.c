/*======================================================================*\

  atrTest.c

  Unit tests for ATR node in the Alan compiler

\*======================================================================*/

#include "atr.c"

void testInitAtr()
{
  unitAssert(TRUE);
}


void registerAtrUnitTests()
{
  registerUnitTest(testInitAtr);
}


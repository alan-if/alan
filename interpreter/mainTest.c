/*======================================================================*\

  mainTest.c

  Unit tests for main module in the Alan interpreter

\*======================================================================*/

#include "main.c"

static void testUpdateColumn() {
  ASSERT(updateColumn(0, "\n") == 1);
  ASSERT(updateColumn(11, "123456789") == 20);
}


void registerMainUnitTests()
{
  registerUnitTest(testUpdateColumn);
}

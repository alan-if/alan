/*======================================================================*\

  stmTest.c

  Unit tests for STM node in the Alan compiler

\*======================================================================*/

#include "stm.c"

void testGenerateEach()
{
  initEmit("unit.acd");

  frameLevel = 1;
  generateGetLocal(id);
  generateSetLocal(id);
  
}


void registerStmUnitTests()
{
  registerUnitTest(testGenerateEach);
}


/*======================================================================*\

  emitTest.c

  Unit tests for EMIT node in the Alan compiler

\*======================================================================*/

#include "emit.c"

void testEmit()
{
  int expectedAddress;
  Aword emitTestArray[5] = {1, 2, 3, 4, 5};

  initEmit("unit.acd");

  expectedAddress = emadr();

  emit(0);
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitstr("123");
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitstr("1234");
  expectedAddress += 2;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emit0(C_STMOP, I_IF);
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitN((Aword *)&emitTestArray, 5);
  expectedAddress += 5;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();
}


void registerEmitUnitTests()
{
  registerUnitTest(testEmit);
}


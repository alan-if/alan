/*======================================================================*\

  stackTest.c

  Unit tests for stack module in the Alan interpreter

\*======================================================================*/

#include "stack.c"


static void testNewBlock()
{
  Aint originalSp;

  blockPointer = 47;
  originalSp = stackp;

  /* Add a block with four local variables */
  newBlock(4);

  unitAssert(stackp == originalSp + 1/*old bp*/ + 4/*Locals*/);
  unitAssert(blockPointer == originalSp + 1);

  unitAssert(getLocal(0,1) == 0);
  setLocal(0,1,14);
  unitAssert(getLocal(0,1) == 14);
  unitAssert(stack[stackp - 4] == 14);
  unitAssert(stack[stackp - 5] == 47);

  endBlock();
  unitAssert(stackp == originalSp);
  unitAssert(blockPointer == 47);
}  

  

void registerStackUnitTests()
{
  registerUnitTest(testNewBlock);
}

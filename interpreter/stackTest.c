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

  ASSERT(stackp == originalSp + 1/*old bp*/ + 4/*Locals*/);
  ASSERT(blockPointer == originalSp + 1);

  ASSERT(getLocal(0,1) == 0);
  setLocal(0,1,14);
  ASSERT(getLocal(0,1) == 14);
  ASSERT(stack[stackp - 4] == 14);
  ASSERT(stack[stackp - 5] == 47);

  endBlock();
  ASSERT(stackp == originalSp);
  ASSERT(blockPointer == 47);
}  

  

void registerStackUnitTests()
{
  registerUnitTest(testNewBlock);
}

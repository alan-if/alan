#include "cgreen.h"

#include "stack.c"


/*----------------------------------------------------------------------*/
static void testNewFrame()
{
  Aint originalSp;
  int ORIGINAL_FRAMEPOINTER = 47;

  framePointer = ORIGINAL_FRAMEPOINTER;
  originalSp = stackp;

  /* Add a block with four local variables */
  newFrame(NULL, 4);
  assert_equal(originalSp + 1/*old fp*/ + 4/*Locals*/, stackp);
  assert_equal(originalSp + 1, framePointer);

  assert_equal(0, getLocal(NULL, 0,1));
  setLocal(NULL, 0,1,14);
  assert_equal(14, getLocal(NULL, 0,1));
  assert_equal(14, stack[stackp - 4]);
  assert_equal(ORIGINAL_FRAMEPOINTER, stack[stackp - 5]);

  endFrame(NULL);
  assert_equal(originalSp, stackp);
  assert_equal(ORIGINAL_FRAMEPOINTER, framePointer);
}  

  
/*----------------------------------------------------------------------*/
static void testNewFrameInStack()
{
  Stack myStack = createStack(50);

  Aint originalSp;
  int ORIGINAL_FRAMEPOINTER = 47;

  framePointer = ORIGINAL_FRAMEPOINTER;
  originalSp = stackp;

  /* Add a block with four local variables */
  newFrame(myStack, 4);
  assert_equal(1/*old fp*/ + 4/*Locals*/, stackDepth(myStack));
  assert_equal(1, myStack->framePointer);

  assert_equal(0, getLocal(myStack, 0,1));
  setLocal(myStack, 0,1,14);
  assert_equal(14, getLocal(myStack, 0,1));
  assert_equal(14, myStack->stack[myStack->stackp - 4]);
  assert_equal(ORIGINAL_FRAMEPOINTER, myStack->stack[myStack->stackp - 5]);

  endFrame(myStack);
  assert_equal(originalSp, stackp);
  assert_equal(ORIGINAL_FRAMEPOINTER, framePointer);
}  

  
/*----------------------------------------------------------------------*/
static void testFrameInFrame()
{
  Aint originalSp;
  int ORIGINAL_FRAMEPOINTER = 47;

  framePointer = ORIGINAL_FRAMEPOINTER;
  originalSp = stackp;

  /* Add a block with one local variable */
  newFrame(NULL, 1);
  setLocal(NULL, 0,1,14);
  assert_equal(14, getLocal(NULL, 0,1));

  newFrame(NULL, 1);
  setLocal(NULL, 0,1,15);
  assert_equal(15, getLocal(NULL, 0,1));
  assert_equal(14, getLocal(NULL, 1,1));
  endFrame(NULL);
  endFrame(NULL);
  assert_equal(ORIGINAL_FRAMEPOINTER, framePointer);
}  

/*----------------------------------------------------------------------*/
static void testPushAndPop()
{
  Stack myStack = createStack(10);

  push(myStack, 1);
  push(myStack, 2);
  push(myStack, 3);
  push(myStack, 4);
  assert_equal(4, stackDepth(myStack));
  assert_equal(4, pop(myStack));
  assert_equal(3, pop(myStack));
  assert_equal(2, pop(myStack));
  assert_equal(1, pop(myStack));
  assert_equal(0, stackDepth(myStack));
}
  

TestSuite *stackTests()
{
  TestSuite *suite = create_test_suite();
  add_test(suite, testNewFrame);
  add_test(suite, testNewFrameInStack);
  add_test(suite, testFrameInFrame);
  add_test(suite, testPushAndPop);
  return suite;
}

#include "cgreen.h"

#include "stack.c"


static Stack theStack;

static void setUp() {
  theStack = createStack(50);
}

static void tearDown() {
  deleteStack(theStack);
}


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
  /* Add a block with four local variables */
  newFrame(theStack, 4);
  assert_equal(1/*old fp*/ + 4/*Locals*/, stackDepth(theStack));
  assert_equal(1, theStack->framePointer);

  assert_equal(0, getLocal(theStack, 0,1));
  setLocal(theStack, 0,1,14);
  assert_equal(14, getLocal(theStack, 0,1));
  assert_equal(14, theStack->stack[theStack->stackp - 4]);
  assert_equal(-1, theStack->stack[theStack->stackp - 5]);

  endFrame(theStack);
  assert_equal(0, stackDepth(theStack));
}  

  
/*----------------------------------------------------------------------*/
static void testFrameInFrame()
{
  /* Add a block with one local variable */
  newFrame(theStack, 1);
  setLocal(theStack, 0,1,14);
  assert_equal(14, getLocal(theStack, 0,1));

  newFrame(theStack, 1);
  setLocal(theStack, 0,1,15);
  assert_equal(15, getLocal(theStack, 0,1));
  assert_equal(14, getLocal(theStack, 1,1));
  endFrame(theStack);
  endFrame(theStack);
  assert_equal(0, stackDepth(theStack));
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

  setup(suite, setUp);
  teardown(suite, tearDown);

  add_test(suite, testNewFrame);
  add_test(suite, testNewFrameInStack);
  add_test(suite, testFrameInFrame);
  add_test(suite, testPushAndPop);

  return suite;
}

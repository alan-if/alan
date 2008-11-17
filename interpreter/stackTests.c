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
  newFrame(4);
  assert_equal(originalSp + 1/*old fp*/ + 4/*Locals*/, stackp);
  assert_equal(originalSp + 1, framePointer);

  assert_equal(0, getLocal(0,1));
  setLocal(0,1,14);
  assert_equal(14, getLocal(0,1));
  assert_equal(14, stack[stackp - 4]);
  assert_equal(ORIGINAL_FRAMEPOINTER, stack[stackp - 5]);

  endFrame();
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
  newFrame(1);
  setLocal(0,1,14);
  assert_equal(14, getLocal(0,1));

  newFrame(1);
  setLocal(0,1,15);
  assert_equal(15, getLocal(0,1));
  assert_equal(14, getLocal(1,1));
  endFrame();
  endFrame();
  assert_equal(ORIGINAL_FRAMEPOINTER, framePointer);
}  

  

TestSuite *stackTests()
{
  TestSuite *suite = create_test_suite();
  add_test(suite, testNewFrame);
  add_test(suite, testFrameInFrame);
  return suite;
}

#include "cgreen.h"

#include "inter.c"

#include "stack.h"


static void syserrHandler(char *message) {
  fail();
}

static Stack theStack;



/*----------------------------------------------------------------------*/

static void setUp() {
  theStack = createStack(50);
  setInterpreterStack(theStack);
  setSyserrHandler(syserrHandler);
  memTop = 100;
}

static void tearDown() {
  deleteStack(theStack);
  setSyserrHandler(NULL);
}


/*----------------------------------------------------------------------*/
static void testBlockInstructions()
{
  Aword blockInstructionCode[] = {4, /* Dummy to not execute at zero */
				  4,
				  INSTRUCTION(I_FRAME),
				  4,
				  INSTRUCTION(I_RETURN)};
  Aword localsInstructionCode[] = {4, /* Dummy to not execute at zero */
				   33, /* Value */
				   1, /* Local variable (starts at 1) */
				   0, /* Number of blocks down */
				   INSTRUCTION(I_SETLOCAL),
				   1, /* Local variable (starts at 1) */
				   0, /* Number of blocks down */
				   INSTRUCTION(I_GETLOCAL),
				   INSTRUCTION(I_RETURN)};

  memory = blockInstructionCode;

  /* Add a block with four local variables */
  interpret(1);

  assert_equal(1/*old fp*/ + 4/*Locals*/ + 1/*The extra "4"*/, stackDepth(theStack));

  memory = localsInstructionCode;
  interpret(1);
  assert_true(pop(theStack) == 33);
}  


/*----------------------------------------------------------------------*/
static void testLoopInstruction()
{
  Aword loopInstructionCode1[] = {4, /* Dummy to not execute at zero */
				  43, /* Marker */
				  12, /* Limit */
				  1, /* Index */
				  INSTRUCTION(I_LOOP),
				  INSTRUCTION(I_RETURN)};
  memory = loopInstructionCode1;
  interpret(1);			/* Should not do anything */
  assert_true(pop(theStack) == 1 && pop(theStack) == 12); /* Index and limit untouched */
  assert_true(pop(theStack) == 43);		/* So the stack should contain the marker */
}


/*----------------------------------------------------------------------*/
static void testLoopEndInstruction()
{
  Aword loopEndInstructionCode[] = {4, /* Dummy to not execute at zero */
				  1,
				  INSTRUCTION(I_FRAME),
				  4, /* Marker on the stack */
				  12, /* End value */
				  9, /* Start value */
				  INSTRUCTION(I_LOOP),
				  INSTRUCTION(I_DUP),
				  1,
				  0,
				  INSTRUCTION(I_SETLOCAL),
				  INSTRUCTION(I_LOOPEND),
				  INSTRUCTION(I_RETURN)};
  memory = loopEndInstructionCode;
  interpret(1);
  assert_true(getLocal(theStack, 0, 1) == 12);
  assert_true(pop(theStack) == 4);
}


/*----------------------------------------------------------------------*/
static void testGoToLoop() {
  Aword testGoToLoopCode[] = {0,
			      INSTRUCTION(I_LOOP), /* 1 */
			      4,
			      INSTRUCTION(I_LOOP),
			      4,
			      INSTRUCTION(I_LOOPNEXT),
			      INSTRUCTION(I_LOOPNEXT),
			      INSTRUCTION(I_LOOPEND),
			      5,
			      INSTRUCTION(I_LOOPEND)}; /* 9 */
  memory = testGoToLoopCode;
  pc = 9;
  jumpBackToStartOfMatchingLOOP();
  assert_true(pc == 1);
}


/*----------------------------------------------------------------------*/
static void testLoopNext() {
  Aword testLoopNextCode[] = {0,
			      INSTRUCTION(I_LOOP),
			      4, /* 2 */
			      INSTRUCTION(I_LOOP),
			      4,
			      INSTRUCTION(I_LOOPNEXT),
			      INSTRUCTION(I_LOOPNEXT),
			      INSTRUCTION(I_LOOPEND),
			      5,
			      INSTRUCTION(I_LOOPEND)}; /* 9 */
  memory = testLoopNextCode;
  pc = 2;
  nextLoop();
  assert_true(pc == 9);
}


/*----------------------------------------------------------------------*/
static void testCountInstruction()
{
  Aword testCountInstructionCode[] = {0,
				      INSTRUCTION(I_COUNT), /* 7 */
				      INSTRUCTION(I_RETURN)}; /* 8 */
  memory = testCountInstructionCode;

  /* Execute an I_COUNT */
  push(theStack, 2);			/* Faked COUNT value */
  push(theStack, 44);			/* Faked limit */
  push(theStack, 5);			/* Faked loop index */
  interpret(1);
  assert_equal(5, pop(theStack));	/* Values intact */
  assert_equal(44, pop(theStack));	/* Values intact */
  assert_equal(3, pop(theStack));	/* Incremented COUNT */
}


/*----------------------------------------------------------------------*/
static void testMaxInstruction() {
  Aword testMaxInstructionCode[] = {0,
				    INSTRUCTION(I_MAX),
				    INSTRUCTION(I_RETURN)};

  push(theStack, 3);			/* Previous aggregate value */
  push(theStack, 11);			/* Limit */
  push(theStack, 12);			/* Index */
  push(theStack, 2);			/* Attribute value */
  memory = testMaxInstructionCode;
  interpret(1);
  assert_equal(12, pop(theStack));
  assert_equal(11, pop(theStack));
  assert_equal(3, pop(theStack));

  push(theStack, 3);			/* Previous aggregate value */
  push(theStack, 11);			/* Limit */
  push(theStack, 12);			/* Index */
  push(theStack, 4);			/* Attribute value */
  memory = testMaxInstructionCode;
  interpret(1);
  assert_equal(12, pop(theStack));
  assert_equal(11, pop(theStack));
  assert_equal(4, pop(theStack));
}


/*----------------------------------------------------------------------*/
static void testMaxInstance() {
  Aword testMaxInstanceCode[] = {0,
				 CURVAR(V_MAX_INSTANCE),
				 INSTRUCTION(I_RETURN)};
  header->instanceMax = 12;
  memory = testMaxInstanceCode;
  interpret(1);
  assert_true(pop(theStack) == header->instanceMax);
}


/*----------------------------------------------------------------------*/
TestSuite *interTests(void)
{
  TestSuite *suite = create_test_suite();

  setup(suite, setUp);
  teardown(suite, tearDown);

  add_test(suite, testBlockInstructions);
  add_test(suite, testGoToLoop);
  add_test(suite, testLoopNext);
  add_test(suite, testLoopInstruction);
  add_test(suite, testLoopEndInstruction);
  add_test(suite, testMaxInstruction);
  add_test(suite, testCountInstruction);
  add_test(suite, testMaxInstance);

  return suite;
}

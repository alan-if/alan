/*======================================================================*\

  interTest.c

  Unit tests for inter module in the Alan interpreter

\*======================================================================*/

#include "inter.c"


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
  Aint originalSp;

  memory = blockInstructionCode;
  memTop = 100;
  framePointer = 47;
  originalSp = stackp;

  /* Add a block with four local variables */
  interpret(1);

  ASSERT(stackp == originalSp + 1/*old bp*/ + 4/*Locals*/ + 1/*The extra "4"*/);

  memory = localsInstructionCode;
  interpret(1);
  ASSERT(pop() == 33);
}  


static void testEachInstructions()
{
  Aword eachInstructionCode1[] = {4, /* Dummy to not execute at zero */
				  1, /* One local variable */
				  INSTRUCTION(I_FRAME),
				  12, /* End value */
				  11, /* Start value */
				  INSTRUCTION(I_EACH),
				  INSTRUCTION(I_RETURN)};

  Aword eachInstructionCode2[] = {4, /* Dummy to not execute at zero */
				  1,
				  INSTRUCTION(I_FRAME),
				  4, /* Marker on the stack */
				  12, /* End value */
				  9, /* Start value */
				  INSTRUCTION(I_EACH),
				  INSTRUCTION(I_ENDEACH),
				  INSTRUCTION(I_RETURN)};

  ACodeHeader testForHeader;

  testForHeader.instanceMax = 2;
  header = &testForHeader;

  memory = eachInstructionCode1;
  interpret(1);			/* Just set up */
  ASSERT(getLocal(0,1) == 11);	/* So local should be initial value */

  memory = eachInstructionCode2;
  interpret(1);

  ASSERT(getLocal(0, 1) == 12);
  ASSERT(pop() == 4);
}


static void testGoToEach() {
  Aword testGoToEachCode[] = {0,
			      INSTRUCTION(I_EACH), /* 1 */
			      4,
			      INSTRUCTION(I_EACH),
			      4,
			      INSTRUCTION(I_NEXTEACH),
			      INSTRUCTION(I_NEXTEACH),
			      INSTRUCTION(I_ENDEACH),
			      5,
			      INSTRUCTION(I_ENDEACH)}; /* 9 */
  memory = testGoToEachCode;
  pc = 9;
  goToEACH();
  ASSERT(pc == 1);
}


static void testNextEach() {
  Aword testNextEachCode[] = {0,
			      INSTRUCTION(I_EACH),
			      4, /* 2 */
			      INSTRUCTION(I_EACH),
			      4,
			      INSTRUCTION(I_NEXTEACH),
			      INSTRUCTION(I_NEXTEACH),
			      INSTRUCTION(I_ENDEACH),
			      5,
			      INSTRUCTION(I_ENDEACH)}; /* 9 */
  memory = testNextEachCode;
  pc = 2;
  nextEach();
  ASSERT(pc == 9);
}


static void testAggregateInstructions(void)
{
  Aword testAgrStartCode[] = {0,
			      INSTRUCTION(I_AGRSTART),	/* 1 */
			      INSTRUCTION(I_RETURN)}; /* 2 */
  Aword testAggregateInstructionCode[] = {0,
					  INSTRUCTION(I_AGRSTART),	/* 1 */
					  4, 5, 6,
					  INSTRUCTION(I_AGREND), /* 5 */
					  INSTRUCTION(I_RETURN), /* 6 */
					  INSTRUCTION(I_COUNT), /* 7 */
					  INSTRUCTION(I_RETURN)}; /* 8 */
  Aint originalSp;

  memory = testAgrStartCode;
  pc = 1;
  originalSp = stackp;
  push(2);			/* Upper loop limit */
  push(1);			/* Start loop value */
  push(0);			/* Initial aggregate value */
  interpret(1);
  ASSERT(stackp == originalSp+3);

  memory = testAggregateInstructionCode;
  pc = 5;
  goToAGRSTART();
  ASSERT(memory[pc-1] == INSTRUCTION(I_AGRSTART));

  goToAGREND();
  ASSERT(memory[pc] == INSTRUCTION(I_AGREND));

  /* Execute an AGREND */
  push(1);			/* Loop terminating limit */
  push(4);			/* Aggregation value */
  push(1);			/* Loop index */
  push(1);			/* Loop value */
  interpret(5);
  ASSERT(pop() == 4);

  /* Execute an I_COUNT */
  push(2);			/* Faked COUNT value */
  push(5);			/* Faked loop index */
  push(5);			/* Loop value */
  interpret(7);
  ASSERT(pop() == 5 && pop() == 5);	/* Values intact */
  ASSERT(pop() == 3);		/* Incremented COUNT */
}

static void testMaxInstance() {
  Aword testMaxInstanceCode[] = {0,
				 CURVAR(V_MAX_INSTANCE),
				 INSTRUCTION(I_RETURN)};
  header->instanceMax = 12;
  memory = testMaxInstanceCode;
  interpret(1);
  ASSERT(pop() == header->instanceMax);
}


void registerInterUnitTests(void)
{
  registerUnitTest(testBlockInstructions);
  registerUnitTest(testGoToEach);
  registerUnitTest(testNextEach);
  registerUnitTest(testEachInstructions);
  registerUnitTest(testAggregateInstructions);
  registerUnitTest(testMaxInstance);
}

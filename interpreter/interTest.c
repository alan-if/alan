/*======================================================================*\

  interTest.c

  Unit tests for inter module in the Alan interpreter

\*======================================================================*/

#include "inter.c"


static Aword blockInstructionCode[] = {4, /* Dummy to not execute at zero */
				       4,
				       INSTRUCTION(I_FRAME),
				       4,
				       INSTRUCTION(I_RETURN)};

static Aword localsInstructionCode[] = {4, /* Dummy to not execute at zero */
					33, /* Value */
					1, /* Local variable (starts at 1) */
					0, /* Number of blocks down */
					INSTRUCTION(I_SETLOCAL),
					1, /* Local variable (starts at 1) */
					0, /* Number of blocks down */
					INSTRUCTION(I_GETLOCAL),
					INSTRUCTION(I_RETURN)};

static void testBlockInstructions()
{
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


static Aword forInstructionCode[] = {4, /* Dummy to not execute at zero */
				     2,
				     INSTRUCTION(I_FRAME),
				     1, /* Local loop variable */
				     INSTRUCTION(I_EACH),
				     2,
				     INSTRUCTION(I_EACH),
				     INSTRUCTION(I_ENDEACH),
				     INSTRUCTION(I_ENDEACH),
				     INSTRUCTION(I_RETURN)};

static void testForInstructions()
{
  AcdHdr testForHeader;

  testForHeader.instanceMax = 2;
  header = &testForHeader;
  memory = forInstructionCode;

  interpret(1);

  ASSERT(getLocal(0, 1) == 2);
  ASSERT(getLocal(0, 2) == 2);
}


static Aword testAggregateInstructionCode[] = {0,
					       INSTRUCTION(I_AGRSTART),	/* 1 */
					       4, 5, 6,
					       INSTRUCTION(I_ENDAGR), /* 5 */
					       INSTRUCTION(I_RETURN), /* 6 */
					       INSTRUCTION(I_COUNT), /* 7 */
					       INSTRUCTION(I_RETURN) /* 8 */
};

static void testAggregateInstructions(void)
{
  AcdHdr agrHeader;

  memory = testAggregateInstructionCode;
  pc = 5;
  goToAGRSTART();
  ASSERT(memory[pc-1] == INSTRUCTION(I_AGRSTART));

  goToENDAGR();
  ASSERT(memory[pc] == INSTRUCTION(I_ENDAGR));

  agrHeader.instanceMax = 1;
  header = &agrHeader;

  push(4);			/* Faked COUNT value */
  push(1);			/* Faked instance index */
  interpret(5);
  ASSERT(pop() == 4);

  push(2);			/* Faked COUNT value */
  push(5);			/* Faked instance index */
  push(5);			/* Twice */
  interpret(7);
  ASSERT(pop() == 5);		/* Same instance index */
  ASSERT(pop() == 3);		/* Incremented COUNT */
}


void registerInterUnitTests(void)
{
  registerUnitTest(testBlockInstructions);
  registerUnitTest(testForInstructions);
  registerUnitTest(testAggregateInstructions);
}

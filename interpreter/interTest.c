/*======================================================================*\

  interTest.c

  Unit tests for inter module in the Alan interpreter

\*======================================================================*/

#include "inter.c"


static Aword blockInstructionCode[] = {4, /* Dummy to not execute at zero */
				       4,
				       INSTRUCTION(I_BLOCK),
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
  blockPointer = 47;
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
				     INSTRUCTION(I_BLOCK),
				     1, /* Local loop variable */
				     INSTRUCTION(I_EACH),
				     2,
				     INSTRUCTION(I_EACH),
				     INSTRUCTION(I_ENDEACH),
				     INSTRUCTION(I_ENDEACH),
				     INSTRUCTION(I_RETURN)};

static AcdHdr testForHeader;

static void testForInstructions()
{
  testForHeader.instanceMax = 2;
  header = &testForHeader;
  memory = forInstructionCode;

  interpret(1);

  ASSERT(getLocal(0, 1) == 2);
  ASSERT(getLocal(0, 2) == 2);
}

void registerInterUnitTests()
{
  registerUnitTest(testBlockInstructions);
  registerUnitTest(testForInstructions);
}

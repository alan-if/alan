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
					0, /* Number of blocks down */
					1, /* Local variable (starts at 1) */
					33, /* Value */
					INSTRUCTION(I_SETLOCAL),
					0, /* Number of blocks down */
					1, /* Local variable (starts at 1) */
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

  unitAssert(stackp == originalSp + 1/*old bp*/ + 4/*Locals*/ + 1/*The extra "4"*/);

  memory = localsInstructionCode;
  interpret(1);
  unitAssert(pop() == 33);
}  


static Aword forInstructionCode[] = {4, /* Dummy to not execute at zero */
				     2,
				     INSTRUCTION(I_BLOCK),
				     1, /* Local loop variable */
				     INSTRUCTION(I_FOR),
				     2,
				     INSTRUCTION(I_FOR),
				     INSTRUCTION(I_ENDFOR),
				     INSTRUCTION(I_ENDFOR),
				     INSTRUCTION(I_RETURN)};

static AcdHdr testForHeader;

static void testForInstructions()
{
  testForHeader.instanceMax = 2;
  header = &testForHeader;
  memory = forInstructionCode;

  interpret(1);

  unitAssert(getLocal(0, 1) == 2);
  unitAssert(getLocal(0, 2) == 2);
}

void registerInterUnitTests()
{
  registerUnitTest(testBlockInstructions);
  registerUnitTest(testForInstructions);
}

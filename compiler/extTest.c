/*======================================================================*\

  extTest.c

  Unit tests for EXT node in the Alan compiler

\*======================================================================*/

#include "ext.c"

void testNewExt()
{
  IdNode *direction = newId(nulsrcp, "w");
  IdNode *targetLocation = newId(nulsrcp, "aLocation");
  Exit *theExit;
  Context *context = pushContext(NULL);
  IdNode *aLocationId = newId(nulsrcp, "aLocation");
  Symbol *aLocationSymbol;
  int firstAddress;
  int entrySize = AwordSizeOf(ExitEntry);

  initAdventure();
  (void) readEcode();
  aLocationSymbol = newSymbol(aLocationId, INSTANCE_SYMBOL);

  theExit = newExit(&nulsrcp, concat(NULL, direction, EXIT_LIST), targetLocation, NULL, NULL);
  ASSERT(theExit->dirs->element.id->symbol != NULL && theExit->dirs->element.id->symbol->code == 1);

  symbolizeExit(theExit);
  ASSERT(readEcode() == 0);

  analyzeExit(theExit, context);
  ASSERT(readEcode() == 351);

  initEmit("unit.a3c");
  firstAddress = nextEmitAddress();
  generateExitEntry(theExit);
  ASSERT(nextEmitAddress() == firstAddress + entrySize);
}


void testHaveExit()
{
  List *exits = concat(concat(NULL,
			      newExit(&nulsrcp,
				     concat(concat(NULL,
						   newId(nulsrcp, "north"), ID_LIST),
					    newId(nulsrcp, "south"), ID_LIST),
				     NULL, NULL, NULL), EXIT_LIST),
		       newExit(&nulsrcp,
			      concat(concat(NULL,
					    newId(nulsrcp, "west"), ID_LIST),
				     newId(nulsrcp, "east"), ID_LIST),
			      NULL, NULL, NULL), EXIT_LIST);

  
  ASSERT(haveExit(exits, newId(nulsrcp, "south")));
  ASSERT(haveExit(exits, newId(nulsrcp, "north")));
  ASSERT(haveExit(exits, newId(nulsrcp, "east")));
  ASSERT(haveExit(exits, newId(nulsrcp, "west")));
}

void registerExtUnitTests()
{
  registerUnitTest(testNewExt);
  registerUnitTest(testHaveExit);
}


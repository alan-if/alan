/*======================================================================*\

  extTest.c

  Unit tests for EXT node in the Alan compiler

\*======================================================================*/

#include "ext.c"

void testNewExt()
{
  IdNode *direction = newId(&nulsrcp, "w");
  IdNode *targetLocation = newId(&nulsrcp, "aLocation");
  ExtNod *theExit;
  Context *context = pushContext(NULL);
  IdNode *aLocationId = newId(&nulsrcp, "aLocation");
  Symbol *aLocationSymbol;
  int firstAddress;
  int entrySize = ACDsizeOf(ExitEntry);

  initAdventure();
  (void) readEcode();
  aLocationSymbol = newSymbol(aLocationId, INSTANCE_SYMBOL);

  theExit = newext(&nulsrcp, concat(NULL, direction, EXIT_LIST), targetLocation, NULL, NULL);
  ASSERT(theExit->dirs->element.id->symbol->code = 1);

  symbolizeExit(theExit);
  ASSERT(readEcode() == 0);

  analyzeExit(theExit, context);
  ASSERT(readEcode() == 351);

  initEmit("unit.a3c");
  firstAddress = emadr();
  generateExitEntry(theExit);
  ASSERT(emadr() == firstAddress + entrySize);
}


void registerExtUnitTests()
{
  registerUnitTest(testNewExt);
}


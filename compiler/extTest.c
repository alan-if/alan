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
  Context *context = copyContext(NULL);
  IdNode *aLocationId = newId(&nulsrcp, "aLocation");
  Symbol *aLocationSymbol;
  int firstAddress;
  int entrySize = ACDsizeOf(ExitEntry);

  initadv();
  (void) readEcode();
  aLocationSymbol = newSymbol(aLocationId, INSTANCE_SYMBOL);

  theExit = newext(&nulsrcp, concat(NULL, direction, LIST_EXT), targetLocation, NULL, NULL);
  unitAssert(theExit->dirs->element.id->symbol->code = 1);

  symbolizeExit(theExit);
  unitAssert(readEcode() == 0);

  analyzeExit(theExit, context);
  unitAssert(readEcode() == 351);

  initEmit("unit.acd");
  firstAddress = emadr();
  generateExitEntry(theExit);
  unitAssert(emadr() == firstAddress + entrySize);
}


void registerExtUnitTests()
{
  registerUnitTest(testNewExt);
}


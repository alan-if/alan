/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

void testInitialLocation()
{
  IdNode *id = newId(&nulsrcp, "atLoc");
  WhrNod *whr = newwhr(&nulsrcp, WHR_AT, newWhat(&nulsrcp, WHAT_ID, id));
  InsNod *atLoc = newInstance(&nulsrcp, id, NULL, NULL);

  symbolizeInstance(atLoc);
  symbolizeWhere(whr);
  unitAssert(generateInitialLocation(NULL) == 0);

  unitAssert(generateInitialLocation(whr) == atLoc->slots->id->symbol->code);
}

void registerWhrUnitTests()
{
  registerUnitTest(testInitialLocation);
}


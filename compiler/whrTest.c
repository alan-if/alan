/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

void testInitialLocation()
{
  IdNode *id = newId(&nulsrcp, "atLoc");
  WhrNod *whr = newwhr(&nulsrcp, WHR_AT, newwht(&nulsrcp, WHT_ID, id));
  InsNod *atLoc = newInstance(&nulsrcp, id, NULL, NULL);

  symbolizeInstance(atLoc);
  symbolizeWhr(whr);
  unitAssert(generateInitialLocation(NULL) == 0);

  unitAssert(generateInitialLocation(whr) == atLoc->slots->symbol->code);
}

void registerWhrUnitTests()
{
  registerUnitTest(testInitialLocation);
}


/*======================================================================*\

  advTest.c

  Unit tests for ADV node in the Alan compiler

\*======================================================================*/

#include "adv.c"


void testInitAdv()
{
  Srcp srcp = {2,3,4};
  adv.whr = newwhr(&srcp, WHR_HERE, NULL);

  analyzeStartAt();		/* Can not Start At Here */
  unitAssert(readEcode() == 211 && readSev() == sevERR);

  adv.whr = newwhr(&srcp, WHR_AT,
		   newwht(&srcp, WHT_ID,
			  newId(&srcp, "atL")));
  analyzeStartAt();		/* Can not Start At unknown Id */
  unitAssert(readSev() == sevERR && readEcode() == 310);
}


void registerAdvUnitTests()
{
  registerUnitTest(testInitAdv);
}


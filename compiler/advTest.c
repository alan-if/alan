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
  unitAssert(lastEcode == 211);
  unitAssert(lmSeverity() == sevERR);


  lastEcode = 0;
  lastSev = 0;
  adv.whr = newwhr(&srcp, WHR_AT,
		   newwht(&srcp, WHT_ID,
			  newId(&srcp, "atL")));
  analyzeStartAt();		/* Can not Start At unknown Id */
  unitAssert(lmSeverity() == sevERR);
  unitAssert(lastEcode == 310);
}


void registerAdvUnitTests()
{
  registerUnitTest(testInitAdv);
}


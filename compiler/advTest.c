/*======================================================================*\

  advTest.c

  Unit tests for ADV node in the Alan compiler

\*======================================================================*/

#include "adv.c"
#include "wht_x.h"

void testInitAdv()
{
  Srcp srcp = {2,3,4};

  IdNode *atUnknownId = newId(&srcp, "atUnknownId");
  IdNode *atInsId = newId(&srcp, "atInsId");
  IdNode *atInsLocId = newId(&srcp, "atInsLocId");
  IdNode *atClaId = newId(&srcp, "atClaId");

  Instance *ins;
  Instance *locationIns;
  ClaNod *cla;


  initAdventure();

  cla = newClass(&srcp, atClaId, NULL, NULL);
  ins = newInstance(&srcp, atInsId, NULL, NULL);
  locationIns = newInstance(&srcp, atInsLocId, newId(&srcp, "location"), NULL);

  adv.whr = newwhr(&srcp, WHR_HERE, NULL);
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Here */
  unitAssert(readEcode() == 211 && readSev() == sevERR);

  adv.whr = newwhr(&srcp, WHERE_AT,
		   newWhat(&srcp, WHAT_ID,
			   atUnknownId));
  symbolizeAdventure();
  unitAssert(readSev() == sevERR && readEcode() == 310);
  analyzeStartAt();		/* Can not Start At unknown Id */
  unitAssert(readSev() == sevERR && readEcode() == 351);

  adv.whr->what->id = atClaId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not an instance */
  unitAssert(readSev() == sevERR && readEcode() == 351);
 
  adv.whr->what->id = atInsId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not inheriting from location */
  unitAssert(readSev() == sevERR && readEcode() == 351);

  adv.whr->what->id = atInsLocId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not a instance */
  unitAssert(readSev() == sevNONE && readEcode() == 0);
}


void registerAdvUnitTests()
{
  registerUnitTest(testInitAdv);
}


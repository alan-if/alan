/*======================================================================*\

  advTest.c

  Unit tests for ADV node in the Alan compiler

\*======================================================================*/

#include "adv.c"

void testInitAdv()
{
  Srcp srcp = {2,3,4};

  IdNode *atUnknownId = newId(&srcp, "atUnknownId");
  IdNode *atInsId = newId(&srcp, "atInsId");
  IdNode *atInsLocId = newId(&srcp, "atInsLocId");
  IdNode *atClaId = newId(&srcp, "atClaId");

  InsNod *ins;
  InsNod *locationIns;
  ClaNod *cla;


  initadv();

  cla = newcla(&srcp, atClaId, NULL, NULL);
  ins = newins(&srcp, atInsId, NULL, NULL);
  locationIns = newins(&srcp, atInsLocId, newId(&srcp, "location"), NULL);

  adv.whr = newwhr(&srcp, WHR_HERE, NULL);
  symbolizeAdv();
  analyzeStartAt();		/* Can not Start At Here */
  unitAssert(readEcode() == 211 && readSev() == sevERR);

  adv.whr = newwhr(&srcp, WHR_AT,
		   newwht(&srcp, WHT_ID,
			  atUnknownId));
  symbolizeAdv();
  unitAssert(readSev() == sevERR && readEcode() == 310);
  analyzeStartAt();		/* Can not Start At unknown Id */
  unitAssert(readSev() == sevERR && readEcode() == 351);

  adv.whr->wht->id = atClaId;
  symbolizeAdv();
  analyzeStartAt();		/* Can not Start At Id not an instance */
  unitAssert(readSev() == sevERR && readEcode() == 351);
 
  adv.whr->wht->id = atInsId;
  symbolizeAdv();
  analyzeStartAt();		/* Can not Start At Id not inheriting from location */
  unitAssert(readSev() == sevERR && readEcode() == 351);

  adv.whr->wht->id = atInsLocId;
  symbolizeAdv();
  analyzeStartAt();		/* Can not Start At Id not a instance */
  unitAssert(readSev() == sevNONE && readEcode() == 0);
}


void registerAdvUnitTests()
{
  registerUnitTest(testInitAdv);
}


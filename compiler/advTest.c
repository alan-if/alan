/*======================================================================*\

  advTest.c

  Unit tests for ADV node in the Alan compiler

\*======================================================================*/

#include "adv.c"
#include "wht_x.h"
#include "exp_x.h"

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

  adv.whr = newWhere(&srcp, WHR_HERE, NULL);
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Here */
  ASSERT(readEcode() == 211 && readSev() == sevERR);

  adv.whr = newWhere(&srcp, WHERE_AT,
		     newWhatExpression(srcp, newWhat(&srcp, WHAT_ID,
						      atUnknownId)));
  symbolizeAdventure();
  ASSERT(readSev() == sevERR && readEcode() == 310);
  analyzeStartAt();		/* Can not Start At unknown Id */
  ASSERT(readSev() == sevERR && readEcode() == 351);

  adv.whr->what->fields.wht.wht->id = atClaId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not an instance */
  ASSERT(readSev() == sevERR && readEcode() == 351);
 
  adv.whr->what->fields.wht.wht->id = atInsId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not inheriting from location */
  ASSERT(readSev() == sevERR && readEcode() == 351);

  adv.whr->what->fields.wht.wht->id = atInsLocId;
  symbolizeAdventure();
  analyzeStartAt();		/* Can not Start At Id not a instance */
  ASSERT(readSev() == sevNONE && readEcode() == 0);
}


void registerAdvUnitTests()
{
  registerUnitTest(testInitAdv);
}


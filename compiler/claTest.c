/*======================================================================*\

  claTest.c

\*======================================================================*/

#include "cla.c"


void testCreateCla()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "claId");
  IdNode *heritage = newId(&srcp, "heritageId");

  /* Create a class with unkown inheritance */
  ClaNod *cla = newcla(&srcp, id, heritage, NULL);

  unitAssert(equalSrcp(cla->srcp, srcp));
  unitAssert(equalId(cla->id, id));
  unitAssert(equalId(cla->heritage, heritage));

  symbolizeClasses();
  unitAssert(lastEcode == 350 && lastSev == sevERR);
}


void registerClaUnitTests()
{
  registerUnitTest(testCreateCla);
}


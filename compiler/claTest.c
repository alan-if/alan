/*======================================================================*\

  claTest.c

\*======================================================================*/

#include "cla.c"

#include "ins_x.h"


void testCreateCla()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "claId");
  IdNode *heritage = newId(&srcp, "heritageId");
  InsNod *ins;

  /* Create a class with unknown inheritance */
  ClaNod *cla = newcla(&srcp, id, heritage, NULL);

  unitAssert(equalSrcp(cla->srcp, srcp));
  unitAssert(equalId(cla->id, id));
  unitAssert(equalId(cla->heritage, heritage));

  symbolizeClasses();
  unitAssert(readEcode() == 310 && readSev() == sevERR);

  /* Add the inheritance id, resymbolize */
  ins = newins(&srcp, heritage, NULL, NULL);
  symbolizeClasses();
  unitAssert(readEcode() == 350 && readSev() == sevERR);
}


void registerClaUnitTests()
{
  registerUnitTest(testCreateCla);
}


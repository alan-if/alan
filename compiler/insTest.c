/*======================================================================*\

  insTest.c

\*======================================================================*/

#include "ins.c"


void testCreateIns()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "insId");
  IdNode *heritage = newId(&srcp, "heritageId");
  InsNod *ins;

  initadv();

  ins = newins(&srcp, id, heritage, NULL);
  unitAssert(equalSrcp(srcp, ins->srcp));
  unitAssert(equalId(id, ins->id));
  unitAssert(equalId(heritage, ins->heritage));

  symbolizeInstance(ins);
  unitAssert(readEcode() == 310 && readSev() == sevERR);
}

void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
}



/*======================================================================*\

  claTest.c

\*======================================================================*/

#include "cla.c"
#include "adv.h"
#include "ins_x.h"
#include "emit.h"


void testCreateClass()
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


void testGenerateClasses()
{
  Srcp srcp = {12,13,14};
  ClaNod *cla;
  Aaddr addr;
  int firstAdr = sizeof(AcdHdr)/sizeof(Aword);

  initadv();

  addr = generateClasses();
  unitAssert(addr == 0);	/* Nothing generated */

  initEmit("unit.acd");
  cla = newcla(&srcp, newId(&srcp, "aSimpleClass"), NULL, NULL);
  addr = generateClasses();
  unitAssert(addr == firstAdr);	/* Should start at first address after header*/
  unitAssert(emadr() == firstAdr + 1);	/* The size of the class table and one class */
}

void registerClaUnitTests()
{
  registerUnitTest(testCreateClass);
  registerUnitTest(testGenerateClasses);
}


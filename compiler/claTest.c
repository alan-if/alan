/*======================================================================*\

  claTest.c

\*======================================================================*/

#include "cla.c"
#include "adv.h"
#include "ins_x.h"
#include "emit.h"
#include "../interpreter/types.h"


void testCreateClass()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "claId");
  IdNode *parent = newId(&srcp, "parentId");
  InsNod *ins;

  /* Create a class with unknown inheritance */
  ClaNod *cla = newcla(&srcp, id, parent, NULL);

  unitAssert(equalSrcp(cla->srcp, srcp));
  unitAssert(equalId(cla->slt->id, id));
  unitAssert(equalId(cla->parent, parent));

  symbolizeClasses();
  unitAssert(readEcode() == 310 && readSev() == sevERR);

  /* Add the inheritance id, resymbolize */
  ins = newInstance(&srcp, parent, NULL, NULL);
  symbolizeClasses();
  unitAssert(readEcode() == 350 && readSev() == sevERR);
}


void testGenerateClasses()
{
  Srcp srcp = {12,13,14};
  ClaNod *cla;
  Aaddr addr;
  int firstAdr = sizeof(AcdHdr)/sizeof(Aword);
  int classSize = sizeof(ClassEntry)/sizeof(Aword);

  initadv();

  initEmit("unit.acd");
  symbolizeClasses();
  addr = generateClasses();
  /* Table should start directly after header */
  unitAssert(addr == firstAdr);
  /* header + 4 classes + 1 EOF should be generated*/
  unitAssert(emadr() == firstAdr + 4*classSize + 1);

  initEmit("unit.acd");
  symbolizeClasses();
  cla = newcla(&srcp, newId(&srcp, "aSimpleClass"), NULL, NULL);
  addr = generateClasses();
  unitAssert(addr == firstAdr);	/* Should start at first address after header */
  unitAssert(emadr() == firstAdr + 5*classSize + 1);	/* 5 classes + EOF */
}

void registerClaUnitTests()
{
  registerUnitTest(testCreateClass);
  registerUnitTest(testGenerateClasses);
}


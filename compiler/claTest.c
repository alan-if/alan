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
  unitAssert(equalId(cla->id, id));
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
  addr = generateClasses();
  unitAssert(addr == firstAdr);	/* Nothing generated except header*/

  initEmit("unit.acd");
  symbolizeClasses();
  cla = newcla(&srcp, newId(&srcp, "aSimpleClass"), NULL, NULL);
  addr = generateClasses();
  unitAssert(addr == firstAdr+classSize);	/* Should start at first address after header and the class entry */
  unitAssert(emadr() == firstAdr + classSize + 1);	/* The size of the class table and one class */
}

void registerClaUnitTests()
{
  registerUnitTest(testCreateClass);
  registerUnitTest(testGenerateClasses);
}


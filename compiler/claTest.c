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
  ClaNod *cla = newClass(&srcp, id, parent, NULL);

  unitAssert(equalSrcp(cla->srcp, srcp));
  unitAssert(equalId(cla->slots->id, id));
  unitAssert(equalId(cla->slots->parentId, parent));

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
  int firstAdr = ACDsizeOf(AcdHdr);
  int classSize = ACDsizeOf(ClassEntry);

  initAdventure();

  initEmit("unit.a3c");
  symbolizeAdventure();
  addr = generateClasses();
  /* Table should start directly after header */
  unitAssert(addr == firstAdr);
  /* header + 4 classes + 1 EOF should be generated*/
  unitAssert(emadr() == firstAdr + 4*classSize + 1);

  initEmit("unit.a3c");
  symbolizeClasses();
  cla = newClass(&srcp, newId(&srcp, "aSimpleClass"), NULL, NULL);
  addr = generateClasses();
  unitAssert(addr == firstAdr);	/* Should start at first address after header */
  unitAssert(emadr() == firstAdr + 5*classSize + 1);	/* 5 classes + EOF */
}

void testGenerateEmptyClassEntry()
{
  Slots *slots = newSlots(NULL, NULL,
			  NULL, NULL,
			  NULL,
			  &nulsrcp, NULL, &nulsrcp, NULL,
			  &nulsrcp, NULL, NULL, NULL, NULL);
  ClaNod *class = newClass(&nulsrcp, newId(&nulsrcp, "aClass"), NULL, slots);
  int entryAddress;
  ClassEntry *entry;
  
  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  generateClassSlotsData(class->slots);
  entryAddress = emadr();
  generateClassEntry(class);
  terminateEmit();
  emitHeader();

  loadACD("unit.a3c");
  entry = (ClassEntry *) &memory[entryAddress];
  unitAssert(convertFromACD(entry->description) == 0);
  unitAssert(convertFromACD(entry->parent) == 0);
}

void registerClaUnitTests()
{
  registerUnitTest(testCreateClass);
  registerUnitTest(testGenerateClasses);
  registerUnitTest(testGenerateEmptyClassEntry);
}


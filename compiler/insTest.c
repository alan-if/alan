/*======================================================================*\

  insTest.c

\*======================================================================*/

#include "ins.c"


void testCreateIns()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "insId");
  IdNode *parent = newId(&srcp, "parentId");
  InsNod *ins;

  initadv();

  ins = newInstance(&srcp, id, parent, NULL);
  unitAssert(equalSrcp(srcp, ins->srcp));
  unitAssert(equalId(id, ins->id));
  unitAssert(equalId(parent, ins->parent));

  symbolizeInstance(ins);
  unitAssert(readEcode() == 310 && readSev() == sevERR);
}


void testGenerateInstances()
{
  Srcp srcp = {12,13,14};
  InsNod *ins;
  Aaddr addr;
  Aaddr instanceTableAddress;
  InstanceEntry *instanceTable;
  int firstAdr = sizeof(AcdHdr)/sizeof(Aword);
  int instanceSize = sizeof(InstanceEntry)/sizeof(Aword);
  AcdHdr header;

  initadv();
  initEmit("unit.acd");
  symbolizeInstances();
  generateInstances(&header);
  unitAssert(header.instanceTableAddress == firstAdr + 2);	/* Only "hero" generated */
  addr = emadr();
  unitAssert(addr == firstAdr + 2 + 1*instanceSize + 1);
  unitAssert(header.theHero == 1);
  /* 4 Class entries and 1 instance entry */

  initadv();
  initEmit("unit.acd");
  ins = newInstance(&srcp, newId(&srcp, "aSimpleInstance"), NULL, NULL);
  symbolizeInstances();
  generateInstanceData(ins);

  instanceTableAddress = emadr();
  generateInstanceEntry(ins);

  /* End should be at the size of the table and one instance */
  addr = emadr();
  unitAssert(addr == instanceTableAddress + instanceSize);
  acdHeader.size = addr;
  terminateEmit();

  loadACD("unit.acd");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  unitAssert(convertFromACD(instanceTable->code) == ins->symbol->code);
  unitAssert(convertFromACD(instanceTable->idAddr) == ins->idAddr);
  unitAssert(convertFromACD(instanceTable->parent) == (ins->parent?ins->parent->symbol->code:0));
  unitAssert(convertFromACD(instanceTable->nams) == ins->slots->namsadr);
  unitAssert(convertFromACD(instanceTable->atrs) == ins->slots->atradr);
  unitAssert(convertFromACD(instanceTable->description) == ins->slots->dscradr);
  unitAssert(convertFromACD(instanceTable->ment) == ins->slots->mentadr);
  unitAssert(convertFromACD(instanceTable->art) == ins->slots->artadr);
  unitAssert(convertFromACD(instanceTable->exts) == ins->slots->extadr);
  unitAssert(convertFromACD(instanceTable->vrbs) == ins->slots->vrbadr);
}


void testHero()
{
  initadv();
  unitAssert(theHero->symbol->code == 1);
  symbolizeAdv();
  unitAssert(inheritsFrom(theHero->symbol, lookup("actor")));
}



void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
  registerUnitTest(testGenerateInstances);
  registerUnitTest(testHero);
}



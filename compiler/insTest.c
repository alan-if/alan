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

  ins = newins(&srcp, id, parent, NULL);
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
  addr = generateInstances();
  unitAssert(addr == firstAdr);	/* Nothing generated except header */

  initEmit("unit.acd");
  symbolizeInstances();
  ins = newins(&srcp, newId(&srcp, "aSimpleInstance"), NULL, NULL);
  generateInstanceData(ins);

  instanceTableAddress = emadr();
  generateInstanceEntry(ins);

  /* End should be at the size of the table and one instance */
  addr = emadr();
  unitAssert(addr == instanceTableAddress + instanceSize);
  header.size = addr;
  terminateEmit(&header);

  loadACD("unit.acd");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  unitAssert(convertFromACD(instanceTable->code) == ins->symbol->code);
  unitAssert(convertFromACD(instanceTable->idAddr) == ins->idAddr);
  unitAssert(convertFromACD(instanceTable->parent) == (ins->parent?ins->parent->symbol->code:0));
  unitAssert(convertFromACD(instanceTable->nams) == ins->slt->namsadr);
  unitAssert(convertFromACD(instanceTable->location) ==ins->slt->whrCode);
  unitAssert(convertFromACD(instanceTable->atrs) == ins->slt->atradr);
  unitAssert(convertFromACD(instanceTable->dscr) == ins->slt->dscradr);
  unitAssert(convertFromACD(instanceTable->ment) == ins->slt->mentadr);
  unitAssert(convertFromACD(instanceTable->art) == ins->slt->artadr);
  unitAssert(convertFromACD(instanceTable->exts) == ins->slt->extadr);
  unitAssert(convertFromACD(instanceTable->vrbs) == ins->slt->vrbadr);
}


void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
  registerUnitTest(testGenerateInstances);
}



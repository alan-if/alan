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
  unitAssert(equalId(id, ins->slots->id));
  unitAssert(equalId(parent, ins->slots->parentId));

  symbolizeInstance(ins);
  unitAssert(readEcode() == 310 && readSev() == sevERR);
}


void testGenerateInstances()
{
  Srcp srcp = {12,13,14};
  InsNod *ins;
  Aaddr address;
  Aaddr instanceTableAddress;
  InstanceEntry *instanceTable;
  int firstAdr = sizeof(AcdHdr)/sizeof(Aword);
  int instanceSize = sizeof(InstanceEntry)/sizeof(Aword);

  initadv();
  initEmit("unit.acd");
  symbolizeAdv();

  address = generateInstanceTable();
  unitAssert(address == firstAdr);
  address = emadr();
  unitAssert(address == firstAdr + 1/*EOF*/);

  initadv();
  initEmit("unit.acd");
  ins = newInstance(&srcp, newId(&srcp, "aSimpleInstance"), NULL, NULL);
  symbolizeInstances();
  generateInstanceData(ins);

  instanceTableAddress = emadr();
  generateInstanceEntry(ins);

  /* End should be at the size of the table and one instance */
  address = emadr();
  unitAssert(address == instanceTableAddress + instanceSize);
  acdHeader.size = address;
  terminateEmit();

  loadACD("unit.acd");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  unitAssert(convertFromACD(instanceTable->code) == ins->slots->id->symbol->code);
  unitAssert(convertFromACD(instanceTable->idAddress) == ins->slots->idAddress);
  unitAssert(convertFromACD(instanceTable->parentClass) == (ins->slots->parentId?ins->slots->parentId->symbol->code:0));
  unitAssert(convertFromACD(instanceTable->attributes) == ins->slots->attributeAddress);
  unitAssert(convertFromACD(instanceTable->description) == ins->slots->descriptionAddress);
  unitAssert(convertFromACD(instanceTable->mentioned) == ins->slots->mentionedAddress);
  unitAssert(convertFromACD(instanceTable->article) == ins->slots->artadr);
  unitAssert(convertFromACD(instanceTable->exits) == ins->slots->exitsAddress);
  unitAssert(convertFromACD(instanceTable->verbs) == ins->slots->verbsAddress);
}


void testHero()
{
  AcdHdr header;

  initadv();
  unitAssert(theHero == NULL);
  addHero();
  unitAssert(theHero != NULL);
  unitAssert(theHero->code != 0);
  symbolizeAdv();
  unitAssert(inheritsFrom(theHero, lookup("actor")));
  generateInstances(&header);
  unitAssert(header.theHero == 1);
}



void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
  registerUnitTest(testGenerateInstances);
  registerUnitTest(testHero);
}



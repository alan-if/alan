/*======================================================================*\

  insTest.c

\*======================================================================*/

#include "ins.c"


void testCreateIns()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "insId");
  IdNode *parent = newId(&srcp, "parentId");
  Instance *ins;

  initAdventure();

  ins = newInstance(&srcp, id, parent, NULL);
  unitAssert(equalSrcp(srcp, ins->srcp));
  unitAssert(equalId(id, ins->props->id));
  unitAssert(equalId(parent, ins->props->parentId));

  symbolizeInstance(ins);
  unitAssert(readEcode() == 310 && readSev() == sevERR);
}


void testGenerateEmptyInstanceEntry()
{
  Properties *props = newProps(NULL, NULL,
			       NULL, NULL,
			       &nulsrcp,
			       NULL, NULL, NULL, &nulsrcp, NULL,
			       &nulsrcp, NULL, NULL, NULL);
  Instance *instance = newInstance(&nulsrcp, newId(&nulsrcp, "aInstance"), NULL, props);
  int entryAddress;
  InstanceEntry *entry;
  
  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  generateInstancePropertiesData(instance->props);
  entryAddress = emadr();
  generateInstanceEntry(instance);
  terminateEmit();
  emitHeader();

  loadACD("unit.a3c");
  entry = (InstanceEntry *) &memory[entryAddress];
  unitAssert(convertFromACD(entry->description) == 0);
  unitAssert(convertFromACD(entry->parentClass) == 0);
}

void testGenerateInstances()
{
  Srcp srcp = {12,13,14};
  Instance *ins;
  Aaddr address;
  Aaddr instanceTableAddress;
  InstanceEntry *instanceTable;
  int firstAdr = sizeof(AcdHdr)/sizeof(Aword);
  int instanceSize = sizeof(InstanceEntry)/sizeof(Aword);

  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  address = generateInstanceTable();
  unitAssert(address == firstAdr);
  address = emadr();
  unitAssert(address == firstAdr + 1/*EOF*/);

  initAdventure();
  initEmit("unit.a3c");
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
  emitHeader();

  loadACD("unit.a3c");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  unitAssert(convertFromACD(instanceTable->code) == ins->props->id->symbol->code);
  unitAssert(convertFromACD(instanceTable->idAddress) == ins->props->idAddress);
  unitAssert(convertFromACD(instanceTable->parentClass) == (ins->props->parentId?ins->props->parentId->symbol->code:0));
  unitAssert(convertFromACD(instanceTable->attributes) == ins->props->attributeAddress);
  unitAssert(convertFromACD(instanceTable->description) == ins->props->descriptionAddress);
  unitAssert(convertFromACD(instanceTable->mentioned) == ins->props->mentionedAddress);
  unitAssert(convertFromACD(instanceTable->article) == ins->props->articleAddress);
  unitAssert(convertFromACD(instanceTable->exits) == ins->props->exitsAddress);
  unitAssert(convertFromACD(instanceTable->verbs) == ins->props->verbsAddress);
}


void testHero()
{
  AcdHdr header;

  initAdventure();
  unitAssert(theHero == NULL);
  addHero();
  unitAssert(theHero != NULL);
  unitAssert(theHero->code != 0);
  symbolizeAdventure();
  unitAssert(inheritsFrom(theHero, lookup("actor")));
  generateInstances(&header);
  unitAssert(header.theHero == 1);
}



void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
  registerUnitTest(testGenerateEmptyInstanceEntry);
  registerUnitTest(testGenerateInstances);
  registerUnitTest(testHero);
}



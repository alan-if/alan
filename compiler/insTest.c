/*======================================================================*\

  insTest.c

\*======================================================================*/

#include "ins.c"


void testCreateIns()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(srcp, "insId");
  IdNode *parent = newId(srcp, "parentId");
  Instance *ins;

  initAdventure();

  ins = newInstance(&srcp, id, parent, NULL);
  ASSERT(equalSrcp(srcp, ins->srcp));
  ASSERT(equalId(id, ins->props->id));
  ASSERT(equalId(parent, ins->props->parentId));

  symbolizeInstance(ins);
  ASSERT(readEcode() == 310 && readSev() == sevERR);
}


void testGenerateEmptyInstanceEntry()
{
  Properties *props = newProps(NULL, NULL, nulsrcp, NULL, NULL, NULL,
			       nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp,
			       NULL, FALSE, nulsrcp,
			       NULL, FALSE,
			       NULL, NULL, NULL);
  Instance *instance = newInstance(&nulsrcp, newId(nulsrcp, "aInstance"), NULL, props);
  int entryAddress;
  InstanceEntry *entry;
  
  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  generateInstancePropertiesData(instance->props);
  entryAddress = nextEmitAddress();
  generateInstanceEntry(instance);
  terminateEmit();
  emitHeader();

  loadACD("unit.a3c");
  entry = (InstanceEntry *) &memory[entryAddress];
  ASSERT(convertFromACD(entry->description) == 0);
  ASSERT(convertFromACD(entry->parent) == 0);
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
  ASSERT(address == firstAdr);
  address = nextEmitAddress();
  ASSERT(address == firstAdr + 1/*EOF*/);

  initAdventure();
  initEmit("unit.a3c");
  ins = newInstance(&srcp, newId(srcp, "aSimpleInstance"), NULL, NULL);
  symbolizeInstances();
  generateInstanceData(ins);

  instanceTableAddress = nextEmitAddress();
  generateInstanceEntry(ins);

  /* End should be at the size of the table and one instance */
  address = nextEmitAddress();
  ASSERT(address == instanceTableAddress + instanceSize);
  acdHeader.size = address;
  terminateEmit();
  emitHeader();

  loadACD("unit.a3c");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  ASSERT(convertFromACD(instanceTable->code) == ins->props->id->symbol->code);
  ASSERT(convertFromACD(instanceTable->idAddress) == ins->props->idAddress);
  ASSERT(convertFromACD(instanceTable->parent) == (ins->props->parentId?ins->props->parentId->symbol->code:0));
  ASSERT(convertFromACD(instanceTable->initialAttributes) == ins->props->attributeAddress);
  ASSERT(convertFromACD(instanceTable->description) == ins->props->descriptionAddress);
  ASSERT(convertFromACD(instanceTable->mentioned) == ins->props->mentionedAddress);
  ASSERT(convertFromACD(instanceTable->indefinite) == ins->props->definiteAddress);
  ASSERT(convertFromACD(instanceTable->exits) == ins->props->exitsAddress);
  ASSERT(convertFromACD(instanceTable->verbs) == ins->props->verbsAddress);
}


void testHero()
{
  AcdHdr header;

  initAdventure();
  ASSERT(theHero == NULL);
  addHero();
  ASSERT(theHero != NULL);
  ASSERT(theHero->code != 0);
  symbolizeAdventure();
  ASSERT(inheritsFrom(theHero, lookup("actor")));
  generateInstances(&header);
  ASSERT(header.theHero == 1);
}



void registerInsUnitTests()
{
  registerUnitTest(testCreateIns);
  registerUnitTest(testGenerateEmptyInstanceEntry);
  registerUnitTest(testGenerateInstances);
  registerUnitTest(testHero);
}



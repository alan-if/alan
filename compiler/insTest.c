/*======================================================================*\

  insTest.c

\*======================================================================*/

#include "ins.c"

#include <cgreen/cgreen.h>

#include "unit.h"
#include "unitList.h"

#include "adv_x.h"


Ensure(testCreateIns) {
  Srcp srcp = {1,2,3};
  IdNode *id = newId(srcp, "insId");
  IdNode *parent = newId(srcp, "parentId");
  Instance *ins;

  initAdventure();

  ins = newInstance(&srcp, id, parent, NULL);
  assert_true(equalSrcp(srcp, ins->srcp));
  assert_true(equalId(id, ins->props->id));
  assert_true(equalId(parent, ins->props->parentId));

  symbolizeInstance(ins);
  assert_true(readEcode() == 310 && readSev() == sevERR);
}


Ensure(testGenerateEmptyInstanceEntry) {
  Properties *props = newProps(NULL, NULL,
			       nulsrcp, NULL,
			       NULL, NULL, NULL,
			       nulsrcp, NULL, NULL, NULL, NULL,
			       NULL, NULL,
			       nulsrcp, NULL,
			       NULL, NULL);
  Instance *instance = newInstance(&nulsrcp, newId(nulsrcp, "aInstance"), NULL, props);
  int entryAddress;
  InstanceEntry *entry;
  
  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  generateInstancePropertiesData(instance->props);
  entryAddress = nextEmitAddress();
  generateInstanceEntry(instance);
  finalizeEmit();
  writeHeader(&acodeHeader);
  terminateEmit();

  loadACD("unit.a3c");
  entry = (InstanceEntry *) &memory[entryAddress];
  assert_true(convertFromACD(entry->description) == 0);
  assert_true(convertFromACD(entry->parent) == 0);
}

Ensure(testGenerateInstances) {
  Srcp srcp = {12,13,14};
  Instance *ins;
  Aaddr address;
  Aaddr instanceTableAddress;
  InstanceEntry *instanceTable;
  int firstAdr = sizeof(ACodeHeader)/sizeof(Aword);
  int instanceSize = sizeof(InstanceEntry)/sizeof(Aword);

  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  address = generateInstanceTable();
  assert_true(address == firstAdr);
  address = nextEmitAddress();
  assert_true(address == firstAdr + instanceSize + 1/*EOF*/);

  initAdventure();
  initEmit("unit.a3c");
  ins = newInstance(&srcp, newId(srcp, "aSimpleInstance"), NULL, NULL);
  symbolizeInstances();
  generateInstanceData(ins);

  instanceTableAddress = nextEmitAddress();
  generateInstanceEntry(ins);

  /* End should be at the size of the table and one instance */
  address = nextEmitAddress();
  assert_true(address == instanceTableAddress + instanceSize);
  acodeHeader.size = address;
  finalizeEmit();
  writeHeader(&acodeHeader);
  terminateEmit();

  loadACD("unit.a3c");
  instanceTable = (InstanceEntry *) &memory[instanceTableAddress];
  assert_true(convertFromACD(instanceTable->code) == ins->props->id->symbol->code);
  assert_true(convertFromACD(instanceTable->id) == ins->props->idAddress);
  assert_true(convertFromACD(instanceTable->parent) == (ins->props->parentId?ins->props->parentId->symbol->code:0));
  assert_true(convertFromACD(instanceTable->initialAttributes) == ins->props->attributeAddress);
  assert_true(convertFromACD(instanceTable->checks) == checksAddressOf(ins->props->description));
  assert_true(convertFromACD(instanceTable->description) == doesAddressOf(ins->props->description));
  assert_true(convertFromACD(instanceTable->mentioned) == ins->props->mentionedAddress);
  assert_true(convertFromACD(instanceTable->exits) == ins->props->exitsAddress);
  assert_true(convertFromACD(instanceTable->verbs) == ins->props->verbsAddress);
}


Ensure(testHero) {
  ACodeHeader header;
  int count;
  Aword buffer[100];
  initEmitBuffer(buffer);

  assert_true(theHero == NULL);
  initAdventure();
  count = instanceCount;
  addHero();
  assert_true(theHero != NULL);
  assert_true(theHero->code != 0);
  symbolizeAdventure();
  assert_true(inheritsFrom(theHero, lookup("actor")));
  generateInstances(&header);
  assert_true(header.theHero == count+1);
}


Ensure(testNowhereIsGenerated) {
  nowhere = NULL;
  initAdventure();
  assert_true(nowhere != NULL);
  assert_true(nowhere->code == NOWHERE);
  symbolizeAdventure();
  assert_true(inheritsFrom(nowhere, lookup("location")));
}


TestSuite *insTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test(suite, testCreateIns);
    add_test(suite, testGenerateEmptyInstanceEntry);
    add_test(suite, testGenerateInstances);
    add_test(suite, testHero);
    add_test(suite, testNowhereIsGenerated);

    return suite;
}

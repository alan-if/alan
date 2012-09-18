/*======================================================================*\

  claTest.c

\*======================================================================*/

#include "cla.c"

#include <cgreen/cgreen.h>

#include "adv.h"
#include "ins_x.h"
#include "emit.h"

#include "unit.h"
#include "unitList.h"


Ensure(testCreateClass) {
  Srcp srcp = {1,2,3};
  IdNode *id = newId(srcp, "claId");
  IdNode *parent = newId(srcp, "parentId");
  Instance *ins;

  /* Create a class with unknown inheritance */
  Class *cla = newClass(&srcp, id, parent, NULL);

  assert_true(equalSrcp(cla->srcp, srcp));
  assert_true(equalId(cla->props->id, id));
  assert_true(equalId(cla->props->parentId, parent));

  symbolizeClasses();
  assert_true(readEcode() == 310 && readSev() == sevERR);

  /* Add the inheritance id, resymbolize */
  ins = newInstance(&srcp, parent, NULL, NULL);
  symbolizeClasses();
  assert_true(readEcode() == 350 && readSev() == sevERR);
}


Ensure(testGenerateClasses) {
  Srcp srcp = {12,13,14};
  Class *cla;
  Aaddr addr;
  int firstAdr = AwordSizeOf(ACodeHeader);
  static int NOOFPREDEFEINEDCLASSES = 8;
  static int classSize = AwordSizeOf(ClassEntry);
  int baseAddress = firstAdr + NOOFPREDEFEINEDCLASSES*classSize;

  initAdventure();

  initEmit("unit.a3c");
  symbolizeAdventure();
  addr = generateClasses();
  /* Table should start directly after header */
  assert_true(addr == firstAdr);
  /* header + PREDEFINED classes + 1 EOF should be generated*/
  assert_true(nextEmitAddress() == baseAddress + 1);

  initEmit("unit.a3c");
  symbolizeClasses();
  cla = newClass(&srcp, newId(srcp, "aSimpleClass"), NULL, NULL);
  addr = generateClasses();
  assert_true(addr == firstAdr);	/* Should start at first address after header */
  assert_true(nextEmitAddress() == baseAddress + classSize + 1);	/* (predefined+1) classes + EOF */
}

Ensure(testGenerateEmptyClassEntry) {
  Properties *props = newProps(NULL,
			       NULL,
			       nulsrcp, NULL,
			       NULL,
			       NULL,
			       NULL,
			       nulsrcp, NULL, NULL, NULL, NULL,
			       NULL, NULL,
			       nulsrcp, NULL,
			       NULL,
			       NULL);
  Class *class = newClass(&nulsrcp, newId(nulsrcp, "aClass"), NULL, props);
  int entryAddress;
  ClassEntry *entry;
  
  initAdventure();
  initEmit("unit.a3c");
  symbolizeAdventure();

  generateCommonPropertiesData(class->props);
  entryAddress = nextEmitAddress();
  generateClassEntry(class);
  finalizeEmit();
  writeHeader(&acodeHeader);
  terminateEmit();

  loadACD("unit.a3c");
  entry = (ClassEntry *) &memory[entryAddress];
  assert_true(convertFromACD(entry->description) == 0);
  assert_true(convertFromACD(entry->parent) == 0);
}

TestSuite *claTests()
{
    TestSuite *suite = create_test_suite();

    add_test(suite, testCreateClass);
    add_test(suite, testGenerateClasses);
    add_test(suite, testGenerateEmptyClassEntry);

    return suite;
}


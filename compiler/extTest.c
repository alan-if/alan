/*======================================================================*\

  extTest.c

  Unit tests for EXT node in the Alan compiler

\*======================================================================*/

#include "ext.c"

#include <cgreen/cgreen.h>

#include "unitList.h"

#include "context_x.h"
#include "adv_x.h"


Ensure(testNewExt) {
  IdNode *direction = newId(nulsrcp, "w");
  IdNode *targetLocation = newId(nulsrcp, "aLocation");
  Exit *theExit;
  Context *context = pushContext(NULL);
  IdNode *aLocationId = newId(nulsrcp, "aLocation");
  Symbol *aLocationSymbol;
  int firstAddress;
  int entrySize = AwordSizeOf(ExitEntry);

  initAdventure();
  (void) readEcode();
  aLocationSymbol = newSymbol(aLocationId, INSTANCE_SYMBOL);

  theExit = newExit(&nulsrcp, newList(direction, EXIT_LIST), targetLocation, NULL, NULL);
  assert_true(theExit->directions->member.id->symbol != NULL && theExit->directions->member.id->symbol->code == 1);

  symbolizeExit(theExit);
  assert_true(readEcode() == 0);

  analyzeExit(theExit, context);
  assert_true(readEcode() == 351);

  initEmit("unit.a3c");
  firstAddress = nextEmitAddress();
  generateExitEntry(theExit);
  assert_true(nextEmitAddress() == firstAddress + entrySize);
}


Ensure(testHaveExit) {
  List *exits = concat(newList(newExit(&nulsrcp,
				       newIdList(newIdList(NULL, "south"), "north"),
				       NULL, NULL, NULL), EXIT_LIST),
		       newExit(&nulsrcp,
			       newIdList(newIdList(NULL, "east"), "west"),
			       NULL, NULL, NULL), EXIT_LIST);

  
  assert_true(haveExit(exits, newId(nulsrcp, "south")));
  assert_true(haveExit(exits, newId(nulsrcp, "north")));
  assert_true(haveExit(exits, newId(nulsrcp, "east")));
  assert_true(haveExit(exits, newId(nulsrcp, "west")));
}

TestSuite *extTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test(suite, testNewExt);
    add_test(suite, testHaveExit);

    return suite;
}


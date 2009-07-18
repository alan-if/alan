/*======================================================================*\

  extTest.c

  Unit tests for EXT node in the Alan compiler

\*======================================================================*/

#include "ext.c"

#include "unit.h"
#include "unitList.h"

#include "context_x.h"
#include "adv_x.h"


void testNewExt()
{
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
  ASSERT(theExit->directions->member.id->symbol != NULL && theExit->directions->member.id->symbol->code == 1);

  symbolizeExit(theExit);
  ASSERT(readEcode() == 0);

  analyzeExit(theExit, context);
  ASSERT(readEcode() == 351);

  initEmit("unit.a3c");
  firstAddress = nextEmitAddress();
  generateExitEntry(theExit);
  ASSERT(nextEmitAddress() == firstAddress + entrySize);
}


void testHaveExit()
{
  List *exits = concat(newList(newExit(&nulsrcp,
				       newIdList(newIdList(NULL, "south"), "north"),
				       NULL, NULL, NULL), EXIT_LIST),
		       newExit(&nulsrcp,
			       newIdList(newIdList(NULL, "east"), "west"),
			       NULL, NULL, NULL), EXIT_LIST);

  
  ASSERT(haveExit(exits, newId(nulsrcp, "south")));
  ASSERT(haveExit(exits, newId(nulsrcp, "north")));
  ASSERT(haveExit(exits, newId(nulsrcp, "east")));
  ASSERT(haveExit(exits, newId(nulsrcp, "west")));
}

void extUnitTests()
{
  registerUnitTest(testNewExt);
  registerUnitTest(testHaveExit);
}


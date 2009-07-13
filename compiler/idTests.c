/*======================================================================*\

  idTest.c

  A unit test module for ID nodes in the Alan compiler

\*======================================================================*/

#include "id.c"

#include "unit.h"


void testGeneratedId() {
  IdNode *id1 = NEW(IdNode);
  IdNode *id2 = NEW(IdNode);
  id1->string = generateIdName();
  id2->string = generateIdName();

  ASSERT(isGeneratedId(id1));
  ASSERT(isGeneratedId(id2));
  ASSERT(!equalId(id1, id2));
}

void idUnitTests()
{
  registerUnitTest(testGeneratedId);
}


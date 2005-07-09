/*======================================================================*\

  elmTest.c

  Unit tests for ELM node in the Alan compiler

\*======================================================================*/

#include "elm.c"

void testPartition()
{
  List *p = NULL;
  Element *e = newEndOfSyntax();
  List *ep = concat(NULL, e, ELEMENT_LIST);
  List *epp = concat(NULL, ep, LIST_LIST);

  ASSERT(partition(&p) == NULL);
  ASSERT(partition(&epp) == NULL);
}


void registerElmUnitTests()
{
  registerUnitTest(testPartition);
}


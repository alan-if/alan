/*======================================================================*\

  resTest.c

  Unit tests for RES node in the Alan compiler

\*======================================================================*/

#include "res.c"

void testHasRestriction()
{
  Symbol *verbSymbol = newVerbSymbol(newId(nulsrcp, "verb"));
  IdNode *parameterId1 = newId(nulsrcp, "p1");
  IdNode *parameterId2 = newId(nulsrcp, "p2");
  Element *element1 = newParameterElement(nulsrcp, parameterId1, 0);
  Element *element2 = newParameterElement(nulsrcp, parameterId2, 0);
  List *elementList = concat(concat(NULL, element1, ELEMENT_LIST), element2, ELEMENT_LIST);
  Restriction *res1 = newRestriction(nulsrcp, parameterId1, ID_RESTRICTION, newId(nulsrcp, "resclas"), NULL);
  List *restrictions = concat(NULL, res1, RESTRICTION_LIST);
  Syntax *syntax = newSyntax(nulsrcp, NULL, NULL, restrictions, nulsrcp);

  setParameters(verbSymbol, elementList);
  ASSERT(hasRestriction(parameterId1->symbol, syntax));
  ASSERT(!hasRestriction(parameterId2->symbol, syntax));
}


void registerResUnitTests()
{
  registerUnitTest(testHasRestriction);
}


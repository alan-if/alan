/*======================================================================*\

  resTest.c

  Unit tests for Restriction Nodes in the Alan compiler

\*======================================================================*/

#include "res.c"

void testHasRestriction()
{
  Symbol *verbSymbol = newVerbSymbol(newId(nulsrcp, "verb"));

  IdNode *parameterId1 = newId(nulsrcp, "p1");
  IdNode *parameterId2 = newId(nulsrcp, "p2");

  Restriction *res1 = newRestriction(nulsrcp, parameterId1, ID_RESTRICTION, newId(nulsrcp, "resclas"), NULL);
  List *restrictions = concat(NULL, res1, RESTRICTION_LIST);
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, restrictions, nulsrcp);

  addElement(syntax, newParameterElement(nulsrcp, parameterId1, 0));
  addElement(syntax, newParameterElement(nulsrcp, parameterId2, 0));

  setParameters(verbSymbol, syntax->elements);

  ASSERT(hasRestriction(parameterId1->symbol, syntax));
  ASSERT(!hasRestriction(parameterId2->symbol, syntax));
}


void registerResUnitTests()
{
  registerUnitTest(testHasRestriction);
}


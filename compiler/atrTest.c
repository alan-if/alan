/*======================================================================*\

  atrTest.c

  Unit tests for ATR node in the Alan compiler

\*======================================================================*/

#include "atr.c"

void testMultipleAtr()
{
  List *attributeList;

  attributeList = concat(NULL, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, "anAttr"), 0, 0, 0), LIST_ATR);
  attributeList = concat(attributeList, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, "anAttr"), 0, 0, 0), LIST_ATR);

  readEcode();
  anatrs(attributeList);
  unitAssert(readEcode() == 218 && readSev() == sevERR);
}


void registerAtrUnitTests()
{
  registerUnitTest(testMultipleAtr);
}


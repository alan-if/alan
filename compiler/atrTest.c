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

static ClaNod *createClass(char string[], List *attributes)
{
  SlotsNode *slots = newEmptySlots();
  ClaNod *theClass;

  slots->attributes = attributes;
  theClass = newcla(&nulsrcp, newId(&nulsrcp, string), NULL, slots);
  return theClass;
}

static List *create2Attributes(char firstString[], char secondString[])
{
  List *theList;

  theList = concat(NULL, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, firstString), 0, 0, 0), LIST_ATR);
  theList = concat(theList, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, secondString), 0, 0, 0), LIST_ATR);
  return theList;
}

void testAttributeListsInSymbolTable()
{
  ClaNod *firstClass, *secondClass;
  List *firstAttributeList, *secondAttributeList;
  SymNod *firstClassSymbol, *secondClassSymbol;

  firstAttributeList = create2Attributes("attribute11", "attribute12");
  secondAttributeList = create2Attributes("attribute21", "attribute22");

  firstClass = createClass("firstClass", firstAttributeList);
  secondClass = createClass("secondClass", secondAttributeList);

  firstClassSymbol = lookup("firstClass");
  unitAssert(firstClassSymbol->fields.cla.attributes == firstAttributeList);
  secondClassSymbol = lookup("secondClass");
  unitAssert(secondClassSymbol->fields.cla.attributes == secondAttributeList);
}


void registerAtrUnitTests()
{
  registerUnitTest(testMultipleAtr);
  registerUnitTest(testAttributeListsInSymbolTable);
}


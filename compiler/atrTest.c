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


void testFindInList()
{
  List *attributes = NULL;
  IdNode *id = newId(&nulsrcp, "theAttribute");
  AtrNod *theAttribute = newatr(&nulsrcp, TYPBOOL, id, 0, 0, 0);
  AtrNod *anotherAttribute = newatr(&nulsrcp, TYPBOOL,
				    newId(&nulsrcp, "another"),
				    0, 0, 0);

  /* Test empty list */
  unitAssert(findAttribute(attributes, id) == NULL);

  /* Test one element */
  attributes = concat(attributes, theAttribute, LIST_ATR);
  unitAssert(findAttribute(attributes, id) == theAttribute);

  /* Test last element */
  attributes = combine(concat(NULL, anotherAttribute, LIST_ATR), attributes);
  attributes = combine(concat(NULL, anotherAttribute, LIST_ATR), attributes);
  unitAssert(findAttribute(attributes, id) == theAttribute);

  /* Test in the middle */
  attributes = concat(attributes, anotherAttribute, LIST_ATR);
  unitAssert(findAttribute(attributes, id) == theAttribute);
}

static ClaNod *createClass(char string[], List *attributes)
{
  SlotsNode *slots = newEmptySlots();
  ClaNod *theClass;

  slots->attributes = attributes;
  theClass = newClass(&nulsrcp, newId(&nulsrcp, string), NULL, slots);
  return theClass;
}

static InsNod *createInstance(char string[], List *attributes)
{
  SlotsNode *slots = newEmptySlots();
  InsNod *theInstance;

  slots->attributes = attributes;
  theInstance = newInstance(&nulsrcp, newId(&nulsrcp, string), NULL, slots);
  return theInstance;
}

static List *create2Attributes(char firstString[], char secondString[])
{
  List *theList;

  theList = concat(NULL, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, firstString), 0, 0, 0), LIST_ATR);
  theList = concat(theList, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, secondString), 0, 0, 0), LIST_ATR);
  return theList;
}

static int firstAttributeCode(SlotsNode *slots)
{
  return slots->symbol->fields.claOrIns.attributes->element.atr->id->code;
}

static int secondAttributeCode(SlotsNode *slots)
{
  return slots->symbol->fields.claOrIns.attributes->next->element.atr->id->code;
}

static InsNod *firstInstance, *secondInstance;


static void numberAttributes(List *aList, int n1, int n2)
{
  aList->element.atr->id->code = n1;
  aList->next->element.atr->id->code = n2;
}


static Bool equalLists(List *list1, List *list2)
{
  List *t1 = list1;
  List *t2 = list2;

  while (t1 != NULL && t2 != NULL && t1->element.atr->id->code && t2->element.atr->id->code) {
    t1 = t1->next;
    t2 = t2->next;
  }
  return t1 == NULL && t2 == NULL;
}

void testCombineAttributes()
{
  List *ownList = create2Attributes("x", "y");
  List *inheritedList = create2Attributes("y", "z");
  List *theCombinedList;

  unitAssert(combineAttributes(NULL, NULL) == NULL);

  numberAttributes(ownList, 1, 2);
  numberAttributes(inheritedList, 2, 3);
  theCombinedList = combineAttributes(ownList, NULL);
  unitAssert(length(theCombinedList) == length(ownList));
  unitAssert(theCombinedList == ownList);

  theCombinedList = combineAttributes(NULL, inheritedList);
  unitAssert(length(theCombinedList) == length(inheritedList));
  unitAssert(equalLists(theCombinedList, inheritedList));

  theCombinedList = combineAttributes(ownList, inheritedList);
  unitAssert(length(theCombinedList) == 3);
}




void testAttributeListsInSymbolTable()
{
  ClaNod *firstClass, *secondClass;
  List *firstClassAttributes, *secondClassAttributes, *firstInstanceAttributes, *secondInstanceAttributes;
  SymNod *firstClassSymbol, *secondClassSymbol, *firstInstanceSymbol, *secondInstanceSymbol;
  int x, y, z;

  initadv();
  firstClassAttributes = create2Attributes("attribute1", "attribute12");
  secondClassAttributes = create2Attributes("attribute1", "attribute22");

  firstClass = createClass("firstClass", firstClassAttributes);
  secondClass = createClass("secondClass", secondClassAttributes);

  firstClassSymbol = lookup("firstClass");
  unitAssert(firstClassSymbol->fields.claOrIns.attributes == firstClassAttributes);
  secondClassSymbol = lookup("secondClass");
  unitAssert(secondClassSymbol->fields.claOrIns.attributes == secondClassAttributes);
  
  firstInstanceAttributes = create2Attributes("attribute11", "attribute12");
  secondInstanceAttributes = create2Attributes("attribute1", "attribute22");

  firstInstance = createInstance("firstInstance", firstInstanceAttributes);
  secondInstance = createInstance("secondInstance", secondInstanceAttributes);

  firstInstanceSymbol = lookup("firstInstance");
  unitAssert(firstInstanceSymbol->fields.claOrIns.attributes == firstInstanceAttributes);
  secondInstanceSymbol = lookup("secondInstance");
  unitAssert(secondInstanceSymbol->fields.claOrIns.attributes == secondInstanceAttributes);

  /* Now set up a class hierarchy:
  location
     !
     fC = a1 + a12 -----+
     !                  !  
     fI = a11 + a12	sC = a1 + a21
                        !
                        sI = a1 + a22
  */
  setParent(firstClassSymbol, location->slots->symbol);
  setParent(secondClassSymbol, firstClassSymbol);
  setParent(firstInstanceSymbol, firstClassSymbol);
  setParent(secondInstanceSymbol, secondClassSymbol);

  numberAllAttributes();

  unitAssert(firstClassAttributes->element.atr->id->code != 0);
  unitAssert(firstClassAttributes->next->element.atr->id->code != 0);
  unitAssert(secondClassAttributes->element.atr->id->code != 0);
  unitAssert(secondClassAttributes->next->element.atr->id->code != 0);
  unitAssert(firstInstanceAttributes->element.atr->id->code != 0);
  unitAssert(firstInstanceAttributes->next->element.atr->id->code != 0);
  unitAssert(secondInstanceAttributes->element.atr->id->code != 0);
  unitAssert(secondInstanceAttributes->next->element.atr->id->code != 0);

  unitAssert(firstAttributeCode(firstClass->slots) != secondAttributeCode(firstClass->slots));
  unitAssert(firstAttributeCode(secondClass->slots) != secondAttributeCode(secondClass->slots));
  unitAssert(firstAttributeCode(firstInstance->slots) != secondAttributeCode(firstInstance->slots));
  unitAssert(firstAttributeCode(secondInstance->slots) != secondAttributeCode(secondInstance->slots));

  x = firstAttributeCode(firstClass->slots);
  unitAssert(firstAttributeCode(secondClass->slots) == x);
  unitAssert(firstAttributeCode(secondInstance->slots) == x);

  y = secondAttributeCode(firstClass->slots);
  unitAssert(secondAttributeCode(firstInstance->slots) == y);

  z = secondAttributeCode(firstClass->slots);
  unitAssert(secondAttributeCode(firstInstance->slots) == z);
}


static void numberAttributes123(List *l)
{
  l->element.atr->id->code = 1;
  l->next->element.atr->id->code = 2;
  l->next->next->element.atr->id->code = 3;
}

static void numberAttributes321(List *l)
{
  l->element.atr->id->code = 3;
  l->next->element.atr->id->code = 2;
  l->next->next->element.atr->id->code = 1;
}

static void numberAttributes231(List *l)
{
  l->element.atr->id->code = 2;
  l->next->element.atr->id->code = 3;
  l->next->next->element.atr->id->code = 1;
}

static Bool attributesAreSorted(List *list)
{
  List *l;
  int previousCode = 0;

  for (l = list; l; l = l->next) {
    if (l->element.atr->id->code <= previousCode)
      return FALSE;
    previousCode = l->element.atr->id->code;
  }
  return TRUE;
}

void testSortAttributes()
{
  List *attributeList = concat(NULL, newatr(&nulsrcp, TYPBOOL, newId(&nulsrcp, "a"), 0, 0, 0), LIST_ATR);
  List *originalList = attributeList;

  unitAssert(sortAttributes(NULL) == NULL);
  unitAssert(sortAttributes(attributeList) == originalList);

  attributeList = combine(attributeList, create2Attributes("x", "y"));
  numberAttributes123(attributeList);
  attributeList = sortAttributes(attributeList);
  unitAssert(attributesAreSorted(attributeList));

  numberAttributes321(attributeList);
  attributeList = sortAttributes(attributeList);
  unitAssert(attributesAreSorted(attributeList));

  numberAttributes231(attributeList);
  attributeList = sortAttributes(attributeList);
  unitAssert(attributesAreSorted(attributeList));
}

void testGenerateAttributes()
{
  int attributeEntrySize = ACDsizeOf(AttributeEntry);
  int address;

  initEmit("unit.acd");

  /* firstInstance has
     1 local attribute
     1 inherited atribute which is locally redefined
     1 inherited
     = 3
     But without analysis where we link in all inherited attributes we will
     only generate 2.
  */
  address = generateAttributes(firstInstance->slots->attributes);
  unitAssert(emadr() == address + 2*attributeEntrySize + 1);

#ifdef Z
  /* After analysis we should find all three */
  replicateInheritedAttributes();
  address = generateAttributes(firstInstance->slots->attributes);
  unitAssert(emadr() == address + 3*attributeEntrySize + 1);
#endif
}

void registerAtrUnitTests()
{
  registerUnitTest(testMultipleAtr);
  registerUnitTest(testAttributeListsInSymbolTable);
  registerUnitTest(testSortAttributes);
  registerUnitTest(testCombineAttributes);
  registerUnitTest(testGenerateAttributes);
}


/*======================================================================*\

  atrTest.c

  Unit tests for ATR node in the Alan compiler

\*======================================================================*/

#include "atr.c"
#include "cla_x.h"
#include "prop_x.h"

void testMultipleAtr()
{
  List *attributeList;

  attributeList = concat(NULL, newAttribute(&nulsrcp, BOOLEAN_TYPE, newId(&nulsrcp, "anAttr"), 0, 0, 0, NULL), ATTRIBUTE_LIST);
  attributeList = concat(attributeList, newAttribute(&nulsrcp, BOOLEAN_TYPE, newId(&nulsrcp, "anAttr"), 0, 0, 0, NULL), ATTRIBUTE_LIST);

  readEcode();
  checkMultipleAttributes(attributeList);
  ASSERT(readEcode() == 218 && readSev() == sevERR);
}


void testFindInList()
{
  List *attributes = NULL;
  IdNode *id = newId(&nulsrcp, "theAttribute");
  Attribute *theAttribute = newAttribute(&nulsrcp, BOOLEAN_TYPE, id, 0, 0, 0, NULL);
  Attribute *anotherAttribute = newAttribute(&nulsrcp, BOOLEAN_TYPE,
				    newId(&nulsrcp, "another"),
				    0, 0, 0, NULL);

  /* Test empty list */
  ASSERT(findAttribute(attributes, id) == NULL);

  /* Test one element */
  attributes = concat(attributes, theAttribute, ATTRIBUTE_LIST);
  ASSERT(findAttribute(attributes, id) == theAttribute);

  /* Test last element */
  attributes = combine(concat(NULL, anotherAttribute, ATTRIBUTE_LIST), attributes);
  attributes = combine(concat(NULL, anotherAttribute, ATTRIBUTE_LIST), attributes);
  ASSERT(findAttribute(attributes, id) == theAttribute);

  /* Test in the middle */
  attributes = concat(attributes, anotherAttribute, ATTRIBUTE_LIST);
  ASSERT(findAttribute(attributes, id) == theAttribute);
}

static ClaNod *createClass(char string[], List *attributes)
{
  Properties *props = newEmptyProps();
  ClaNod *theClass;

  props->attributes = attributes;
  theClass = newClass(&nulsrcp, newId(&nulsrcp, string), NULL, props);
  return theClass;
}

static Instance *createInstance(char string[], List *attributes)
{
  Properties *props = newEmptyProps();
  Instance *theInstance;

  props->attributes = attributes;
  theInstance = newInstance(&nulsrcp, newId(&nulsrcp, string), NULL, props);
  return theInstance;
}

static List *create2Attributes(char firstString[], char secondString[])
{
  List *theList;

  theList = concat(NULL, newAttribute(&nulsrcp, BOOLEAN_TYPE, newId(&nulsrcp, firstString), 0, 0, 0, NULL), ATTRIBUTE_LIST);
  theList = concat(theList, newAttribute(&nulsrcp, BOOLEAN_TYPE, newId(&nulsrcp, secondString), 0, 0, 0, NULL), ATTRIBUTE_LIST);
  return theList;
}

static int attributeCode(Properties *props, char *string)
{
  Attribute *atr = findAttribute(props->attributes, newId(&nulsrcp, string));
  return atr->id->code;
}

static Instance *firstInstance, *secondInstance;


static void numberTheAttributes(List *aList, int n1, int n2)
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

  ASSERT(combineAttributes(NULL, NULL) == NULL);

  numberTheAttributes(ownList, 1, 2);
  numberTheAttributes(inheritedList, 2, 3);
  theCombinedList = combineAttributes(ownList, NULL);
  ASSERT(length(theCombinedList) == length(ownList));
  ASSERT(theCombinedList == ownList);

  theCombinedList = combineAttributes(NULL, inheritedList);
  ASSERT(length(theCombinedList) == length(inheritedList));
  ASSERT(equalLists(theCombinedList, inheritedList));

  theCombinedList = combineAttributes(ownList, inheritedList);
  ASSERT(length(theCombinedList) == 3);
}




void testAttributeListsInSymbolTable()
{
  ClaNod *firstClass, *secondClass;
  List *firstClassAttributes, *secondClassAttributes, *firstInstanceAttributes, *secondInstanceAttributes;
  Symbol *firstClassSymbol, *secondClassSymbol, *firstInstanceSymbol, *secondInstanceSymbol;
  int x, y, z;

  initAdventure();
  firstClassAttributes = create2Attributes("a1", "a12");
  secondClassAttributes = create2Attributes("a1", "a21");

  firstClass = createClass("firstClass", firstClassAttributes);
  secondClass = createClass("secondClass", secondClassAttributes);

  firstClassSymbol = lookup("firstClass");
  ASSERT(firstClassSymbol->fields.entity.props->attributes == firstClassAttributes);
  secondClassSymbol = lookup("secondClass");
  ASSERT(secondClassSymbol->fields.entity.props->attributes == secondClassAttributes);
  
  firstInstanceAttributes = create2Attributes("a11", "a12");
  secondInstanceAttributes = create2Attributes("a1", "a22");

  firstInstance = createInstance("firstInstance", firstInstanceAttributes);
  secondInstance = createInstance("secondInstance", secondInstanceAttributes);

  firstInstanceSymbol = lookup("firstInstance");
  ASSERT(firstInstanceSymbol->fields.entity.props->attributes == firstInstanceAttributes);
  secondInstanceSymbol = lookup("secondInstance");
  ASSERT(secondInstanceSymbol->fields.entity.props->attributes == secondInstanceAttributes);

  /* Now set up a class hierarchy:
  location
     !
     fC = a1 + a12 -----+
     !                  !  
     fI = a11 + a12	sC = a1 + a21
                        !
                        sI = a1 + a22
  */
  setParent(firstClassSymbol, location->props->id->symbol);
  setParent(secondClassSymbol, firstClassSymbol);
  setParent(firstInstanceSymbol, firstClassSymbol);
  setParent(secondInstanceSymbol, secondClassSymbol);

  numberAllAttributes();

  ASSERT(attributeCode(firstClass->props, "a1") != 0);
  ASSERT(attributeCode(firstClass->props, "a12") != 0);
  ASSERT(attributeCode(secondClass->props, "a1") != 0);
  ASSERT(attributeCode(secondClass->props, "a21") != 0);
  ASSERT(attributeCode(firstInstance->props, "a11") != 0);
  ASSERT(attributeCode(firstInstance->props, "a12") != 0);
  ASSERT(attributeCode(secondInstance->props, "a1") != 0);
  ASSERT(attributeCode(secondInstance->props, "a22") != 0);

  ASSERT(attributeCode(firstClass->props, "a1") != attributeCode(firstClass->props, "a12"));
  ASSERT(attributeCode(secondClass->props, "a1") != attributeCode(secondClass->props, "a21"));
  ASSERT(attributeCode(firstInstance->props, "a11") != attributeCode(firstInstance->props, "a12"));
  ASSERT(attributeCode(secondInstance->props, "a1") != attributeCode(secondInstance->props, "a22"));

  x = attributeCode(firstClass->props, "a1");
  ASSERT(attributeCode(secondClass->props, "a1") == x);
  ASSERT(attributeCode(secondInstance->props, "a1") == x);

  y = attributeCode(firstClass->props, "a12");
  ASSERT(attributeCode(firstInstance->props, "a12") == y);

  z = attributeCode(secondClass->props, "a21");
  ASSERT(attributeCode(secondInstance->props, "a22") != z);
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
  List *attributeList = concat(NULL, newAttribute(&nulsrcp, BOOLEAN_TYPE, newId(&nulsrcp, "a"), 0, 0, 0, NULL), ATTRIBUTE_LIST);
  List *originalList = attributeList;

  ASSERT(sortAttributes(NULL) == NULL);
  ASSERT(sortAttributes(attributeList) == originalList);

  attributeList = combine(attributeList, create2Attributes("x", "y"));
  numberAttributes123(attributeList);
  attributeList = sortAttributes(attributeList);
  ASSERT(attributesAreSorted(attributeList));

  numberAttributes321(attributeList);
  attributeList = sortAttributes(attributeList);
  ASSERT(attributesAreSorted(attributeList));

  numberAttributes231(attributeList);
  attributeList = sortAttributes(attributeList);
  ASSERT(attributesAreSorted(attributeList));
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
  address = generateAttributes(firstInstance->props->attributes);
  ASSERT(nextEmitAddress() == address + 2*attributeEntrySize + 1);
}

static void testResolveThisAttributeForClass()
{
  List *theAttributes = create2Attributes("x", "y");
  Properties *theProps = newProps(NULL, NULL, theAttributes, NULL,
				  nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp,
				  NULL, FALSE, nulsrcp,
				  NULL, FALSE,
				  NULL, NULL, NULL);
  ClaNod *theClass = newClass(&nulsrcp, newId(&nulsrcp, "aClass"), NULL, theProps);
  Context context = {CLASS_CONTEXT, NULL, NULL, NULL, theClass, NULL};
  Attribute *theResolvedAttribute;

  theResolvedAttribute = resolveAttributeOfThis(newId(&nulsrcp, "x"), &context);
  ASSERT(theResolvedAttribute == theAttributes->element.atr);
}

void registerAtrUnitTests()
{
  registerUnitTest(testMultipleAtr);
  registerUnitTest(testAttributeListsInSymbolTable);
  registerUnitTest(testSortAttributes);
  registerUnitTest(testCombineAttributes);
  registerUnitTest(testGenerateAttributes);
  registerUnitTest(testResolveThisAttributeForClass);
}

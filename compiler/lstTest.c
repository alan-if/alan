/*======================================================================*\

  lstTest.c

  Unit tests for List node in the Alan compiler

\*======================================================================*/

#include "lst.c"
#include "id_x.h"


void testLength()
{
  List *aList = NULL;

  unitAssert(length(aList) == 0);

  aList = concat(aList, newId(&nulsrcp, "id1"), ID_LIST);
  unitAssert(length(aList) == 1);

  aList = concat(aList, newId(&nulsrcp, "id2"), ID_LIST);
  unitAssert(length(aList) == 2);

  aList = concat(aList, newId(&nulsrcp, "id3"), ID_LIST);
  unitAssert(length(aList) == 3);
}

void testInsert()
{
  IdNode *anElement = newId(&nulsrcp, "anElement");
  List *aList = concat(NULL, anElement, ATTRIBUTE_LIST);

#ifdef SYSERR
  insert(NULL, NULL, LIST_ATR);
  unitAssert(readEcode() == 997 && readSev() == sevERR);

  insert(aList, anElement, LIST_ID);
  unitAssert(readEcode() == 997 && readSev() == sevERR);
#endif

  insert(aList, anElement, ATTRIBUTE_LIST);
  unitAssert(length(aList) == 2);
}

void testTailOf()
{
  List *listOfOne = concat(NULL, newId(&nulsrcp, "anId"), ID_LIST);
  List *listOfTwo = concat(concat(NULL, newId(&nulsrcp, "anId"), ID_LIST),
			   newId(&nulsrcp, "anId"), ID_LIST);
			   
  unitAssert(tailOf(NULL) == NULL);
  unitAssert(tailOf(listOfOne) == listOfOne);
  unitAssert(tailOf(listOfTwo) == listOfTwo->next);
}

void registerLstUnitTests()
{
  registerUnitTest(testLength);
  registerUnitTest(testInsert);
  registerUnitTest(testTailOf);
}


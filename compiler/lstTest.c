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

  aList = concat(aList, newId(&nulsrcp, "id1"), LIST_ID);
  unitAssert(length(aList) == 1);

  aList = concat(aList, newId(&nulsrcp, "id2"), LIST_ID);
  unitAssert(length(aList) == 2);

  aList = concat(aList, newId(&nulsrcp, "id3"), LIST_ID);
  unitAssert(length(aList) == 3);
}

void testInsert()
{
  IdNode *anElement = newId(&nulsrcp, "anElement");
  List *aList = concat(NULL, anElement, LIST_ATR);

#ifdef SYSERR
  insert(NULL, NULL, LIST_ATR);
  unitAssert(readEcode() == 997 && readSev() == sevERR);

  insert(aList, anElement, LIST_ID);
  unitAssert(readEcode() == 997 && readSev() == sevERR);
#endif

  insert(aList, anElement, LIST_ATR);
  unitAssert(length(aList) == 2);
}

void registerLstUnitTests()
{
  registerUnitTest(testLength);
  registerUnitTest(testInsert);
}


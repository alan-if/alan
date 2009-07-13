/*======================================================================*\

  lstTest.c

  Unit tests for List node in the Alan compiler

\*======================================================================*/

#include "lst.c"
#include "id_x.h"

#include "unit.h"
#include <setjmp.h>

void testLength()
{
  List *aList = NULL;

  ASSERT(length(aList) == 0);

  // TODO newIdList(NULL, => newIdList( perhaps?
  aList = newIdList(NULL, "id1");
  ASSERT(length(aList) == 1);

  aList = concat(aList, "id2", ID_LIST);
  ASSERT(length(aList) == 2);

  aList = concat(aList, "id3", ID_LIST);
  ASSERT(length(aList) == 3);
}


void insertingShouldIncreaseLength()
{
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = concat(NULL, aMember, ID_LIST);
  ASSERT(length(aList) == 1);

  insert(aList, aMember, ID_LIST);
  ASSERT(length(aList) == 2);
}


/*----------------------------------------------------------------------*/
static Bool syserrHandlerCalled;

jmp_buf syserrLabel;


static void syserrHandler(char *message) {
  syserrHandlerCalled = TRUE;
  longjmp(syserrLabel, TRUE);
}

/* From util.c */
extern void setSyserrHandler(void (*f)(char *));

#define TRY(code)   \
  setSyserrHandler(syserrHandler); \
  syserrHandlerCalled = FALSE;\
  if (setjmp(syserrLabel) == 0) { \
    code \
  } \
  setSyserrHandler(NULL);

#define CATCH() \
  setSyserrHandler(NULL);
  

void insertingIntoANullListFails()
{
  IdNode *aMember = newId(nulsrcp, "aMember");

  TRY(
    insert(NULL, aMember, ID_LIST);
    ASSERT(FALSE);
  )
  ASSERT(syserrHandlerCalled);
}

void insertingANullMemberFails()
{
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = concat(NULL, aMember, ID_LIST);

  TRY(
    insert(aList, NULL, ID_LIST);
    ASSERT(FALSE);
  )
  ASSERT(syserrHandlerCalled);
}

void insertingWrongTypeOfElementFails()
{
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = newIdList(NULL, "aMember");

  TRY(
    insert(aList, aMember, ATTRIBUTE_LIST);
    ASSERT(FALSE);
  );
  ASSERT(syserrHandlerCalled);
}

void testTailOf()
{
  List *listOfOne = newIdList(NULL, "anId");
  List *listOfTwo = newIdList(newIdList(NULL, "anId"),
			      "anId");
			   
  ASSERT(getLastListNode(NULL) == NULL);
  ASSERT(getLastListNode(listOfOne) == listOfOne);
  ASSERT(getLastListNode(listOfTwo) == listOfTwo->next);
}

void testRemoveFromList()
{
  List element1;
  List element2;
  List element3;
  List *result;

  ASSERT(removeFromList(NULL, NULL) == NULL);

  element1.next = NULL;
  ASSERT(removeFromList(&element1, &element1) == NULL);

  element1.next = &element2;
  element2.next = NULL;
  result = removeFromList(&element1, &element1);
  ASSERT(result == &element2);
  ASSERT(element2.next == NULL);
  ASSERT(element1.next == NULL);

  element1.next = &element2;
  element2.next = &element3;
  element3.next = NULL;
  result = removeFromList(&element1, &element1);
  ASSERT(result == &element2);
  ASSERT(element2.next == &element3);
  ASSERT(element1.next == NULL);

  element1.next = &element2;
  element2.next = &element3;
  element3.next = NULL;
  result = removeFromList(&element1, &element2);
  ASSERT(result == &element1);
  ASSERT(element1.next == &element3);
  ASSERT(element2.next == NULL);
}


int sorter(List *element1, List *element2)
{
  if (element1->kind == element2->kind)
    return 0;
  else if (element1->kind < element2->kind)
    return -1;
  else
    return 1;
}


void testSortList()
{
  List element1;
  List element2;
  List element3;
  List *result;

  ASSERT(sortList(NULL, NULL) == NULL);

  element1.next = NULL;
  ASSERT(sortList(&element1, &sorter) == &element1);

  element1.kind = 1;
  element1.next = &element2;
  element2.kind = 3;
  result = sortList(&element1, &sorter);
  ASSERT(result->kind < result->next->kind);

  element1.kind = 2;
  element1.next = &element2;
  element2.kind = 1;
  result = sortList(&element1, &sorter);
  ASSERT(result->kind == 1);
  ASSERT(result->next->kind == 2);

  element1.kind = 2;
  element2.kind = 3;
  element3.kind = 1;
  element1.next = &element2;
  element2.next = &element3;
  element3.next = NULL;
  result = sortList(&element1, &sorter);
  ASSERT(result->kind == 1);
  ASSERT(result->next->kind == 2);
  ASSERT(result->next->next->kind == 3);
}


static void testCopyList() {
  List *l1 = newIdList(NULL, "id1");
  List *l4 = newIdList(newIdList(newIdList(newIdList(NULL, "id1"),
					   "id2"),
				 "id3"),
		       "id4");
  List *copy;
  int i;

  ASSERT(copyList(NULL) == NULL);

  copy = copyList(l1);
  ASSERT(l1->next == NULL);
  ASSERT(l1->member.id == copy->member.id);

  copy = copyList(l4);
  ASSERT(length(copy) == length(l4));
  for (i = 1; i<=length(copy); i++)
    ASSERT(getMember(copy, i) == getMember(l4, i));
}

void lstUnitTests()
{
  registerUnitTest(testLength);
  registerUnitTest(insertingShouldIncreaseLength);
  registerUnitTest(insertingIntoANullListFails);
  registerUnitTest(insertingANullMemberFails);
  registerUnitTest(insertingWrongTypeOfElementFails);
  registerUnitTest(testTailOf);
  registerUnitTest(testRemoveFromList);
  registerUnitTest(testSortList);
  registerUnitTest(testCopyList);
}


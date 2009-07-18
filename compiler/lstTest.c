/*======================================================================*\

  lstTest.c

  Unit tests for List node in the Alan compiler

\*======================================================================*/

#include "lst.c"
#include "id_x.h"

#include "unit.h"
#include <setjmp.h>

void canCreateNewEmptyListWithType() {
  List *list = newEmptyList(ID_LIST);
  ASSERT(list->kind == ID_LIST);
  ASSERT(list->member.lst == NULL);
}


void canConcatToANewEmptyList() {
  List *list = newEmptyList(ID_LIST);
  IdNode *theId = newId(nulsrcp, "theId");

  list = concat(list, theId, ID_LIST);
  ASSERT((IdNode *)list->member.lst == theId);
  ASSERT(list->next == NULL);
}


void canCreateNewListWithMember() {
  IdNode *theId = newId(nulsrcp, "theId");
  List *list = newList(theId, ID_LIST);
  ASSERT(list->kind == ID_LIST);
  ASSERT(list->member.ptr == theId);
}


void testLength()
{
  List *aList = NULL;

  ASSERT(length(aList) == 0);

  // TODO newIdList(NULL, "string") => newIdList("string)
  // and concatId(list, "string)
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
  List *aList = newList(aMember, ID_LIST);
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
  List *aList = newList(aMember, ID_LIST);

  TRY(
    insert(aList, NULL, ID_LIST);
    ASSERT(FALSE);
  )
  ASSERT(syserrHandlerCalled);
}

void insertingWrongTypeOfMemberFails()
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
  List member1;
  List member2;
  List member3;
  List *result;

  ASSERT(removeFromList(NULL, NULL) == NULL);

  member1.next = NULL;
  ASSERT(removeFromList(&member1, &member1) == NULL);

  member1.next = &member2;
  member2.next = NULL;
  result = removeFromList(&member1, &member1);
  ASSERT(result == &member2);
  ASSERT(member2.next == NULL);
  ASSERT(member1.next == NULL);

  member1.next = &member2;
  member2.next = &member3;
  member3.next = NULL;
  result = removeFromList(&member1, &member1);
  ASSERT(result == &member2);
  ASSERT(member2.next == &member3);
  ASSERT(member1.next == NULL);

  member1.next = &member2;
  member2.next = &member3;
  member3.next = NULL;
  result = removeFromList(&member1, &member2);
  ASSERT(result == &member1);
  ASSERT(member1.next == &member3);
  ASSERT(member2.next == NULL);
}


int sorter(List *member1, List *member2)
{
  if (member1->kind == member2->kind)
    return 0;
  else if (member1->kind < member2->kind)
    return -1;
  else
    return 1;
}


void testSortList()
{
  List member1;
  List member2;
  List member3;
  List *result;

  ASSERT(sortList(NULL, NULL) == NULL);

  member1.next = NULL;
  ASSERT(sortList(&member1, &sorter) == &member1);

  member1.kind = 1;
  member1.next = &member2;
  member2.kind = 3;
  result = sortList(&member1, &sorter);
  ASSERT(result->kind < result->next->kind);

  member1.kind = 2;
  member1.next = &member2;
  member2.kind = 1;
  result = sortList(&member1, &sorter);
  ASSERT(result->kind == 1);
  ASSERT(result->next->kind == 2);

  member1.kind = 2;
  member2.kind = 3;
  member3.kind = 1;
  member1.next = &member2;
  member2.next = &member3;
  member3.next = NULL;
  result = sortList(&member1, &sorter);
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
  registerUnitTest(canCreateNewEmptyListWithType);
  registerUnitTest(canConcatToANewEmptyList);
  registerUnitTest(canCreateNewListWithMember);
  registerUnitTest(testLength);
  registerUnitTest(insertingShouldIncreaseLength);
  registerUnitTest(insertingIntoANullListFails);
  registerUnitTest(insertingANullMemberFails);
  registerUnitTest(insertingWrongTypeOfMemberFails);
  registerUnitTest(testTailOf);
  registerUnitTest(testRemoveFromList);
  registerUnitTest(testSortList);
  registerUnitTest(testCopyList);
}


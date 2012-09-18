/*======================================================================*\

  lstTest.c

  Unit tests for List node in the Alan compiler

\*======================================================================*/

#include "lst.c"

#include <cgreen/cgreen.h>

#include "id_x.h"

#include "unit.h"
#include <setjmp.h>

Ensure(canCreateNewEmptyListWithType) {
  List *list = newEmptyList(ID_LIST);
  assert_true(list->kind == ID_LIST);
  assert_true(list->member.lst == NULL);
}


Ensure(canConcatToANewEmptyList) {
  List *list = newEmptyList(ID_LIST);
  IdNode *theId = newId(nulsrcp, "theId");

  list = concat(list, theId, ID_LIST);
  assert_true((IdNode *)list->member.lst == theId);
  assert_true(list->next == NULL);
}


Ensure(canCreateNewListWithMember) {
  IdNode *theId = newId(nulsrcp, "theId");
  List *list = newList(theId, ID_LIST);
  assert_true(list->kind == ID_LIST);
  assert_true(list->member.ptr == theId);
}


Ensure(testLength) {
  List *aList = NULL;

  assert_true(length(aList) == 0);

  // TODO newIdList(NULL, "string") => newIdList("string)
  // and concatId(list, "string)
  aList = newIdList(NULL, "id1");
  assert_true(length(aList) == 1);

  aList = concat(aList, "id2", ID_LIST);
  assert_true(length(aList) == 2);

  aList = concat(aList, "id3", ID_LIST);
  assert_true(length(aList) == 3);
}


Ensure(insertingShouldIncreaseLength) {
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = newList(aMember, ID_LIST);
  assert_true(length(aList) == 1);

  insert(aList, aMember, ID_LIST);
  assert_true(length(aList) == 2);
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
  

Ensure(insertingIntoANullListFails) {
  IdNode *aMember = newId(nulsrcp, "aMember");

  TRY(
    insert(NULL, aMember, ID_LIST);
    assert_true(FALSE);
  )
  assert_true(syserrHandlerCalled);
}

Ensure(insertingANullMemberFails) {
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = newList(aMember, ID_LIST);

  TRY(
    insert(aList, NULL, ID_LIST);
    assert_true(FALSE);
  )
  assert_true(syserrHandlerCalled);
}

Ensure(insertingWrongTypeOfMemberFails) {
  IdNode *aMember = newId(nulsrcp, "aMember");
  List *aList = newIdList(NULL, "aMember");

  TRY(
    insert(aList, aMember, ATTRIBUTE_LIST);
    assert_true(FALSE);
  );
  assert_true(syserrHandlerCalled);
}

Ensure(testTailOf) {
  List *listOfOne = newIdList(NULL, "anId");
  List *listOfTwo = newIdList(newIdList(NULL, "anId"),
			      "anId");
			   
  assert_true(getLastListNode(NULL) == NULL);
  assert_true(getLastListNode(listOfOne) == listOfOne);
  assert_true(getLastListNode(listOfTwo) == listOfTwo->next);
}

Ensure(testRemoveFromList) {
  List member1;
  List member2;
  List member3;
  List *result;

  assert_true(removeFromList(NULL, NULL) == NULL);

  member1.next = NULL;
  assert_true(removeFromList(&member1, &member1) == NULL);

  member1.next = &member2;
  member2.next = NULL;
  result = removeFromList(&member1, &member1);
  assert_true(result == &member2);
  assert_true(member2.next == NULL);
  assert_true(member1.next == NULL);

  member1.next = &member2;
  member2.next = &member3;
  member3.next = NULL;
  result = removeFromList(&member1, &member1);
  assert_true(result == &member2);
  assert_true(member2.next == &member3);
  assert_true(member1.next == NULL);

  member1.next = &member2;
  member2.next = &member3;
  member3.next = NULL;
  result = removeFromList(&member1, &member2);
  assert_true(result == &member1);
  assert_true(member1.next == &member3);
  assert_true(member2.next == NULL);
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


Ensure(testSortList) {
    List *member1 = newList(NULL, ID_LIST);
    List *member2 = newList(NULL, ID_LIST);
    List *member3 = newList(NULL, ID_LIST);
    List *result;

  assert_true(sortList(NULL, NULL) == NULL);

  member1->next = NULL;
  assert_true(sortList(member1, &sorter) == member1);

  member1->kind = 1;
  member1->next = member2;
  member2->kind = 3;
  result = sortList(member1, &sorter);
  assert_true(result->kind < result->next->kind);

  member1->kind = 2;
  member1->next = member2;
  member2->kind = 1;
  result = sortList(member1, &sorter);
  assert_true(result->kind == 1);
  assert_true(result->next->kind == 2);

  member1->kind = 2;
  member2->kind = 3;
  member3->kind = 1;
  member1->next = member2;
  member2->next = member3;
  member3->next = NULL;
  result = sortList(member1, &sorter);
  assert_true(result->kind == 1);
  assert_true(result->next->kind == 2);
  assert_true(result->next->next->kind == 3);
}


Ensure(testCopyList) {
  List *l1 = newIdList(NULL, "id1");
  List *l4 = newIdList(newIdList(newIdList(newIdList(NULL, "id1"),
					   "id2"),
				 "id3"),
		       "id4");
  List *copy;
  int i;

  assert_true(copyList(NULL) == NULL);

  copy = copyList(l1);
  assert_true(l1->next == NULL);
  assert_true(l1->member.id == copy->member.id);

  copy = copyList(l4);
  assert_true(length(copy) == length(l4));
  for (i = 1; i<=length(copy); i++)
    assert_true(getMember(copy, i) == getMember(l4, i));
}

TestSuite *lstTests()
{
    TestSuite *suite = create_test_suite(); 
    add_test(suite, canCreateNewEmptyListWithType);
    add_test(suite, canConcatToANewEmptyList);
    add_test(suite, canCreateNewListWithMember);
    add_test(suite, testLength);
    add_test(suite, insertingShouldIncreaseLength);
    add_test(suite, insertingIntoANullListFails);
    add_test(suite, insertingANullMemberFails);
    add_test(suite, insertingWrongTypeOfMemberFails);
    add_test(suite, testTailOf);
    add_test(suite, testRemoveFromList);
    add_test(suite, testSortList);
    add_test(suite, testCopyList);
    return suite;
}


/*======================================================================*\

  elmTest.c

  Unit tests for ELM node in the Alan compiler

\*======================================================================*/

#include "elm.c"

static void testEqualElements() {
  IdNode *id1 = newId(nulsrcp, "id1");
  IdNode *id2 = newId(nulsrcp, "id2");
  Element *e1 = newWordElement(nulsrcp, id1);
  Element *e2 = newWordElement(nulsrcp, id2);
  List *l1 = concat(NULL, e1, ELEMENT_LIST);
  List *l2 = concat(NULL, e2, ELEMENT_LIST);

  ASSERT(equalElements(l1, l2) == FALSE);

  e2 = newWordElement(nulsrcp, id1);
  l2 = concat(NULL, e2, ELEMENT_LIST);
  ASSERT(equalElements(l1, l2) == TRUE);

  e2->kind = END_OF_SYNTAX;
  ASSERT(equalElements(l1, l2) == FALSE);

  e2->kind = PARAMETER_ELEMENT;
  ASSERT(equalElements(l1, l2) == FALSE);

  ASSERT(equalElements(NULL, NULL) == TRUE);
  ASSERT(equalElements(NULL, l1) == FALSE);
  ASSERT(equalElements(l1, NULL) == FALSE);
}

static void testNewEntryForPartition() {
  List *entries = NULL;
  ElementEntry *entry;

  entry = newEntryForPartition(&entries);
  ASSERT(entries != NULL);
  ASSERT(entry != NULL);
  ASSERT(entry == (ElementEntry*)entries->element.eent);

  entry = newEntryForPartition(&entries);
  ASSERT(entry != NULL);
  ASSERT(entry != (ElementEntry*)entries->element.eent);
  ASSERT(entry == (ElementEntry*)entries->next->element.eent);
  ASSERT(length(entries) == 2);  
}


void registerElmUnitTests()
{
  registerUnitTest(testEqualElements);
  registerUnitTest(testNewEntryForPartition);
}

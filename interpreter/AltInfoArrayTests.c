#include "cgreen/cgreen.h"

#include "AltInfoArray.c"

/*----------------------------------------------------------------------*/
static void canDuplicateAltInfoArray() {
  AltInfo original[20];
  AltInfo *duplicate;
  static int COPYSIZE = 15;
  int i;

  for (i = 0; i < COPYSIZE; i++)
    original[i].end = FALSE;
  original[COPYSIZE].end = TRUE;

  duplicate = duplicateAltInfoArray(original);
  assert_true(duplicate[COPYSIZE].end);
  free(duplicate);
}


/*----------------------------------------------------------------------*/
static void canFindLastAltInfoIndex() {
  AltInfo array[20];
  static int SIZE = 13;
  int i;

  for (i = 0; i < SIZE; i++)
    array[i].end = FALSE;
  array[SIZE].end = TRUE;

  assert_equal(lastAltInfoIndex(array), SIZE-1);
}

/*----------------------------------------------------------------------*/
static void canFindNextFreeAltInfo() {
  AltInfo array[20];
  static int SIZE = 13;
  int i;

  for (i = 0; i < SIZE; i++)
    array[i].end = FALSE;
  array[SIZE].end = TRUE;

  assert_equal(nextFreeAltInfo(array), &array[SIZE]);
}

/*----------------------------------------------------------------------*/
AltEntry altEntry;

AltEntry *finder(Aint parameterNumber, Aint theInstance, Aint theClass) {
  return &altEntry;
}

static void canAddGlobalAlternatives() {
  AltInfo altInfos[10];
  altInfos[0].end = TRUE;

  addGlobalAlternatives(altInfos, &finder);

  assert_false(altInfos[0].end);
  assert_equal(&altEntry, altInfos[0].alt);
  assert_equal(NO_PARAMETER, altInfos[0].parameter);
  assert_equal(NO_INSTANCE, altInfos[0].instance);
  assert_equal(NO_CLASS, altInfos[0].class);
  assert_true(altInfos[1].end);
}


/*----------------------------------------------------------------------*/
TestSuite *altInfoArrayTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, canDuplicateAltInfoArray);
  add_test(suite, canFindLastAltInfoIndex);
  add_test(suite, canFindNextFreeAltInfo);
  add_test(suite, canAddGlobalAlternatives);
  return suite;
}

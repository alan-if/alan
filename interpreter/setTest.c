/*======================================================================*\

  setTest.c

  Unit tests for set type module in the Alan interpreter

\*======================================================================*/

#include "set.c"


static void testSetAdd() {
  Set aSet = {0, 0, NULL};
  int i;

  /* Add a single value */
  addToSet(&aSet, 0);
  ASSERT(aSet.size == 1);
  ASSERT(aSet.members[0] == 0);
  /* Add it again, should not extend the set */
  addToSet(&aSet, 0);
  ASSERT(aSet.size == 1);

  /* Add a number of elements so that we need to extend */
  for (i = 1; i<6; i++) {
    addToSet(&aSet, i);
    ASSERT(aSet.size == i+1);
    ASSERT(aSet.members[i] == i);
  }
  ASSERT(aSet.size == 6);
}  

static void testSetRemove() {
  Set aSet = {0, 0, NULL};
  int i;

  /* Add a number of elements to remove */
  for (i = 1; i<6; i++)
    addToSet(&aSet, i);

  ASSERT(aSet.size == 5);

  removeFromSet(&aSet, 1);
  ASSERT(aSet.size == 4);

  /* Do it again, should not change... */
  removeFromSet(&aSet, 1);
  ASSERT(aSet.size == 4);

  removeFromSet(&aSet, 5);
  ASSERT(aSet.size == 3);
  removeFromSet(&aSet, 4);
  ASSERT(aSet.size == 2);
  removeFromSet(&aSet, 3);
  ASSERT(aSet.size == 1);
  removeFromSet(&aSet, 2);
  ASSERT(aSet.size == 0);

  removeFromSet(&aSet, 1);
  ASSERT(aSet.size == 0);
}  

static void testInSet() {
  Set aSet = {0, 0, NULL};
  int i;

  for (i = 6; i>0; i--)
    addToSet(&aSet, i);
  for (i = 1; i<7; i++)
    ASSERT(inSet(&aSet, i));
}

void registerSetUnitTests()
{
  registerUnitTest(testSetAdd);
  registerUnitTest(testSetRemove);
  registerUnitTest(testInSet);
}

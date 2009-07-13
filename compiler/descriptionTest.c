/*======================================================================*\

  descriptionTest.c

  Unit tests for DESCRIPTION node in the Alan compiler

\*======================================================================*/

#include "description.c"

#include "unit.h"


void testChecksOf() {
  Description *description = newDescription(nulsrcp, NULL, nulsrcp, NULL);
  List list;

  ASSERT(checksOf(NULL) == NULL);
  ASSERT(checksOf(description) == NULL);
  description->checks = &list;
  ASSERT(checksOf(description) == &list);
}


void descriptionUnitTests()
{
  registerUnitTest(testChecksOf);
}


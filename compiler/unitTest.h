/*======================================================================*\

  unitTest.h

  Test harness for unit tests in Alan compiler

\*======================================================================*/

#define UNIT

#include <stdio.h>

static int passed = 0;
static int failed = 0;

static void fail()
{
  failed++;
}


static void unitReportProgress(failed, passed)
{
  return;
  printf("failed: %d, passed: %d\n", failed, passed);
}


/* Assert a particular test */
void unitAssert(int x)
{
  (x)? passed++ : fail();
  unitReportProgress(failed, passed);
}


/* Run the tests in the test case array*/
static void unitTest(void)
{
  Case *current;

  for (current = caseList; current != NULL; current = current->next) {
    (*current->theCase)();
  }
  if (failed == 0)
    printf("\nAll %d cases PASSED!!\n", passed);
  else
    printf("\n%d of %d cases FAILED!!\n", failed, passed+failed);
}


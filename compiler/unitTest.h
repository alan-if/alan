/*======================================================================*\

  unitTest.h

  Test harness for unit tests in Alan compiler

\*======================================================================*/

#define UNIT

#include <stdio.h>

static int passed = 0;
static int failed = 0;

/* Assert a particular test */
#define unitAssert(x) ({(x)?passed++:fail(),failed++; unitReportProgress(failed, passed);})


/* Run the tests in the test case array*/
static void unitTest(void)
{
  void (**theCase)();

  for (theCase = &cases[0]; *theCase != NULL; theCase++) {
    (*theCase)();
  }
  if (failed == 0)
    printf("\nAll %d cases PASSED!!\n", passed);
  else
    printf("\n%d of %d cases FAILED!!\n", failed, passed+failed);
}


static void fail()
{}


static void unitReportProgress(failed, passed)
{
  return;
  printf("failed: %d, passed: %d\n", failed, passed);
}


/*======================================================================*\

  advTest.c

  A unit test main program for the Alan compiler
  It will include the complete module source and then define some tests
  and test cases which will be run through the unitTest() runner.

\*======================================================================*/

#include <stdio.h>

#include "adv.c"


static void (*(cases[]))();

#include "unitTest.h"


int main()
{

  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  unitTest();

  return 0;
}

/* From unitList.c */  
extern int lastEcode;

void testInitAdv()
{
  Srcp srcp = {2,3,4};
  adv.whr = newwhr(&srcp, WHR_HERE, NULL);
  analyzeStartAt();		/* Can not Start At Here */

  unitAssert(lastEcode == 211);
  unitAssert(lmSeverity() == sevERR);
}


static void (*(cases[]))() = {
  &testInitAdv,
  NULL
};


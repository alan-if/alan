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
extern lmSev lastSev;

void testInitAdv()
{
  Srcp srcp = {2,3,4};
  adv.whr = newwhr(&srcp, WHR_HERE, NULL);

  analyzeStartAt();		/* Can not Start At Here */
  unitAssert(lastEcode == 211);
  unitAssert(lmSeverity() == sevERR);


  lastEcode = 0;
  lastSev = 0;
  adv.whr = newwhr(&srcp, WHR_AT,
		   newwht(&srcp, WHT_ID,
			  newId(&srcp, "atL")));
  analyzeStartAt();		/* Can not Start At unknown Id */
  unitAssert(lmSeverity() == sevERR);
  unitAssert(lastEcode == 310);
}


static void (*(cases[]))() = {
  &testInitAdv,
  NULL
};


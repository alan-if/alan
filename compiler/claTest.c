/*======================================================================*\

  claTest.c

  A unit test main program for the Alan compiler
  It will include the complete module source and then define some tests
  and test cases which will be run through the unitTest() runner.

\*======================================================================*/

#include <stdio.h>

#include "cla.c"


static void (*(cases[]))();

#include "unitTest.h"


int main()
{

  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  unitTest();

  return 0;
}
  

void testCreateCla()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "claId");
  IdNode *heritage = newId(&srcp, "heritageId");

  ClaNod *cla = newcla(&srcp, id, heritage, NULL);

  unitAssert(equalSrcp(srcp, cla->srcp));
  unitAssert(equalId(id, cla->id));
  unitAssert(equalId(heritage, cla->heritage));

  analyzeClasses();
}


static void (*(cases[]))() = {
  &testCreateCla,
  NULL
};


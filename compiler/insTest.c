/*======================================================================*\

  insTest.c

  A unit test main program for the Alan compiler
  It will include the complete module source and then define some tests
  and test cases which will be run through the unitTest() runner.

\*======================================================================*/

#include <stdio.h>

#include "ins.c"


static void (*(cases[]))();

#include "unitTest.h"


int main()
{

  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  unitTest();

  return 0;
}
  

void testCreateIns()
{
  Srcp srcp = {1,2,3};
  IdNode *id = newId(&srcp, "insId");
  IdNode *heritage = newId(&srcp, "heritageId");

  InsNod *ins = newins(&srcp, id, heritage, NULL);

  unitAssert(equalSrcp(srcp, ins->srcp));
  unitAssert(equalId(id, ins->id));
  unitAssert(equalId(heritage, ins->heritage));

  analyzeInstances();
}


static void (*(cases[]))() = {
  &testCreateIns,
  NULL
};


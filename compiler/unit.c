/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler

\*======================================================================*/

#include <stdio.h>

typedef struct Case {
  void (*theCase)();
  struct Case *next;
} Case;

static Case *caseList = NULL;
static Case *lastCase = NULL;

void registerUnitTest(void (*aCase)());


#include "unitTest.h"

/* Faking the List system */
#include "lmList.h"
/* From unitList.c */  
extern int readEcode();
extern lmSev readSev();


#include "claTest.c"
#include "insTest.c"
#include "advTest.c"
#include "symTest.c"

int main()
{
  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  registerClaUnitTests();
  registerInsUnitTests();
  registerAdvUnitTests();
  registerSymUnitTests();

  unitTest();

  return 0;
}

void registerUnitTest(void (*aCase)())
{
  if (lastCase == NULL) {
    caseList = malloc(sizeof(Case));
    caseList->theCase = aCase;
    lastCase = caseList;
  } else {
    lastCase->next = malloc(sizeof(Case));
    lastCase = lastCase->next;
    lastCase->theCase = aCase;
  }
}
    
    
  
  


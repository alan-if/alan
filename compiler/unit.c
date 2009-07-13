/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler

\*======================================================================*/

#include "sysdep.h"
#include "acode.h"

#include <stdio.h>
#include <setjmp.h>


#include "unit.h"


typedef struct Case {
	void (*theCase)();
	struct Case *next;
} Case;

static Case *caseList = NULL;
static Case *lastCase = NULL;


Aword *memory;


/*======================================================================*/
Aword convertFromACD(Aword w)
{
  Aword s;                      /* The swapped ACODE word */
  char *wp, *sp;
  int i;

  wp = (char *) &w;
  sp = (char *) &s;

  if (littleEndian())
    for (i = 0; i < sizeof(Aword); i++)
      sp[sizeof(Aword)-1 - i] = wp[i];
  else
    for (i = 0; i < sizeof(Aword); i++)
      sp[i] = wp[i];
  
  return s;
}


/*======================================================================*\

  Test harness for unit tests in Alan compiler

\*======================================================================*/

static int passed = 0;
static int failed = 0;

/*----------------------------------------------------------------------*/
static void unitFail(char sourceFile[], int lineNumber, const char function[])
{
  printf("%s:%d: unit test '%s()' failed!\n", sourceFile, lineNumber, function);
  failed++;
}


/*----------------------------------------------------------------------*/
static void unitReportProgress(failed, passed)
{
  return;
  printf("failed: %d, passed: %d\n", failed, passed);
}


/* Assert a particular test */
/*======================================================================*/
void unitAssert(int x, char sourceFile[], int lineNumber, const char function[])
{
  (x)? passed++ : unitFail(sourceFile, lineNumber, function);
  unitReportProgress(failed, passed);
}


/* Run the tests in the test case array */
/*----------------------------------------------------------------------*/
static void unitTest(void)
{
  Case *current;

  for (current = caseList; current != NULL; current = current->next) {
    (*current->theCase)();
  }
  if (failed == 0)
    printf("All %d unit tests PASSED!!\n", passed);
  else {
    printf("******************************\n");
    printf("%d of %d unit tests FAILED!!\n", failed, passed+failed);
    printf("******************************\n");
  }
}


/* Faking the List system */
#include "lmList.h"
#include "unitList.h"


#define UNITTEST(module) \
  extern void module##UnitTests(); \
  module##UnitTests();


int main()
{
  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  UNITTEST(add);
  UNITTEST(adv);
  UNITTEST(atr);
  UNITTEST(cla);
  UNITTEST(description);
  UNITTEST(elm);
  UNITTEST(emit);
  UNITTEST(exp);
  UNITTEST(ext);
  UNITTEST(id);
  UNITTEST(ifid);
  UNITTEST(ins);
  UNITTEST(lst);
  UNITTEST(prop);
  UNITTEST(res);
  UNITTEST(resource);
  UNITTEST(stm);
  UNITTEST(stx);
  UNITTEST(sym);
  UNITTEST(vrb);
  UNITTEST(whr);
  UNITTEST(wrd);
   
  unitTest();

  return 0;
}


/*======================================================================*/
void registerUnitTest(void (*aCase)())
{
  if (lastCase == NULL) {
    caseList = calloc(sizeof(Case), 1);
    caseList->theCase = aCase;
    lastCase = caseList;
  } else {
    lastCase->next = calloc(sizeof(Case), 1);
    lastCase = lastCase->next;
    lastCase->theCase = aCase;
  }
  lastCase->next = NULL;
}


/*----------------------------------------------------------------------*/
static Aword reversed(Aword w)		/* IN - The ACODE word to swap bytes in */
{
  Aword s;			/* The swapped ACODE word */
  char *wp, *sp;
  int i;

  wp = (char *) &w;
  sp = (char *) &s;

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return (s);
}

/*----------------------------------------------------------------------*/
static void reverse(Aword *w)
{
  *w = reversed(*w);
}

/*----------------------------------------------------------------------*/
static void reverseHdr(ACodeHeader *header)
{
  int i;

  /* Reverse all words in the header except the first (version marking) */
  for (i = 1; i < sizeof(ACodeHeader)/sizeof(Aword); i++)
    reverse(&((Aword *)header)[i]);
}

/*======================================================================*/
void loadACD(char fileName[])
{
	ACodeHeader temporaryHeader;
	int readSize = 0;
	FILE *acdFile = fopen(fileName, "rb");

	readSize = fread(&temporaryHeader, 1, sizeof(temporaryHeader), acdFile);

	if (littleEndian())
		reverseHdr(&temporaryHeader);

	memory = calloc(4*temporaryHeader.size, 1);

	rewind(acdFile);
	fread(memory, sizeof(Aword), temporaryHeader.size, acdFile);

}

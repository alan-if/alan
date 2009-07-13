/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler

\*======================================================================*/

#include "sysdep.h"
#include "acode.h"

#include <stdio.h>
#include <setjmp.h>


typedef struct Case {
	void (*theCase)();
	struct Case *next;
} Case;

static Case *caseList = NULL;
static Case *lastCase = NULL;


static void registerUnitTest(void (*aCase)());


static Aword *memory;
static void loadACD(char fileName[]);

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

static void unitFail(char sourceFile[], int lineNumber, const char function[])
{
  printf("%s:%d: unit test '%s()' failed!\n", sourceFile, lineNumber, function);
  failed++;
}


static void unitReportProgress(failed, passed)
{
  return;
  printf("failed: %d, passed: %d\n", failed, passed);
}


#define ASSERT(x) (unitAssert((x), __FILE__, __LINE__, __FUNCTION__))

/* Assert a particular test */
void unitAssert(int x, char sourceFile[], int lineNumber, const char function[])
{
  (x)? passed++ : unitFail(sourceFile, lineNumber, function);
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
    printf("All %d unit tests PASSED!!\n", passed);
  else {
    printf("******************************\n");
    printf("%d of %d unit tests FAILED!!\n", failed, passed+failed);
    printf("******************************\n");
  }
}


/* Faking the List system */
#include "lmList.h"
/* From unitList.c */
extern int readEcode();
extern lmSev readSev();

#include "ifidTest.c"
#include "descriptionTest.c"
#include "lstTest.c"
#include "resourceTest.c"
#include "elmTest.c"
#include "stmTest.c"
#include "claTest.c"
#include "propTest.c"
#include "insTest.c"
#include "advTest.c"
#include "symTest.c"
#include "whrTest.c"
#include "vrbTest.c"
#include "extTest.c"
#include "emitTest.c"
#include "atrTest.c"
#include "expTest.c"
#include "addTest.c"
#include "stxTest.c"
#include "resTest.c"
#include "wrdTest.c"
#include "paramTest.c"
#include "idTests.c"


int main()
{
	lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

	registerDescriptionUnitTests();
	registerLstUnitTests();
	registerPropUnitTests();
	registerResourceUnitTests();
	registerClaUnitTests();
	registerIfidUnitTests();
	registerInsUnitTests();
	registerAdvUnitTests();
	registerSymUnitTests();
	registerWhrUnitTests();
	registerVrbUnitTests();
	registerEmitUnitTests();
	registerExtUnitTests();
	registerExpUnitTests();
	registerAtrUnitTests();
	registerAddUnitTests();
	registerResUnitTests();
	registerStxUnitTests();
	registerStmUnitTests();
	registerElmUnitTests();
	registerWrdUnitTests();
	registerParamUnitTests();
	registerIdUnitTests();

	unitTest();

	return 0;
}

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

static void reverse(Aword *w)
{
	*w = reversed(*w);
}

static void reverseHdr(ACodeHeader *header)
{
	int i;

	/* Reverse all words in the header except the first (version marking) */
	for (i = 1; i < sizeof(ACodeHeader)/sizeof(Aword); i++)
		reverse(&((Aword *)header)[i]);
}

static void loadACD(char fileName[])
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

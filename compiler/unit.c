/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler

\*======================================================================*/

#include "sysdep.h"
#include "../interpreter/acode.h"


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


#include "unitTest.h"

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

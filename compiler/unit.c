/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler

\*======================================================================*/

#include "sysdep.h"
#include "acode.h"


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

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return s;
}


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
#include "whrTest.c"
#include "emitTest.c"


int main()
{
  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  registerClaUnitTests();
  registerInsUnitTests();
  registerAdvUnitTests();
  registerSymUnitTests();
  registerWhrUnitTests();
  registerEmitUnitTests();

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


static void reverse(Aword *w)
{
  *w = reversed(*w);
}

static void reverseHdr(AcdHdr *hdr)
{
  int i;

  /* Reverse all words in the header except the first (version marking) */
  for (i = 1; i < sizeof(AcdHdr)/sizeof(Aword); i++)
    reverse(&((Aword *)hdr)[i]);
}

static void loadACD(char fileName[])
{
  AcdHdr tmphdr;
  FILE *acdFile = fopen(fileName, "r");

  fread(&tmphdr, sizeof(tmphdr), 1, acdFile);

#ifdef REVERSED
  reverseHdr(&tmphdr);
#endif

  memory = malloc(4*tmphdr.size);

  rewind(acdFile);
  fread(memory, sizeof(Aword), tmphdr.size, acdFile);

}

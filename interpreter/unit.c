/*======================================================================*\

  unit.c

  A unit test main program for the Alan interpreter

\*======================================================================*/

#include "sysdep.h"
#include "acode.h"
#include "reverse.h"

#include <setjmp.h>


typedef struct Case {
  void (*theCase)();
  struct Case *next;
} Case;

static Case *caseList = NULL;
static Case *lastCase = NULL;


static void registerUnitTest(void (*aCase)());


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


#include "syserr.h"

static Bool hadSyserr = FALSE;
jmp_buf syserr_label;
jmp_buf uniterr_label;
Bool expectSyserr = FALSE;

void syserr(char msg[])
{
  if (!expectSyserr) {
    printf("*** SYSTEM ERROR: %s ***\n", msg);
    longjmp(uniterr_label, TRUE);
  } else {
    hadSyserr = TRUE;
    longjmp(syserr_label, TRUE);
  }
}


#include "unitTest.h"

#include "exeTest.c"
#include "parseTest.c"
#include "stackTest.c"
#include "interTest.c"
#include "reverseTest.c"
#include "sysdepTest.c"
#include "setTest.c"
#include "mainTest.c"

int main()
{
  if (setjmp(uniterr_label) == 0) {
    registerExeUnitTests();
    registerParseUnitTests();
    registerStackUnitTests();
    registerInterUnitTests();
    registerReverseUnitTests();
    registerSysdepUnitTests();
    registerSetUnitTests();
    registerMainUnitTests();

    unitTest();
  }
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


static void loadACD(char fileName[])
{
  ACodeHeader tmphdr;
  FILE *acdFile = fopen(fileName, "r");

  fread(&tmphdr, sizeof(tmphdr), 1, acdFile);

  if (littleEndian())
    reverseHdr(&tmphdr);

  memory = calloc(4*tmphdr.size, 1);

  rewind(acdFile);
  fread(memory, sizeof(Aword), tmphdr.size, acdFile);

}

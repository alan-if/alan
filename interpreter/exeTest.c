/*======================================================================*\

  exeTest.c

  Unit tests for exe module in the Alan interpreter

\*======================================================================*/

#include "exe.c"


static char testFileName[] = "getStringTestFile";
static FILE *testFile;
static void writeAndOpenGetStringTestFile(int fpos, char *testString)
{
  int i;

  testFile = fopen(testFileName, "wb");
  for (i = 0; i < fpos; i++) fputc(' ', testFile);
  fprintf(testFile, testString);
  fclose(testFile);
  txtfil = fopen(testFileName, "rb");
}


extern void unlink();

void testGetString()
{
  int fpos = 55;
  char testString[] = "hejhopp";

  writeAndOpenGetStringTestFile(fpos, testString);
  header->pack = FALSE;
  header->stringOffset = 0;
  getstr(fpos, strlen(testString));
  ASSERT(strcmp((char *)pop(), testString)==0);
  header->stringOffset = 1;
  getstr(fpos, strlen(testString)-1);
  ASSERT(strcmp((char *)pop(), &testString[1])==0);
  fclose(txtfil);
  unlink(testFileName);
}


static void testIncreaseEventQueue()
{
  eventQueueSize = 0;
  eventQueue = NULL;
  eventQueueTop = 0;

  increaseEventQueue();

  ASSERT(eventQueueSize != 0);
  ASSERT(eventQueue != NULL);
}

static void testPushGameState() {
  gameState = NULL;
  gameStateTop = 0;
  pushGameState();
  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);
}


static void testPopGameState() {
  attributes = allocate(3*sizeof(AttributeEntry));
  admin = allocate(3*sizeof(AdminEntry));

  header->attributesAreaSize = 3*sizeof(AttributeEntry);
  header->instanceMax = 2;

  gameState = NULL;
  gameStateTop = 0;
  gameStateSize = 0;
  attributes[0].value = 12;
  attributes[2].value = 3;
  pushGameState();
  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);

  eventQueueTop = 1;
  eventQueue[1].time = 47;
  attributes[0].value = 11;
  attributes[2].value = 4;
  pushGameState();

  eventQueueTop = 0;
  eventQueue[1].time = 1;
  popGameState();

  ASSERT(eventQueueTop == 1);
  ASSERT(eventQueue[1].time == 47);

  popGameState();

  ASSERT(attributes[0].value == 12);
  ASSERT(attributes[2].value == 3);
}

static void testWhereIllegalId() {
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    where(0);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    where(1000);
  ASSERT(hadSyserr);
}


void registerExeUnitTests()
{
  registerUnitTest(testGetString);
  registerUnitTest(testIncreaseEventQueue);
  registerUnitTest(testPushGameState);
  registerUnitTest(testPopGameState);
  registerUnitTest(testWhereIllegalId);
}

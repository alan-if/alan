/*======================================================================*\

  exeTest.c

  Unit tests for exe module in the Alan interpreter

\*======================================================================*/

#include "exe.c"


static void testCountTrailingBlanks() {
  char *threeBlanks = "h   ";
  char *fiveBlanks = "     ";
  char *empty = "";
  ASSERT(countTrailingBlanks(threeBlanks, strlen(threeBlanks)-1) == 3);
  ASSERT(countTrailingBlanks(threeBlanks, 1) == 1);
  ASSERT(countTrailingBlanks(threeBlanks, 0) == 0);
  ASSERT(countTrailingBlanks(fiveBlanks, strlen(fiveBlanks)-1) == 5);
  ASSERT(countTrailingBlanks(empty, strlen(empty)-1) == 0);
}

static void testSkipWordForwards() {
  char *string = "a string of words";

  ASSERT(skipWordForwards(string, 0) == 1);
  ASSERT(skipWordForwards(string, 2) == 8);
  ASSERT(skipWordForwards(string, 9) == 11);
  ASSERT(skipWordForwards(string, 12) == 17);
  ASSERT(skipWordForwards(string, strlen(string)-1) == strlen(string));
}

static void testSkipWordBackwards() {
  char *string = "a string of words";
  char *emptyString = "";

  ASSERT(skipWordBackwards(string, 0) == 0);
  ASSERT(skipWordBackwards(string, 4) == 2);
  ASSERT(skipWordBackwards(string, 10) == 9);
  ASSERT(skipWordBackwards(string, strlen(string)) == 12);

  ASSERT(skipWordBackwards(emptyString, strlen(emptyString)) == 0);
}

static void testStripCharsFromString() {
  char *characters;
  char *rest;
  char *result;

  characters = "abcdef";
  result = stripCharsFromStringForwards(3, characters, &rest);
  ASSERT(strcmp(result, "abc")==0);
  ASSERT(strcmp(rest, "def")==0);

  characters = "ab";
  result = stripCharsFromStringForwards(3, characters, &rest);
  ASSERT(strcmp(result, "ab")==0);
  ASSERT(strcmp(rest, "")==0);

  characters = "";
  result = stripCharsFromStringForwards(3, characters, &rest);
  ASSERT(strcmp(result, "")==0);
  ASSERT(strcmp(rest, "")==0);

  characters = "abcdef";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  ASSERT(strcmp(result, "def")==0);
  ASSERT(strcmp(rest, "abc")==0);

  characters = "ab";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  ASSERT(strcmp(result, "ab")==0);
  ASSERT(strcmp(rest, "")==0);

  characters = "";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  ASSERT(strcmp(result, "")==0);
  ASSERT(strcmp(rest, "")==0);
}

static void testStripWordsFromString() {
  char *testString = "this is four  words";
  char *empty = "";
  char *result;
  char *rest;

  result = stripWordsFromStringForwards(3, empty, &rest);
  ASSERT(strcmp(result, "") == 0);
  ASSERT(strcmp(rest, "") == 0);

  result = stripWordsFromStringForwards(3, testString, &rest);
  ASSERT(strcmp(result, "this is four") == 0);
  ASSERT(strcmp(rest, "words") == 0);

  result = stripWordsFromStringForwards(7, testString, &rest);
  ASSERT(strcmp(result, "this is four  words") == 0);
  ASSERT(strcmp(rest, "") == 0);

  result = stripWordsFromStringBackwards(3, empty, &rest);
  ASSERT(strcmp(result, "") == 0);
  ASSERT(strcmp(rest, "") == 0);

  result = stripWordsFromStringBackwards(3, testString, &rest);
  ASSERT(strcmp(result, "is four  words") == 0);
  ASSERT(strcmp(rest, "this") == 0);

  result = stripWordsFromStringBackwards(7, testString, &rest);
  ASSERT(strcmp(result, "this is four  words") == 0);
  ASSERT(strcmp(rest, "") == 0);

  testString = " an initial space";
  result = stripWordsFromStringBackwards(7, testString, &rest);
  ASSERT(strcmp(result, "an initial space") == 0);
  ASSERT(strcmp(rest, "") == 0);
}


static char testFileName[] = "getStringTestFile";
static FILE *testFile;
static void writeAndOpenGetStringTestFile(int fpos, char *testString)
{
  int i;

  testFile = fopen(testFileName, "wb");
  for (i = 0; i < fpos; i++) fputc(' ', testFile);
  fprintf(testFile, testString);
  fclose(testFile);
  textFile = fopen(testFileName, "rb");
}


void testGetString()
{
  int fpos = 55;
  char testString[] = "hejhopp";

  writeAndOpenGetStringTestFile(fpos, testString);
  header->pack = FALSE;
  header->stringOffset = 0;
  getStringFromFile(fpos, strlen(testString));
  ASSERT(strcmp((char *)pop(), testString)==0);
  header->stringOffset = 1;
  getStringFromFile(fpos, strlen(testString)-1);
  ASSERT(strcmp((char *)pop(), &testString[1])==0);
  fclose(textFile);
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
  header->instanceMax = 1;
  expectSyserr = TRUE;
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    where(0);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    where(2);
  ASSERT(hadSyserr);
  expectSyserr = FALSE;
}

static void testHereIllegalId() {
  header->instanceMax = 1;
  expectSyserr = TRUE;
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    isHere(0);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    isHere(2);
  ASSERT(hadSyserr);
  expectSyserr = FALSE;
}

static void testLocateIllegalId() {
  header->instanceMax = 1;
  expectSyserr = TRUE;
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    locate(0, 1);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    locate(2, 1);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    locate(1, 0);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    locate(1, 2);
  ASSERT(hadSyserr);
  expectSyserr = FALSE;
}


void registerExeUnitTests()
{
  registerUnitTest(testCountTrailingBlanks);
  registerUnitTest(testSkipWordForwards);
  registerUnitTest(testSkipWordBackwards);
  registerUnitTest(testStripCharsFromString);
  registerUnitTest(testStripWordsFromString);
  registerUnitTest(testGetString);
  registerUnitTest(testIncreaseEventQueue);
  registerUnitTest(testPushGameState);
  registerUnitTest(testPopGameState);
  registerUnitTest(testWhereIllegalId);
  registerUnitTest(testHereIllegalId);
  registerUnitTest(testLocateIllegalId);
}

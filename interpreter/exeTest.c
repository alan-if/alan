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
  admin = allocate(sizeof(AdminEntry));
  attributes = allocate(3);
  gameState = NULL;
  gameStateTop = 0;
  pushGameState();
  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);
}


static void testPopGameState() {
  attributes = allocate(3*sizeof(AttributeEntry));
  admin = allocate(3*sizeof(AdminEntry));

  header->attributesAreaSize = 3*sizeof(AttributeEntry)/sizeof(Aword);
  header->instanceMax = 2;

  gameState = NULL;
  gameStateTop = 0;
  gameStateSize = 0;
  attributes[0].value = 12;
  attributes[2].value = 3;
  pushGameState();
  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);

  eventQueueTop = 2;
  eventQueue[1].time = 47;
  attributes[0].value = 11;
  attributes[2].value = 4;
  admin[2].location = 12;
  admin[2].alreadyDescribed = 2;
  admin[2].visitsCount = 13;
  admin[2].script = 33;
  admin[2].step = 3886;
  admin[2].waitCount = 38869878;
  pushGameState();

  admin[2].location = 55;
  admin[2].alreadyDescribed = 55;
  admin[2].visitsCount = 55;
  admin[2].script = 55;
  admin[2].step = 55;
  admin[2].waitCount = 55;

  eventQueueTop = 0;
  eventQueue[1].time = 1;
  popGameState();
  ASSERT(admin[2].location == 12);
  ASSERT(admin[2].alreadyDescribed == 2);
  ASSERT(admin[2].visitsCount == 13);
  ASSERT(admin[2].script == 33);
  ASSERT(admin[2].step == 3886);
  ASSERT(admin[2].waitCount == 38869878);

  ASSERT(eventQueueTop == 2);
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
    where(0, TRUE);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    where(2, TRUE);
  ASSERT(hadSyserr);
  expectSyserr = FALSE;
}

static void testHereIllegalId() {
  header->instanceMax = 1;
  expectSyserr = TRUE;
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    isHere(0, FALSE);
  ASSERT(hadSyserr);
  hadSyserr = FALSE;
  if (setjmp(syserr_label) == 0)
    isHere(2, FALSE);
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


static void testSaveRestore() {
  FILE *saveFile = fopen("testSaveFile", "w");
  Aword scoreTable = EOF;
  int i;

  /* Set up empty eventQ and scores and other irrelevant data */
  eventQueueTop = 0;
  scores = &scoreTable;
  adventureName = "adventure";

  /* Init header */
  header->instanceMax = 3;
  header->attributesAreaSize = 20*sizeof(AttributeEntry)/sizeof(Aword);

  /* Initialize a fake instance table */
  instance = malloc(4*sizeof(InstanceEntry));
  instance[0].initialAttributes = 12; /* Shouldn't matter where.. */
  instance[1].initialAttributes = 12; /* Shouldn't matter where.. */
  instance[2].initialAttributes = 12; /* Shouldn't matter where.. */
  instance[3].initialAttributes = 12; /* Shouldn't matter where.. */

  /* Allocate an attribute area and initialize it */
  attributes = malloc(20*sizeof(AttributeEntry));
  for (i = 0; i<20; i++) {
    attributes[i].code = i;
    attributes[i].value = i;
    attributes[i].stringAddress = 0;
  }
  attributes[20].code = EOF;

  /* Fake admin areas for 3 instances */
  admin[1].attributes = &attributes[0];
  admin[1].attributes[0].code = 11;
  admin[1].attributes[0].value = 11;
  admin[2].attributes = &attributes[5];
  admin[2].attributes[0].code = 22;
  admin[2].attributes[0].value = 22;
  admin[3].attributes = &attributes[7];
  admin[3].attributes[0].code = 33;
  admin[3].attributes[0].value = 33;

  /* Save the game data */
  saveGame(saveFile);
  fclose(saveFile);
  free(attributes);

  /* Get another attribute area and initialize admin areas */
  attributes = malloc(20*sizeof(AttributeEntry));
  for (i = 0; i<20; i++) {
    attributes[19-i].code = i;
    attributes[19-i].value = i;
  }
  admin[1].attributes = &attributes[0];
  admin[2].attributes = &attributes[5];
  admin[3].attributes = &attributes[7];

  saveFile = fopen("testSaveFile", "r");
  restoreGame(saveFile);
  fclose(saveFile);
  unlink("testSaveFile");

  ASSERT(admin[1].attributes[0].code == 11);
  ASSERT(admin[1].attributes[0].value == 11);
  ASSERT(admin[2].attributes[0].code == 22);
  ASSERT(admin[2].attributes[0].value == 22);
  ASSERT(admin[3].attributes[0].code == 33);
  ASSERT(admin[3].attributes[0].value == 33);
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
  registerUnitTest(testSaveRestore);
}

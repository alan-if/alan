/*======================================================================*\

  exeTests.c

  Unit tests for exe module in the Alan interpreter
  using cgreen unit test framework

\*======================================================================*/

#include "cgreen/cgreen.h"

#include "set.h"
#include "exe.c"


/*----------------------------------------------------------------------*/
static void testCountTrailingBlanks() {
  char *threeBlanks = "h   ";
  char *fiveBlanks = "     ";
  char *empty = "";
  assert_true(countTrailingBlanks(threeBlanks, strlen(threeBlanks)-1) == 3);
  assert_true(countTrailingBlanks(threeBlanks, 1) == 1);
  assert_true(countTrailingBlanks(threeBlanks, 0) == 0);
  assert_true(countTrailingBlanks(fiveBlanks, strlen(fiveBlanks)-1) == 5);
  assert_true(countTrailingBlanks(empty, strlen(empty)-1) == 0);
}


/*----------------------------------------------------------------------*/
static void testSkipWordForwards() {
  char *string = "a string of words";

  assert_true(skipWordForwards(string, 0) == 1);
  assert_true(skipWordForwards(string, 2) == 8);
  assert_true(skipWordForwards(string, 9) == 11);
  assert_true(skipWordForwards(string, 12) == 17);
  assert_true(skipWordForwards(string, strlen(string)-1) == strlen(string));
}


/*----------------------------------------------------------------------*/
static void testSkipWordBackwards() {
  char *string = "a string of words";
  char *emptyString = "";

  assert_true(skipWordBackwards(string, 0) == 0);
  assert_true(skipWordBackwards(string, 4) == 2);
  assert_true(skipWordBackwards(string, 10) == 9);
  assert_true(skipWordBackwards(string, strlen(string)) == 12);

  assert_true(skipWordBackwards(emptyString, strlen(emptyString)) == 0);
}


/*----------------------------------------------------------------------*/
static void testStripCharsFromString() {
  char *characters;
  char *rest;
  char *result;

  characters = "abcdef";
  result = stripCharsFromStringForwards(3, characters, &rest);
  assert_true(strcmp(result, "abc")==0);
  assert_true(strcmp(rest, "def")==0);

  characters = "ab";
  result = stripCharsFromStringForwards(3, characters, &rest);
  assert_true(strcmp(result, "ab")==0);
  assert_true(strcmp(rest, "")==0);

  characters = "";
  result = stripCharsFromStringForwards(3, characters, &rest);
  assert_true(strcmp(result, "")==0);
  assert_true(strcmp(rest, "")==0);

  characters = "abcdef";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  assert_true(strcmp(result, "def")==0);
  assert_true(strcmp(rest, "abc")==0);

  characters = "ab";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  assert_true(strcmp(result, "ab")==0);
  assert_true(strcmp(rest, "")==0);

  characters = "";
  result = stripCharsFromStringBackwards(3, characters, &rest);
  assert_true(strcmp(result, "")==0);
  assert_true(strcmp(rest, "")==0);
}


/*----------------------------------------------------------------------*/
static void testStripWordsFromString() {
  char *testString = "this is four  words";
  char *empty = "";
  char *result;
  char *rest;

  result = stripWordsFromStringForwards(3, empty, &rest);
  assert_true(strcmp(result, "") == 0);
  assert_true(strcmp(rest, "") == 0);

  result = stripWordsFromStringForwards(3, testString, &rest);
  assert_true(strcmp(result, "this is four") == 0);
  assert_true(strcmp(rest, "words") == 0);

  result = stripWordsFromStringForwards(7, testString, &rest);
  assert_true(strcmp(result, "this is four  words") == 0);
  assert_true(strcmp(rest, "") == 0);

  result = stripWordsFromStringBackwards(3, empty, &rest);
  assert_true(strcmp(result, "") == 0);
  assert_true(strcmp(rest, "") == 0);

  result = stripWordsFromStringBackwards(3, testString, &rest);
  assert_true(strcmp(result, "is four  words") == 0);
  assert_true(strcmp(rest, "this") == 0);

  result = stripWordsFromStringBackwards(7, testString, &rest);
  assert_true(strcmp(result, "this is four  words") == 0);
  assert_true(strcmp(rest, "") == 0);

  testString = " an initial space";
  result = stripWordsFromStringBackwards(7, testString, &rest);
  assert_true(strcmp(result, "an initial space") == 0);
  assert_true(strcmp(rest, "") == 0);
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


/*----------------------------------------------------------------------*/
static void testGetString()
{
  int fpos = 55;
  char testString[] = "hejhopp";

  writeAndOpenGetStringTestFile(fpos, testString);
  header->pack = FALSE;
  header->stringOffset = 0;
  assert_true(strcmp(getStringFromFile(fpos, strlen(testString)), testString)==0);
  header->stringOffset = 1;
  assert_true(strcmp(getStringFromFile(fpos, strlen(testString)-1), &testString[1])==0);
  fclose(textFile);
  unlink(testFileName);
}


/*----------------------------------------------------------------------*/
static void testIncreaseEventQueue()
{
  eventQueueSize = 0;
  eventQueue = NULL;
  eventQueueTop = 0;

  increaseEventQueue();

  assert_true(eventQueueSize != 0);
  assert_true(eventQueue != NULL);
}



/*----------------------------------------------------------------------*/
static Bool syserrHandlerCalled;

jmp_buf syserrLabel;


static void syserrHandler(char *message) {
  syserrHandlerCalled = TRUE;
  printf("%s\n", message);
  longjmp(syserrLabel, TRUE);
}


static int triedAndNoSyserrCaught() {
  setSyserrHandler(syserrHandler);
  syserrHandlerCalled = FALSE;
  if (setjmp(syserrLabel) == 0)
    return FALSE;
  else {
    assert_true(syserrHandlerCalled);
    return TRUE;
  }
}

static void failAssertion(void) {
	assert_true(FALSE);
}

/*----------------------------------------------------------------------*/
static void testWhereIllegalId() {
  header->instanceMax = 1;

  if (triedAndNoSyserrCaught()) {
    where(0, TRUE);
    failAssertion();
  }

  if (triedAndNoSyserrCaught()) {
    where(2, TRUE);
    failAssertion();
  }
}


/*----------------------------------------------------------------------*/
static void testHereIllegalId() {
  header->instanceMax = 1;

  if (triedAndNoSyserrCaught()) {
    isHere(0, FALSE);
    failAssertion();
  }

  if (triedAndNoSyserrCaught()) {
    isHere(2, FALSE);
    failAssertion();
  }
}


/*----------------------------------------------------------------------*/
static void testLocateIllegalId() {
  header->instanceMax = 1;

  if (triedAndNoSyserrCaught()) {
    locate(0, 1);
    failAssertion();
  }

  if (triedAndNoSyserrCaught()) {
    locate(2, 1);
    failAssertion();
  }

  if (triedAndNoSyserrCaught()) {
    locate(1, 0);
    failAssertion();
  }

  if (triedAndNoSyserrCaught()) {
    locate(1, 2);
    failAssertion();
  }
}


/*----------------------------------------------------------------------*/
static void testWhere() {
  admin = allocate(5*sizeof(AdminEntry));
  instances = allocate(5*sizeof(InstanceEntry));
  classes = allocate(5*sizeof(ClassEntry));
  header = allocate(sizeof(ACodeHeader));

  header->locationClassId = 1;
  header->instanceMax = 4;

  instances[1].parent = 1;	/* A location */
  admin[1].location = 3;
  assert_true(where(1, TRUE) == 0);	/* Locations are always nowhere */
  assert_true(where(1, FALSE) == 0);

  instances[2].parent = 0;	/* Not a location */
  admin[2].location = 1;	/* At 1 */
  assert_true(where(2, TRUE) == 1);
  assert_true(where(2, FALSE) == 1);

  instances[3].parent = 0;	/* Not a location */
  admin[3].location = 2;	/* In 2 which is at 1*/
  assert_true(where(3, TRUE) == 2);
  assert_true(where(3, FALSE) == 1);

  instances[4].parent = 0;	/* Not a location */
  admin[4].location = 3;	/* In 3 which is in 2 which is at 1*/
  assert_true(where(4, TRUE) == 3);
  assert_true(where(4, FALSE) == 1);

  free(admin);
  free(instances);
  free(classes);
  free(header);
}


/*----------------------------------------------------------------------*/
static void testGetMembers() {
  Set *set = newSet(0);
  Aword code[] = {0,	/* Dummy */
		  (Aword) set,
		  INSTRUCTION(I_SETSIZE),
		  INSTRUCTION(I_RETURN)};

  Stack stack = createStack(50);
  setInterpreterStack(stack);

  memory = code;
  memTop = 100;
  assert_equal(0, evaluate(1));
}


/*----------------------------------------------------------------------*/
void testContainerSize() {
  header = allocate(sizeof(ACodeHeader));
  instances = allocate(4*sizeof(InstanceEntry));
  admin = allocate(4*sizeof(AdminEntry));

  header->instanceMax = 3;
  instances[1].container = 1;
  admin[1].location = 0;
  admin[2].location = 1;
  admin[3].location = 2;

  assert_true(containerSize(1, TRUE) == 1);
  assert_true(containerSize(1, FALSE) == 2);

  free(admin);
  free(instances);
  free(header);
}


/*----------------------------------------------------------------------*/
static void tearDown() {
  setSyserrHandler(NULL);
}


TestSuite *exeTests() {
  TestSuite *suite = create_test_suite();

  teardown(suite, tearDown);

  add_test(suite, testCountTrailingBlanks);
  add_test(suite, testSkipWordForwards);
  add_test(suite, testSkipWordBackwards);
  add_test(suite, testStripCharsFromString);
  add_test(suite, testStripWordsFromString);
  add_test(suite, testGetString);
  add_test(suite, testIncreaseEventQueue);
  add_test(suite, testWhereIllegalId);
  add_test(suite, testHereIllegalId);
  add_test(suite, testLocateIllegalId);
  add_test(suite, testWhere);
  add_test(suite, testGetMembers);
  add_test(suite, testContainerSize);

  return suite;
}

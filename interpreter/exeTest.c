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
  EventQueueSize = 0;
  eventQueue = NULL;
  etop = 0;

  increaseEventQueue();

  ASSERT(EventQueueSize != 0);
  ASSERT(eventQueue != NULL);
}



void registerExeUnitTests()
{
  registerUnitTest(testGetString);
  registerUnitTest(testIncreaseEventQueue);
}

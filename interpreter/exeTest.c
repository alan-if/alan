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
  unitAssert(strcmp((char *)pop(), testString)==0);
  header->stringOffset = 1;
  getstr(fpos, strlen(testString)-1);
  unitAssert(strcmp((char *)pop(), &testString[1])==0);
  fclose(txtfil);
  unlink(testFileName);
}


void registerExeUnitTests()
{
  registerUnitTest(testGetString);
}

/*======================================================================*\

  emitTest.c

  Unit tests for EMIT node in the Alan compiler

\*======================================================================*/

#include "emit.c"

void testEmit()
{
  int expectedAddress;
  Aword emitTestArray[5] = {1, 2, 3, 4, 5};

  initEmit("unit.a3c");

  expectedAddress = emadr();

  emit(0);
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitString("123");
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitString("1234");
  expectedAddress += 2;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emit0(C_STMOP, I_IF);
  expectedAddress += 1;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitN((Aword *)&emitTestArray, 5);
  expectedAddress += 5;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();

  emitEntry((Aword *)&emitTestArray, 6*4);
  expectedAddress += 6;
  unitAssert(emadr() == expectedAddress);
  expectedAddress = emadr();
}


static void generateTextDataFile(char textDataFileName[], char textData[])
{
  FILE *textDataFile = fopen(textDataFileName, WRITE_MODE);
  fwrite(textData, strlen(textData), 1, textDataFile);
  fclose(textDataFile);
}


static void testEmitTextDataToAcodeFile()
{
  char textDataFileName[] = "emitTestData";
  char textData[] = "asfasjfalsfhwerouwr87340183482jlasfls";
  int i;

  generateTextDataFile(textDataFileName, textData);
  acdfil = fopen("emitTestAcode", WRITE_MODE);
  emitTextDataToAcodeFile(textDataFileName);
  fclose(acdfil);
  acdfil = fopen("emitTestAcode", READ_MODE);
  for (i = 0; i < strlen(textData); i ++)
    if (fgetc(acdfil) != textData[i]) {
      unitAssert(FALSE);
    }
  fclose(acdfil);
  unlink(textDataFileName);
}
  


void registerEmitUnitTests()
{
  registerUnitTest(testEmit);
  registerUnitTest(testEmitTextDataToAcodeFile);
}


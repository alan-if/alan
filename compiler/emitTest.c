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

  expectedAddress = nextEmitAddress();

  emit(0);
  expectedAddress += 1;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitString("123");
  expectedAddress += 1;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitString("1234");
  expectedAddress += 2;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emit0(I_IF);
  expectedAddress += 1;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitN((Aword *)&emitTestArray, 5);
  expectedAddress += 5;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitEntry((Aword *)&emitTestArray, 6*4);
  expectedAddress += 6;
  ASSERT(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();
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
      ASSERT(FALSE);
    }
  fclose(acdfil);
  unlink(textDataFileName);
}
  


void registerEmitUnitTests()
{
  registerUnitTest(testEmit);
  registerUnitTest(testEmitTextDataToAcodeFile);
}


/*======================================================================*\

  emitTest.c

  Unit tests for EMIT node in the Alan compiler

\*======================================================================*/

#include "emit.c"

#include "unit.h"


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
  copyTextDataToAcodeFile(textDataFileName);
  fclose(acdfil);
  acdfil = fopen("emitTestAcode", READ_MODE);
  for (i = 0; i < strlen(textData); i ++)
    if (fgetc(acdfil) != textData[i]) {
      ASSERT(FALSE);
    }
  fclose(acdfil);
  unlink(textDataFileName);
}


static void finalizeEmitShouldAdjustCRCWithoutChangingSize() {
    int i;

    /* Given: */
    crc = 0;
    acodeHeader.acdcrc = 0;
    
    Aword *headerAsArray = (Aword *)&acodeHeader;

    int sizeDiff = ASIZE(ACodeHeader) - ASIZE(Pre3_0beta3Header);
    for (i = ASIZE(Pre3_0beta3Header); i < ASIZE(ACodeHeader); i++)
        headerAsArray[i] = 0x22334455;

    int size = nextEmitAddress();

    /* When: */
    finalizeEmit();

    /* Then: */
    ASSERT(acodeHeader.acdcrc == sizeDiff * ((Aword)0x22 + (Aword)0x33 + (Aword)0x44 + (Aword)0x55));
    ASSERT(acodeHeader.size == size);
}


void emitUnitTests()
{
  registerUnitTest(testEmit);
  registerUnitTest(testEmitTextDataToAcodeFile);
  registerUnitTest(finalizeEmitShouldAdjustCRCWithoutChangingSize);
}


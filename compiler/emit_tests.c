/*======================================================================*\

  emit_tests.c

  Unit tests for the Emit module in the Alan compiler

\*======================================================================*/
#include <cgreen/cgreen.h>

#include "emit.h"
#include "sysdep.h"

#include "srcp.mock"
#include "lmList.mock"
#include "lmlog.mock"


Describe(Emit);
BeforeEach(Emit) {}
AfterEach(Emit) {}


Ensure(Emit, testEmit) {
  int expectedAddress;
  Aword emitTestArray[5] = {1, 2, 3, 4, 5};

  initEmit("unit.a3c");

  expectedAddress = nextEmitAddress();

  emit(0);
  expectedAddress += 1;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitString("123");
  expectedAddress += 1;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitString("1234");
  expectedAddress += 2;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emit0(I_IF);
  expectedAddress += 1;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitN((Aword *)&emitTestArray, 5);
  expectedAddress += 5;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();

  emitEntry((Aword *)&emitTestArray, 6*4);
  expectedAddress += 6;
  assert_true(nextEmitAddress() == expectedAddress);
  expectedAddress = nextEmitAddress();
}


static void generateTextDataFile(char textDataFileName[], char textData[])
{
  FILE *textDataFile = fopen(textDataFileName, WRITE_MODE);
  fwrite(textData, strlen(textData), 1, textDataFile);
  fclose(textDataFile);
}


Ensure(Emit, testEmitTextDataToAcodeFile) {
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
      assert_true(FALSE);
    }
  fclose(acdfil);
  unlink(textDataFileName);
}

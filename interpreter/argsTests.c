#include "cgreen/cgreen.h"

#include "args.c"

Ensure (canAddAcodeExtension) {
    char *fileName = allocate(strlen("file")+strlen(".a3c")+1);
    strcpy(fileName, "file");
    assert_string_equal(addAcodeExtension(fileName), "file.a3c");
    free(fileName);
}

Ensure (gameNameCanStripLeadingDotAndSlash) {
    char *sampleGameName = strdup("./game.a3c");
    assert_string_equal(gameName(sampleGameName), "game");
}

TestSuite *argsTests() {
  TestSuite *suite = create_test_suite();

  add_test(suite, canAddAcodeExtension);
  add_test(suite, gameNameCanStripLeadingDotAndSlash);

  return suite;
}

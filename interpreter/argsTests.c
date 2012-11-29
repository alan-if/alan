#include "cgreen/cgreen.h"

#include "args.c"

Describe(Args);
BeforeEach(Args) {}
AfterEach(Args) {}

Ensure(Args, canAddAcodeExtension) {
    char *fileName = allocate(strlen("file")+strlen(".a3c")+1);
    strcpy(fileName, "file");
    assert_string_equal(addAcodeExtension(fileName), "file.a3c");
    free(fileName);
}

Ensure(Args, gameNameCanStripLeadingDotAndSlash) {
    char *sampleGameName = strdup("./game.a3c");
    assert_string_equal(gameName(sampleGameName), "game");
}

TestSuite *argsTests() {
  TestSuite *suite = create_test_suite();

  add_test_with_context(suite, Args, canAddAcodeExtension);
  add_test_with_context(suite, Args, gameNameCanStripLeadingDotAndSlash);

  return suite;
}

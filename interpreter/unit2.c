#include "cgreen.h"

TestSuite *actTests();
TestSuite *altInfoArrayTests();
TestSuite *altInfoTests();
TestSuite *exeTests();
TestSuite *parseTests();
TestSuite *saveTests();
TestSuite *stateTests();
TestSuite *stackTests();
TestSuite *sysdepTests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, actTests());
    add_suite(suite, altInfoArrayTests());
    add_suite(suite, altInfoTests());
    add_suite(suite, exeTests());
    add_suite(suite, parseTests());
    add_suite(suite, saveTests());
    add_suite(suite, stackTests());
    add_suite(suite, stateTests());
    add_suite(suite, sysdepTests());
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}

#include "cgreen/cgreen.h"

TestSuite *actTests();
TestSuite *altInfoArrayTests();
TestSuite *altInfoTests();
TestSuite *exeTests();
TestSuite *interTests();
TestSuite *mainTests();
TestSuite *parseTests();
TestSuite *reverseTests();
TestSuite *saveTests();
TestSuite *setTests();
TestSuite *stateTests();
TestSuite *stateStackTests();
TestSuite *stackTests();
TestSuite *sysdepTests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    TestReporter *reporter = create_cute_reporter();

    add_suite(suite, actTests());
    add_suite(suite, altInfoArrayTests());
    add_suite(suite, altInfoTests());
    add_suite(suite, exeTests());
    add_suite(suite, interTests());
    add_suite(suite, mainTests());
    add_suite(suite, parseTests());
    add_suite(suite, reverseTests());
    add_suite(suite, saveTests());
    add_suite(suite, setTests());
    add_suite(suite, stackTests());
    add_suite(suite, stateStackTests());
    add_suite(suite, stateTests());
    add_suite(suite, sysdepTests());
    if (argc > 1) {
        return run_single_test(suite, argv[1], reporter);
    }
    return run_test_suite(suite, reporter);
}

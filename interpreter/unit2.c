#include "cgreen/cgreen.h"
#include "xml_reporter.h"

#ifdef SMARTALLOC
#include "smartall.h"
#endif

#define ADD_UNIT_TESTS_FOR(module) \
  TestSuite *module##Tests(); \
  add_suite(suite, module##Tests());


int main(int argc, char **argv) {
	int return_code;
    TestSuite *suite = create_test_suite();
    TestReporter *reporter = create_text_reporter();

    ADD_UNIT_TESTS_FOR(act);
    ADD_UNIT_TESTS_FOR(altInfo);
    ADD_UNIT_TESTS_FOR(args);
    ADD_UNIT_TESTS_FOR(debug);
    ADD_UNIT_TESTS_FOR(exe);
    ADD_UNIT_TESTS_FOR(inter);
    ADD_UNIT_TESTS_FOR(instance);
    ADD_UNIT_TESTS_FOR(main);
    ADD_UNIT_TESTS_FOR(params);
    ADD_UNIT_TESTS_FOR(parameterPosition);
    ADD_UNIT_TESTS_FOR(parse);
    ADD_UNIT_TESTS_FOR(reverse);
    ADD_UNIT_TESTS_FOR(rules);
    ADD_UNIT_TESTS_FOR(save);
    ADD_UNIT_TESTS_FOR(set);
    ADD_UNIT_TESTS_FOR(stack);
    ADD_UNIT_TESTS_FOR(stateStack);
    ADD_UNIT_TESTS_FOR(state);
    ADD_UNIT_TESTS_FOR(sysdep);
    ADD_UNIT_TESTS_FOR(output);
    ADD_UNIT_TESTS_FOR(word);

    // TODO: Awful hack, should use command arg parsing...
    if (argc == 1)
    	return_code = run_test_suite(suite, reporter);
    else if (argc == 2) {
        if (strcmp(argv[1], "-xml") == 0) {
            reporter = create_xml_reporter();
            return_code = run_test_suite(suite, reporter);
        } else
            return_code = run_single_test(suite, argv[2], reporter);
    } else if (argc == 3) {
        if (strcmp(argv[1], "-xml") == 0) {
            reporter = create_xml_reporter();
            return_code = run_single_test(suite, argv[2], reporter);
        } else if (strcmp(argv[2], "-xml") == 0) {
            reporter = create_xml_reporter();
            return_code = run_single_test(suite, argv[1], reporter);
        } else
            printf("Usage: %s [-xml] [<test case name>]", argv[0]);
    } else {
        printf("Usage: %s [-xml] [<test case name>]", argv[0]);
    }
#ifdef SMARTALLOC
    sm_dump(1);
#endif
    return return_code;
}

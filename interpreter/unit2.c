#include "cgreen/cgreen.h"

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

    if (argc > 1) {
        return_code = run_single_test(suite, argv[1], reporter);
    } else {
    	return_code = run_test_suite(suite, reporter);
    }
#ifdef SMARTALLOC
    sm_dump(1);
#endif
    return return_code;
}

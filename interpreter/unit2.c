#include "cgreen/cgreen.h"
#include "xml_reporter.h"
#include <stdlib.h>
#include "gopt.h"

#ifdef SMARTALLOC
#include "smartall.h"
#endif

#define ADD_UNIT_TESTS_FOR(module) \
  TestSuite *module##Tests(); \
  add_suite(suite, module##Tests());


static void add_unittests(TestSuite *suite) {
    ADD_UNIT_TESTS_FOR(altInfo);
    ADD_UNIT_TESTS_FOR(args);
    ADD_UNIT_TESTS_FOR(debug);
    ADD_UNIT_TESTS_FOR(exe);
    ADD_UNIT_TESTS_FOR(inter);
    ADD_UNIT_TESTS_FOR(instance);
    ADD_UNIT_TESTS_FOR(params);
    ADD_UNIT_TESTS_FOR(main);
    ADD_UNIT_TESTS_FOR(parameterPosition);
    ADD_UNIT_TESTS_FOR(parse);
    ADD_UNIT_TESTS_FOR(reverse);
    ADD_UNIT_TESTS_FOR(save);
    ADD_UNIT_TESTS_FOR(set);
    ADD_UNIT_TESTS_FOR(stack);
    ADD_UNIT_TESTS_FOR(stateStack);
    ADD_UNIT_TESTS_FOR(state);
    ADD_UNIT_TESTS_FOR(sysdep);
    ADD_UNIT_TESTS_FOR(output);
    ADD_UNIT_TESTS_FOR(word);
}


static int interpreter_tests(int argc, const char **argv) {
    int return_code;
    TestSuite *suite = create_test_suite();
    TestReporter *reporter;
    const char *prefix;

    add_unittests(suite);

    void *options= gopt_sort(&argc, argv, gopt_start(
                                                     gopt_option( 'x', 
                                                                  GOPT_ARG, 
                                                                  gopt_shorts( 'x' ), 
                                                                  gopt_longs( "xml" ))));

    if (gopt_arg(options, 'x', &prefix))
        reporter = create_xml_reporter(prefix);
    else
        reporter = create_text_reporter();
    
    if (argc == 1) {
        return_code = run_test_suite(suite, reporter);
    } else if (argc == 2) {
        return_code = run_single_test(suite, argv[1], reporter);
    } else {
        printf("Usage: %s [--xml <fileprefix>] [<test case name>]\n", argv[0]);
    }
    return return_code;
}

int main(int argc, const char **argv) {
    return interpreter_tests(argc, argv);
}


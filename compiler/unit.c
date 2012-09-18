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
    ADD_UNIT_TESTS_FOR(add);
    ADD_UNIT_TESTS_FOR(adv);
    ADD_UNIT_TESTS_FOR(atr);
    ADD_UNIT_TESTS_FOR(cla);
    ADD_UNIT_TESTS_FOR(description);
    ADD_UNIT_TESTS_FOR(elm);
    ADD_UNIT_TESTS_FOR(emit);
    ADD_UNIT_TESTS_FOR(exp);
    ADD_UNIT_TESTS_FOR(ext);
    ADD_UNIT_TESTS_FOR(id);
    ADD_UNIT_TESTS_FOR(ifid);
    ADD_UNIT_TESTS_FOR(ins);
    ADD_UNIT_TESTS_FOR(lst);
    ADD_UNIT_TESTS_FOR(prop);
    ADD_UNIT_TESTS_FOR(res);
    ADD_UNIT_TESTS_FOR(resource);
    ADD_UNIT_TESTS_FOR(stm);
    ADD_UNIT_TESTS_FOR(stx);
    ADD_UNIT_TESTS_FOR(sym);
    ADD_UNIT_TESTS_FOR(vrb);
    ADD_UNIT_TESTS_FOR(whr);
    ADD_UNIT_TESTS_FOR(wrd);
}


static int compiler_tests(int argc, const char **argv) {
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
    return compiler_tests(argc, argv);
}

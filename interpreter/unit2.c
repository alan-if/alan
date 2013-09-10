#include "cgreen/cgreen.h"
#include "xml_reporter.h"
#include <stdlib.h>
#include "gopt.h"

#define ADD_UNITTESTS_FOR(module) \
  TestSuite *module##Tests(); \
  add_suite(suite, module##Tests());


static void add_unittests(TestSuite *suite) {
#include "add_unittests.include"
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


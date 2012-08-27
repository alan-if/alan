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
}


int main(int argc, const char **argv) {
    int return_code;
    TestSuite *suite = create_test_suite();
    TestReporter *reporter = create_text_reporter();

    /* Option values: */
    FILE *out_stream;
    const char *filename;

    add_unittests(suite);

    void *options= gopt_sort(&argc, argv, gopt_start(
                                                     gopt_option( 'x', 
                                                                  GOPT_ARG, 
                                                                  gopt_shorts( 'x' ), 
                                                                  gopt_longs( "xml" ))));

    if (gopt_arg(options, 'x', &filename) && strcmp(filename, "-")) {
        /*
         * if -x or --xml was specified, and its argument was not "-"
         */
        out_stream = fopen( filename, "wb" );
        if(!out_stream ){
            fprintf(stderr, "%s: %s: could not open file for output\n", argv[0], filename);
            exit( EXIT_FAILURE);
        }
    } else
        out_stream= stdout;
    
    if (gopt(options, 'x'))
        reporter = create_xml_reporter(out_stream);
    
    if (argc == 2) {
        return_code = run_test_suite(suite, reporter);
    } else if (argc == 3) {
        return_code = run_single_test(suite, argv[2], reporter);
    } else {
        printf("Usage: %s [--xml] <library of unittests> [<test case name>]\n", argv[0]);
    }
    return return_code;
}

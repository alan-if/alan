#include "xml_reporter.h"
#include "cgreen/reporter.h"
#include "cgreen/breadcrumb.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>


static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line);
static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments);


static char *current_suite = NULL; /* TODO: actually need a stack... Could we use the breadcrumb? */

static FILE *out;


TestReporter *create_xml_reporter(FILE *output) {
    TestReporter *reporter = create_reporter();
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->show_fail = &xml_show_fail;
    reporter->show_incomplete = &xml_show_incomplete;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    out = output;
    fprintf(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    return reporter;
}

static void indent(TestReporter *reporter) {
    int depth = get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb);
    while (depth-- > 0) {
        fprintf(out, "\t");
    }
}

static void pathprinter(const char *trail, int level, int depth, void *memo) {
    fprintf(out, "%s/", trail);
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count) {
    indent(reporter);
    fprintf(out, "<testsuite name=\"");
    walk_breadcrumb(reporter->breadcrumb, pathprinter, NULL);
    fprintf(out, "%s\">\n", suitename);
    reporter_start(reporter, suitename);
    current_suite = strdup(suitename);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    indent(reporter);
    fprintf(out, "<testcase classname=\"");
    walk_breadcrumb(reporter->breadcrumb, pathprinter, NULL);
    fprintf(out, "\" name=\"%s\">\n", testname);
    reporter_start(reporter, testname);
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    indent(reporter);
    fprintf(out, "<failure message=\"");
    vfprintf(out, message, arguments);
    fprintf(out, "\">\n");
    indent(reporter);
    fprintf(out, "\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
    indent(reporter);
    fprintf(out, "</failure>\n");
}

static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments) {
    indent(reporter);
    fprintf(out, "<error type=\"Fatal\" message=\"");
    vfprintf(out, message, arguments);
    fprintf(out, "\">\n");
    indent(reporter);
    fprintf(out, "</error>\n");
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line) {
    reporter_finish(reporter, filename, line);
    indent(reporter);
    fprintf(out, "</testcase>\n");
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    reporter_finish(reporter, filename, line);
    indent(reporter);
    fprintf(out, "</testsuite>\n");
    free(current_suite);
    current_suite = NULL;
}

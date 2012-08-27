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

static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count) {
    indent(reporter);
    fprintf(out, "<testsuite name=\"%s\">\n", name);
    reporter_start(reporter, name);
    current_suite = strdup(name);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *name) {
    indent(reporter);
    fprintf(out, "<testcase classname=\"%s\" name=\"%s\">\n", current_suite, name);
    reporter_start(reporter, name);
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
    fprintf(out, "</suite>\n");
    free(current_suite);
    current_suite = NULL;
}

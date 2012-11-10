#include "cgreen/cgreen.h"

#include "act.c"

Describe(act);
BeforeEach(act){}
AfterEach(act){}

#include "lists.h"

TestSuite *actTests() {
    TestSuite *suite = create_test_suite();
    return suite;
}

#include "cgreen/cgreen.h"

#include "act.c"

Describe(act);
BeforeEach(act){}
AfterEach(act){}

#include "lists.h"

Ensure(act, canDoSomething) {
}

TestSuite *actTests() {
    TestSuite *suite = create_test_suite();
    return suite;
}

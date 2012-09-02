#include "cgreen/cgreen.h"

#include "act.c"


#include "lists.h"

Ensure(fails) {
    assert_true(false);
}

TestSuite *actTests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, fails);
    return suite;
}

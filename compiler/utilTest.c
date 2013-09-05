#include <cgreen/cgreen.h>

#include "util.c"

Ensure(spaces_returns_requested_number_of_spaces) {
    assert_that(strlen(spaces(3)), is_equal_to(3));
}

TestSuite *utilTests()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, spaces_returns_requested_number_of_spaces);
    return suite;
}


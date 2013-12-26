#include <cgreen/cgreen.h>

#include "util.c"

Describe(Utilities);
BeforeEach(Utilities) {}
AfterEach(Utilities) {}

Ensure(Utilities, spaces_returns_requested_number_of_spaces) {
    assert_that(strlen(spaces(3)), is_equal_to(3));
}

Ensure(Utilities, strmov_can_move_empty_string) {
    char *from = "";
    char to[20];

    strmov(to, from);
    assert_that(to, is_equal_to_string(""));
}

Ensure(Utilities, strmov_can_move_string_of_one) {
    char *from = "1";
    char to[20];

    strmov(to, from);
    assert_that(to, is_equal_to_string("1"));
}

Ensure(Utilities, strmov_can_move_string_of_many) {
    char *from = "this is many characters";
    char to[20];

    strmov(to, from);
    assert_that(to, is_equal_to_string("this is many characters"));
}

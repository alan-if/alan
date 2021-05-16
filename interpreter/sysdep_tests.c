#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "sysdep.h"

#include "syserr.mock"
#include "instance.mock"


Describe(Sysdep);

BeforeEach(Sysdep) {
}

AfterEach(Sysdep) {
}

Ensure(Sysdep, returns_zero_for_equal_ascii_strings) {
    char *string1 = "abc";

    assert_that(compareStrings(string1, string1), is_equal_to(0));
}

Ensure(Sysdep, returns_not_zero_for_different_ascii_strings) {
    char *string1 = "abc";
    char *string2 = "abz";

    assert_that(compareStrings(string1, string2), is_not_equal_to(0));
}

Ensure(Sysdep, returns_zero_for_ascii_strings_different_only_in_case) {
    char *string1 = "abc";
    char *string2 = "aBc";

    assert_that(compareStrings(string1, string2), is_equal_to(0));
}

Ensure(Sysdep, returns_not_zero_for_different_iso_strings) {
    char *string1 = "ab\xC5";
    char *string2 = "ab\xC6";

    assert_that(compareStrings(string1, string2), is_not_equal_to(0));
}

Ensure(Sysdep, returns_zero_for_iso_strings_different_only_in_case) {
    char *string1 = "ab\xE5";
    char *string2 = "ab\xC5";

    assert_that(compareStrings(string1, string2), is_equal_to(0));
}

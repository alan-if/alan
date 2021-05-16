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

Ensure(Sysdep, canCompareStringsIgnoringCase)
{
  assert_true(equalStrings("abcd", "abcd"));
  assert_true(equalStrings("abcd", "Abcd"));
  assert_true(equalStrings("abcd", "aBcd"));
  assert_true(equalStrings("abcd", "abCd"));
  assert_true(equalStrings("abcd", "abcD"));
  assert_true(equalStrings("abcd", "abcd"));
  assert_true(equalStrings("ABCD", "Abcd"));
  assert_true(equalStrings("ABCD", "aBcd"));
  assert_true(equalStrings("ABCD", "abCd"));
  assert_true(equalStrings("ABCD", "abcD"));
  assert_true(!equalStrings("abcd", "abcde"));
  assert_true(!equalStrings("abcde", "Abcd"));
  assert_true(!equalStrings("abc", "aBcd"));
  assert_true(!equalStrings("abd", "abCd"));
  assert_true(!equalStrings("bcd", "abcD"));
}

Ensure(Sysdep, returns_true_for_equal_ascii_strings) {
    char *string1 = "abc";

    assert_that(equalStrings(string1, string1));
}

Ensure(Sysdep, returns_false_for_different_ascii_strings) {
    char *string1 = "abc";
    char *string2 = "abz";

    assert_that(!equalStrings(string1, string2));
}

Ensure(Sysdep, returns_true_for_ascii_strings_different_only_in_case) {
    char *string1 = "abc";
    char *string2 = "aBc";

    assert_that(equalStrings(string1, string2));
}

Ensure(Sysdep, returns_false_for_different_iso_strings) {
    char *string1 = "ab\xC5";
    char *string2 = "ab\xC6";

    assert_that(!equalStrings(string1, string2));
}

Ensure(Sysdep, returns_true_for_iso_strings_different_only_in_case) {
    char *string1 = "ab\xE5";
    char *string2 = "ab\xC5";

    assert_that(equalStrings(string1, string2));
}

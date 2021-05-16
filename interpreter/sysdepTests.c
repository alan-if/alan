/*======================================================================*\

  sysdepTests.c

  Unit tests for sysdep module in the Alan interpreter

\*======================================================================*/

#include "cgreen/cgreen.h"

#include "sysdep.c"


Describe(Sysdep);
BeforeEach(Sysdep) {}
AfterEach(Sysdep) {}


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

Ensure(Sysdep, canSeeLowerCase)
{
  assert_true(isLower(246));
}

/*======================================================================*\

  sysdepTest.c

  Unit tests for sysdep module in the Alan interpreter

\*======================================================================*/

#include "sysdep.c"


static void testStringEqualsIgnoringCase()
{
  ASSERT(stringEqualsIgnoringCase("abcd", "abcd"));
  ASSERT(stringEqualsIgnoringCase("abcd", "Abcd"));
  ASSERT(stringEqualsIgnoringCase("abcd", "aBcd"));
  ASSERT(stringEqualsIgnoringCase("abcd", "abCd"));
  ASSERT(stringEqualsIgnoringCase("abcd", "abcD"));
  ASSERT(stringEqualsIgnoringCase("abcd", "abcd"));
  ASSERT(stringEqualsIgnoringCase("ABCD", "Abcd"));
  ASSERT(stringEqualsIgnoringCase("ABCD", "aBcd"));
  ASSERT(stringEqualsIgnoringCase("ABCD", "abCd"));
  ASSERT(stringEqualsIgnoringCase("ABCD", "abcD"));
  ASSERT(!stringEqualsIgnoringCase("abcd", "abcde"));
  ASSERT(!stringEqualsIgnoringCase("abcde", "Abcd"));
  ASSERT(!stringEqualsIgnoringCase("abc", "aBcd"));
  ASSERT(!stringEqualsIgnoringCase("abd", "abCd"));
  ASSERT(!stringEqualsIgnoringCase("bcd", "abcD"));
}


void registerSysdepUnitTests()
{
  registerUnitTest(testStringEqualsIgnoringCase);
}

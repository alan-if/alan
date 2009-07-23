#include "cgreen/cgreen.h"

#include "word.c"

Ensure canAllocatePlayerWordsFromEmpty() {
  assert_equal(playerWords, NULL);
  ensureSpaceForPlayerWords(1);
  assert_not_equal(playerWords, NULL);
  assert_equal(playerWordsLength, PLAYER_WORDS_EXTENT);
}

Ensure canExtendPlayerWords() {
  assert_equal(playerWords, NULL);
  ensureSpaceForPlayerWords(0);
  ensureSpaceForPlayerWords(PLAYER_WORDS_EXTENT);
  assert_not_equal(playerWords, NULL);
  assert_equal(playerWordsLength, 2*PLAYER_WORDS_EXTENT);
}

TestSuite *wordTests()
{
  TestSuite *suite = create_test_suite();

  add_test(suite, canAllocatePlayerWordsFromEmpty);
  add_test(suite, canExtendPlayerWords);

  return suite;
}

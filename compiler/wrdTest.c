/*======================================================================*\

  wrdTest.c

  Unit tests for WRD node in the Alan compiler

\*======================================================================*/

#include "wrd.c"

static void testInitWrd()
{
}

static void testInsertWord()
{
  Word w1, w2, w3, w4;

  w1.string = "s1";
  w1.low = w1.high = NULL;
  w2.string = "s2";
  w2.low = w2.high = NULL;
  w3.string = "s3";
  w3.low = w3.high = NULL;
  w4.string = "s4";
  w4.low = w4.high = NULL;

  wordTree = NULL;
  insertWord(&w1);
  ASSERT(wordTree == &w1);
  insertWord(&w3);
  ASSERT(wordTree->high == &w3);
  insertWord(&w2);
  ASSERT(wordTree->high->low == &w2);
  insertWord(&w4);
  ASSERT(wordTree->high->high == &w4);
}

static void testNewWord()
{
  Instance i1;

  wordTree = NULL;

  newPronounWord("p", &i1);
  ASSERT(strcmp(wordTree->string, "p") == 0);
  ASSERT(wordTree->ref[PRONOUN_WORD]->element.ins == &i1);

  newSynonymWord("s", findWord("p"));
  ASSERT(strcmp(wordTree->high->string, "s") == 0);
  ASSERT(wordTree->high->ref[SYNONYM_WORD]->element.word == findWord("p"));
}

static void testGenerateWordEntry()
{
  Word w1, *w2;
  DictionaryEntry de[2];

  wordTree = NULL;
  initEmitBuffer((Aword*)&de);

  w1.stradr = 14;
  w1.classbits = VERB_BIT;
  w1.code = 17;
  w1.nounRefAddress = 19;
  w1.adjectiveRefAddress = 21;

  generateWordEntry(&w1);
  ASSERT(convertFromACD(de[0].wrd) == 14);
  ASSERT(convertFromACD(de[0].classBits) == VERB_BIT);
  ASSERT(convertFromACD(de[0].code) == 17);
  ASSERT(convertFromACD(de[0].nounRefs) == 19);
  ASSERT(convertFromACD(de[0].adjectiveRefs) == 21);  

  newSynonymWord("w2", &w1);
  w2 = findWord("w2");
  w2->stradr = 15;
  w2->classbits = SYNONYM_BIT;

  generateWordEntry(w2);
  ASSERT(convertFromACD(de[1].wrd) == 15);
  ASSERT(convertFromACD(de[1].classBits) == (SYNONYM_BIT|VERB_BIT));
  ASSERT(convertFromACD(de[1].code) == 17);
  ASSERT(convertFromACD(de[1].nounRefs) == 19);
  ASSERT(convertFromACD(de[1].adjectiveRefs) == 21);  
}

void registerWrdUnitTests()
{
  registerUnitTest(testInitWrd);
  registerUnitTest(testInsertWord);
  registerUnitTest(testNewWord);
  registerUnitTest(testGenerateWordEntry);
}

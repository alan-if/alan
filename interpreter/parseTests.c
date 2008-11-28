#include "cgreen/cgreen.h"

#include "parse.c"

/*----------------------------------------------------------------------*/
static void makeEOS(ElementEntry *element) {
  element->code = EOS;
}

static void makeEOF(ElementEntry *element) {
  *((Aword *)element) = EOF;	/* End of table */
}

static void makeParameterElement(ElementEntry *element) {
  element->code = 0;
}

static void makeWordElement(ElementEntry *element, int code, int next) {
  element->code = code;
  element->next = next;
}

static DictionaryEntry *makeDictionary(int size) {
  dictsize = size;
  return allocate(size*sizeof(DictionaryEntry));
}

static void makeDictionaryEntry(int index, int code, int classBits) {
  if (index > dictsize) syserr("makeDictionaryEntry() out of size");
  dictionary[index].code = code;
  dictionary[index].classBits = classBits;
}


/*----------------------------------------------------------------------*/
static void testMatchEndOfSyntax() {
  ElementEntry *element;
  ElementEntry *elementTable;

  memory = allocate(100*sizeof(Aword));
  elementTable = (ElementEntry *)&memory[20];

  /* No words */
  playerWords[0].code = EOF;
  wordIndex = 0;

  /* First try an empty parse tree */
  makeEOF(elementTable);
  element = matchEndOfSyntax(elementTable);
  assert_equal(NULL, element);

  /* Then one with a single EOS */
  makeEOS(&elementTable[0]);
  makeEOF(&elementTable[1]);

  element = matchEndOfSyntax(elementTable);
  assert_not_equal(NULL, element);
  assert_equal(EOS, element->code);

  free(memory);
}

/*----------------------------------------------------------------------*/
static void testMatchParameterElement() {
  ElementEntry *element;
  ElementEntry *elementTable;

  memory = allocate(100*sizeof(Aword));
  elementTable = (ElementEntry *)&memory[50];

  /* No words */
  playerWords[0].code = EOF;
  wordIndex = 0;

  /* First test an empty parse tree */
  makeEOF(elementTable);
  element = matchEndOfSyntax(elementTable);
  assert_equal(NULL, element);

  /* Then one with a single EOS */
  makeParameterElement(&elementTable[0]);
  makeEOF(&elementTable[1]);

  element = matchParameterElement(elementTable);
  assert_not_equal(NULL, element);
  assert_equal(0, element->code);

  /* Parameter entry at the end */
  makeEOS(&elementTable[0]);
  makeParameterElement(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParameterElement(elementTable);
  assert_not_equal(NULL, element);
  assert_equal(0, element->code);

  free(memory);
}

/*----------------------------------------------------------------------*/
static void testMatchParseTree() {
  ElementEntry *element;
  ElementEntry *elementTable;
  Bool plural;
  ParamEntry parameters[10];

  memory = allocate(100*sizeof(Aword));
  elementTable = (ElementEntry *)&memory[50];

  /* Emulate end of player input */
  playerWords[0].code = EOF;
  wordIndex = 0;

  /* First test EOF with empty parse tree */
  makeEOF(elementTable);
  element = matchParseTree(NULL, elementTable, &plural);
  assert_equal(NULL, element);

  /* Test EOF with EOS */
  makeEOS(&elementTable[0]);
  makeEOF(&elementTable[1]);
  element = matchParseTree(NULL, elementTable, &plural);
  assert_equal(elementTable, element);

  /* Test EOF with word, EOS */
  makeWordElement(&elementTable[0], 1, 0);
  makeEOS(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParseTree(NULL, elementTable, &plural);
  assert_equal(&elementTable[1], element);

  /* Test word, EOF with word, EOS */
  dictionary = makeDictionary(100);
  makeDictionaryEntry(0, 1, PREPOSITION_BIT);
  playerWords[0].code = 0;		/* A preposition with code 0 */
  playerWords[1].code = EOF;
  makeWordElement(&elementTable[0], 1, addressOf(&elementTable[1]));
  makeEOS(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParseTree(parameters, elementTable, &plural);
  assert_equal(&elementTable[1], element);
  free(dictionary);
  free(memory);
}

/*----------------------------------------------------------------------*/
static void testSetupParameterForWord() {
  ACodeHeader acdHeader;
  header = &acdHeader;
  header->maxParameters = 10;
  header->instanceMax = 10;
  dictionary = makeDictionary(20);
  memory = allocate(40*sizeof(Aword));

  makeDictionaryEntry(2, 23, VERB_BIT);
  memcpy(&memory[12], "qwerty", 7);
  dictionary[2].string = 12;

  playerWords[1].code = 2;
  litCount = 0;
  setupParameterForWord(1, 1);

  assert_equal(instanceFromLiteral(1), parameters[0].instance);
  assert_equal(TRUE, parameters[0].useWords);
  assert_equal(1, parameters[0].firstWord);
  assert_equal(1, parameters[0].lastWord);

  assert_equal(EOF, parameters[1].instance);

  free(dictionary);
  free(memory);
}


TestSuite *parseTests()
{
  TestSuite *suite = create_test_suite();
  add_test(suite, testSetupParameterForWord);
  add_test(suite, testMatchEndOfSyntax);
  add_test(suite, testMatchParameterElement);
  add_test(suite, testMatchParseTree);
  return suite;
}

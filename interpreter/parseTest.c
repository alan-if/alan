/*======================================================================*\

  parseTest.c

  Unit tests for parse module in the Alan interpreter

\*======================================================================*/

#include "parse.c"

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
}


static void makeDictionary(int index, int class, int code) {
  dictionary[index].code = code;
  dictionary[index].class = class;
}



static void testMatchEndOfSyntax() {
  ElementEntry *element;
  ElementEntry *elementTable;

  memory = allocate(100);
  elementTable = (ElementEntry *)&memory[50];

  /* No words */
  playerWords[0] = EOF;
  wordIndex = 0;

  /* First try an empty parse tree */
  makeEOF(elementTable);
  element = matchEndOfSyntax(elementTable);
  ASSERT(element == NULL);

  /* Then one with a single EOS */
  makeEOS(&elementTable[0]);
  makeEOF(&elementTable[1]);

  element = matchEndOfSyntax(elementTable);
  ASSERT(element != NULL);
  ASSERT(element->code == EOS);

  free(memory);
}


static void testMatchParameterElement() {
  ElementEntry *element;
  ElementEntry *elementTable;

  memory = allocate(100);
  elementTable = (ElementEntry *)&memory[50];

  /* No words */
  playerWords[0] = EOF;
  wordIndex = 0;

  /* First test an empty parse tree */
  makeEOF(elementTable);
  element = matchEndOfSyntax(elementTable);
  ASSERT(element == NULL);

  /* Then one with a single EOS */
  makeParameterElement(&elementTable[0]);
  makeEOF(&elementTable[1]);

  element = matchParameterElement(elementTable);
  ASSERT(element != NULL);
  ASSERT(element->code == 0);

  /* Parameter entry at the end */
  makeEOS(&elementTable[0]);
  makeParameterElement(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParameterElement(elementTable);
  ASSERT(element != NULL);
  ASSERT(element->code == 0);

  free(memory);
}

static void testMatchParseTree() {
  ElementEntry *element;
  ElementEntry *elementTable;
  Bool plural;
  ParamEntry parameters[10];

  memory = allocate(100);
  elementTable = (ElementEntry *)&memory[50];

  /* Emulate end of player input */
  playerWords[0] = EOF;
  wordIndex = 0;

  /* First test EOF with empty parse tree */
  makeEOF(elementTable);
  element = matchParseTree(NULL, elementTable, &plural);
  ASSERT(element == NULL);

  /* Test EOF with EOS */
  makeEOS(&elementTable[0]);
  makeEOF(&elementTable[1]);
  element = matchParseTree(NULL, elementTable, &plural);
  ASSERT(element == elementTable);

  /* Test EOF with word, EOS */
  makeWordElement(&elementTable[0], 1, 0);
  makeEOS(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParseTree(NULL, elementTable, &plural);
  ASSERT(element == &elementTable[1]);

  /* Test word, EOF with word, EOS */
  dictionary = allocate(100);
  makeDictionary(0, 1, PREPOSITION_WORD);
  dictsize = 1;
  playerWords[0] = 0;		/* A preposition with code 0 */
  playerWords[1] = EOF;
  makeWordElement(&elementTable[0], 1, addressOf(&elementTable[1]));
  makeEOS(&elementTable[1]);
  makeEOF(&elementTable[2]);
  element = matchParseTree(parameters, elementTable, &plural);
  ASSERT(element == &elementTable[1]);

}

void registerParseUnitTests()
{
  registerUnitTest(testMatchEndOfSyntax);
  registerUnitTest(testMatchParameterElement);

  registerUnitTest(testMatchParseTree);
}

#include "cgreen/cgreen.h"

#include "parse.c"

#include "literal.h"

/*----------------------------------------------------------------------*/
static void makeEOS(ElementEntry *element) {
    element->code = EOS;
}

static void makeParameterElement(ElementEntry *element) {
    element->code = 0;
}

static void makeWordElement(ElementEntry *element, int code, int next) {
    element->code = code;
    element->next = next;
}

static DictionaryEntry *makeDictionary(int size) {
    dictionarySize = size;
    return allocate(size*sizeof(DictionaryEntry));
}

static void makeDictionaryEntry(int index, int code, int classBits) {
    if (index > dictionarySize)
	syserr("makeDictionaryEntry() outside size of dictionary");
    dictionary[index].code = code;
    dictionary[index].classBits = classBits;
}


static ACodeHeader acdHeader;

static void setupHeader(void) {
    header = &acdHeader;
    header->maxParameters = 10;
    header->instanceMax = 10;
}


/*----------------------------------------------------------------------*/
Ensure canMatchEndOfSyntax(void) {
    ElementEntry *element;
    ElementEntry *elementTable;

    memory = allocate(100*sizeof(Aword));
    elementTable = (ElementEntry *)&memory[20];

    /* No words */
    ensureSpaceForPlayerWords(0);
    playerWords[0].code = EOF;
    wordIndex = 0;

    /* First try an empty parse tree */
    setEndOfList(elementTable);
    element = matchEndOfSyntax(elementTable);
    assert_equal(NULL, element);

    /* Then one with a single EOS */
    makeEOS(&elementTable[0]);
    setEndOfList(&elementTable[1]);

    element = matchEndOfSyntax(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(EOS, element->code);

    free(memory);
}

/*----------------------------------------------------------------------*/
Ensure canMatchParameterElement(void) {
    ElementEntry *element;
    ElementEntry *elementTable;

    memory = allocate(100*sizeof(Aword));
    elementTable = (ElementEntry *)&memory[50];

    /* No words */
    ensureSpaceForPlayerWords(0);
    setEndOfList(&playerWords[0]);
    wordIndex = 0;

    /* First test an empty parse tree */
    setEndOfList(elementTable);
    element = matchEndOfSyntax(elementTable);
    assert_equal(NULL, element);

    /* Then one with a single EOS */
    makeParameterElement(&elementTable[0]);
    setEndOfList(&elementTable[1]);

    element = matchParameterElement(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(0, element->code);

    /* Parameter entry at the end */
    makeEOS(&elementTable[0]);
    makeParameterElement(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = matchParameterElement(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(0, element->code);

    free(memory);
}

/*----------------------------------------------------------------------*/
Ensure canMatchParseTree(void) {
    ElementEntry *element;
    ElementEntry *elementTable;
    Bool plural;
    Parameter parameters[10];
    Parameter multipleParameters[10];

    memory = allocate(100*sizeof(Aword));
    elementTable = (ElementEntry *)&memory[50];

    /* Emulate end of player input */
    ensureSpaceForPlayerWords(0);
    playerWords[0].code = EOF;
    wordIndex = 0;

    /* First test EOF with empty parse tree */
    setEndOfList(elementTable);
    element = matchParseTree(parameters, elementTable, &plural, multipleParameters);
    assert_equal(NULL, element);

    /* Test EOF with EOS */
    makeEOS(&elementTable[0]);
    setEndOfList(&elementTable[1]);
    element = matchParseTree(parameters, elementTable, &plural, multipleParameters);
    assert_equal(elementTable, element);

    /* Test EOF with word, EOS */
    makeWordElement(&elementTable[0], 1, 0);
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = matchParseTree(parameters, elementTable, &plural, multipleParameters);
    assert_equal(&elementTable[1], element);

    /* Test word, EOF with word, EOS */
    dictionary = makeDictionary(100);
    makeDictionaryEntry(0, 1, PREPOSITION_BIT);
    playerWords[0].code = 0;		/* A preposition with code 0 */
    playerWords[1].code = EOF;
    makeWordElement(&elementTable[0], 1, addressOf(&elementTable[1]));
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = matchParseTree(parameters, elementTable, &plural, multipleParameters);
    assert_equal(&elementTable[1], element);
    free(dictionary);
    free(memory);
}

/*----------------------------------------------------------------------*/
Ensure canSetupParameterForWord(void) {
    Parameter *messageParameters;

    dictionary = makeDictionary(20);
    memory = allocate(40*sizeof(Aword));

    makeDictionaryEntry(2, 23, VERB_BIT);
    memcpy(&memory[12], "qwerty", 7);
    dictionary[2].string = 12;

    messageParameters = allocateParameterArray(NULL, MAXPARAMS);
    ensureSpaceForPlayerWords(2);
    playerWords[1].code = 2;
    litCount = 0;
    addParameterForWord(messageParameters, 1);

    assert_equal(instanceFromLiteral(1), messageParameters[0].instance);
    assert_equal(TRUE, messageParameters[0].useWords);
    assert_equal(1, messageParameters[0].firstWord);
    assert_equal(1, messageParameters[0].lastWord);

    assert_true(isEndOfList(&messageParameters[1]));

    free(dictionary);
    free(memory);
    free(messageParameters);
}


/*----------------------------------------------------------------------*/
Ensure canSeeBitsInFlag(void) {
    assert_true(hasBit(-1, OMNIBIT));
    assert_false(hasBit(0, OMNIBIT));
    assert_true(hasBit(-1, MULTIPLEBIT));
    assert_false(hasBit(0, MULTIPLEBIT));
}


/*----------------------------------------------------------------------*/
Ensure canSetupInstanceParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);

    addParameterForInstance(parameters, 2);

    assert_false(isLiteral(parameters[0].instance));
    assert_equal(parameters[0].instance, 2);
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupStringParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);

    addParameterForString(parameters, "a string");

    assert_true(isLiteral(parameters[0].instance));
    assert_string_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, "a string");
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupIntegerParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);

    addParameterForInteger(parameters, 14);

    assert_true(isLiteral(parameters[0].instance));
    assert_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, 14);
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canUncheckAllParameterPositions(void) {
    Bool checked[MAXPARAMS+1];
    int i;

    for (i = 0; i < MAXPARAMS; i++)
        checked[i] = i;
    uncheckAllParameterPositions(checked);
    for (i = 0; i < MAXPARAMS; i++)
        assert_equal(checked[i], 0);
}


/*----------------------------------------------------------------------*/
Ensure canMatchEmptyParameterArray(void) {
    Parameter parameters[2];
    clearList(parameters);

    matchParameters(parameters);
    assert_equal(listLength(parameters), 0);
}



TestSuite *parseTests(void)
{
    TestSuite *suite = create_test_suite();

    setup(suite, setupHeader);
    add_test(suite, canSetupParameterForWord);
    add_test(suite, canMatchEndOfSyntax);
    add_test(suite, canMatchParameterElement);
    add_test(suite, canMatchParseTree);
    add_test(suite, canSeeBitsInFlag);
    add_test(suite, canSetupInstanceParametersForMessages);
    add_test(suite, canSetupStringParametersForMessages);
    add_test(suite, canSetupIntegerParametersForMessages);
    add_test(suite, canUncheckAllParameterPositions);
    add_test(suite, canMatchEmptyParameterArray);
    return suite;
}

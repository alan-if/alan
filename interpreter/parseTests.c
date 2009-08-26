#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

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


#define DICTIONARY_SIZE 20
#define ADJECTIVE1_DICTIONARY_INDEX 3
#define ADJECTIVE2_DICTIONARY_INDEX 11
#define NOUN_DICTIONARY_INDEX 5

static DictionaryEntry *makeDictionary() {
    DictionaryEntry *dictionary = allocate(DICTIONARY_SIZE*sizeof(DictionaryEntry));
    dictionarySize = DICTIONARY_SIZE;
    return dictionary;
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
    element = elementForEndOfSyntax(elementTable);
    assert_equal(NULL, element);

    /* Then one with a single EOS */
    makeEOS(&elementTable[0]);
    setEndOfList(&elementTable[1]);

    element = elementForEndOfSyntax(elementTable);
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
    element = elementForEndOfSyntax(elementTable);
    assert_equal(NULL, element);

    /* Then one with a single EOS */
    makeParameterElement(&elementTable[0]);
    setEndOfList(&elementTable[1]);

    element = elementForParameter(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(0, element->code);

    /* Parameter entry at the end */
    makeEOS(&elementTable[0]);
    makeParameterElement(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = elementForParameter(elementTable);
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
    element = parseInputAccordingToElementTree(elementTable, parameters, &plural, multipleParameters);
    assert_equal(NULL, element);

    /* Test EOF with EOS */
    makeEOS(&elementTable[0]);
    setEndOfList(&elementTable[1]);
    element = parseInputAccordingToElementTree(elementTable, parameters, &plural, multipleParameters);
    assert_equal(elementTable, element);

    /* Test EOF with word, EOS */
    makeWordElement(&elementTable[0], 1, 0);
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = parseInputAccordingToElementTree(elementTable, parameters, &plural, multipleParameters);
    assert_equal(&elementTable[1], element);

    /* Test word, EOF with word, EOS */
    dictionary = makeDictionary();
    makeDictionaryEntry(0, 1, PREPOSITION_BIT);
    playerWords[0].code = 0;		/* A preposition with code 0 */
    playerWords[1].code = EOF;
    makeWordElement(&elementTable[0], 1, addressOf(&elementTable[1]));
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
    element = parseInputAccordingToElementTree(elementTable, parameters, &plural, multipleParameters);
    assert_equal(&elementTable[1], element);
    free(dictionary);
    free(memory);
}

/*----------------------------------------------------------------------*/
Ensure canSetupParameterForWord(void) {
    Parameter *messageParameters;

    dictionary = makeDictionary();
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


static void mockedInstanceMatcher(Parameter parameter) {
    mock(parameter.firstWord, parameter.lastWord);
    parameter.candidates[0].instance = 17;
    parameter.candidates[1].instance = -1;
}

/*----------------------------------------------------------------------*/
Ensure canMatchEmptyParameterArray(void) {
    Parameter parameters[2];
    clearList(parameters);

    expect_never(mockedInstanceMatcher);

    matchParameters(parameters, mockedInstanceMatcher);
}


/*----------------------------------------------------------------------*/
Ensure canMatchSingleParameter(void) {
    Parameter parameters[2];
    Parameter candidates[2];

    parameters[0].firstWord = 1;
    parameters[0].lastWord = 1;
    parameters[0].candidates = NULL;
    setEndOfList(&parameters[1]);

    clearList(candidates);
   
    expect(mockedInstanceMatcher,
           want(parameter.firstWord, parameters[0].firstWord),
           want(parameter.lastWord, parameters[0].lastWord));
    
    matchParameters(parameters, mockedInstanceMatcher);

    assert_not_equal(parameters[0].candidates, NULL);
    assert_equal(listLength(parameters[0].candidates), 1);
    assert_equal(parameters[0].candidates[0].instance, 17);
}


static Aint *mockedReferenceFinder(int wordIndex) {
    return (Aint *)mock(wordIndex);
}

static void givenPlayerWordsForANoun(int firstWordIndex) {
    wordIndex = firstWordIndex;
    ensureSpaceForPlayerWords(firstWordIndex);
    playerWords[firstWordIndex].code = ADJECTIVE1_DICTIONARY_INDEX;
}



/*----------------------------------------------------------------------*/
Ensure matchNounPhraseCanMatchSingleNounWithSingleMatch(void) {
    int theExpectedInstance[2] = {23, EOF};
    int theExpectedWordIndex = 3;
    Parameter candidates[MAXENTITY+1];
    Parameter parameter = {0, FALSE, 3, 3, candidates};

    clearList(candidates);
 
	givenPlayerWordsForANoun(theExpectedWordIndex);

    expect(mockedReferenceFinder, want(wordIndex, theExpectedWordIndex));
    will_return(mockedReferenceFinder, theExpectedInstance);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(listLength(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance[0]);
}


static void givenPlayerWordsWithTwoAdjectivesAndANoun(int firstWordIndex) {
    wordIndex = firstWordIndex;
    ensureSpaceForPlayerWords(firstWordIndex+2);
    playerWords[firstWordIndex].code = ADJECTIVE1_DICTIONARY_INDEX;
    playerWords[firstWordIndex+1].code = ADJECTIVE2_DICTIONARY_INDEX;
    playerWords[firstWordIndex+2].code = NOUN_DICTIONARY_INDEX;
}

static void givenADictionaryWithTwoAdjectivesAndANoun() {
    dictionary = makeDictionary();
    makeDictionaryEntry(ADJECTIVE1_DICTIONARY_INDEX, 1, ADJECTIVE_BIT);
    makeDictionaryEntry(ADJECTIVE2_DICTIONARY_INDEX, 1, ADJECTIVE_BIT);
    makeDictionaryEntry(NOUN_DICTIONARY_INDEX, 1, NOUN_BIT);
}


/*----------------------------------------------------------------------*/
Ensure canMatchNounAndAdjectiveWithSingleMatch(void) {
    int theExpectedInstance = 55;
    int firstAdjectiveInstances[4] = {23, theExpectedInstance, 33, EOF};
    int theNounInstances[4] = {25, theExpectedInstance, 34, EOF};
    int theExpectedFirstAdjectiveWordIndex = 3;
    int theExpectedNounWordIndex = 4;
    Parameter candidates[MAXENTITY+1];
    Parameter parameter = {0, FALSE, theExpectedFirstAdjectiveWordIndex, theExpectedNounWordIndex, candidates};
    
    clearList(candidates);
    
    givenADictionaryWithTwoAdjectivesAndANoun();

    expect(mockedReferenceFinder, want(wordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(wordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(listLength(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance);
}


/*----------------------------------------------------------------------*/
Ensure canMatchMultipleAdjectivesAndNounWithSingleMatch(void) {
    int theExpectedInstance = 55;
    int firstAdjectiveInstances[4] = {23, theExpectedInstance, 33, EOF};
    int secondAdjectiveInstances[4] = {24, theExpectedInstance, 33, EOF};
    int theNounInstances[4] = {25, theExpectedInstance, 34, EOF};
    int theExpectedFirstAdjectiveWordIndex = 3;
    int theExpectedSecondAdjectiveWordIndex = 4;
    int theExpectedNounWordIndex = 5;
    Parameter candidates[MAXENTITY+1];
    Parameter parameter = {0, FALSE, theExpectedFirstAdjectiveWordIndex, theExpectedNounWordIndex, candidates};
    
    clearList(candidates);
    
	givenPlayerWordsWithTwoAdjectivesAndANoun(theExpectedFirstAdjectiveWordIndex);

	givenADictionaryWithTwoAdjectivesAndANoun();

    expect(mockedReferenceFinder, want(wordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(wordIndex, theExpectedSecondAdjectiveWordIndex));
    will_return(mockedReferenceFinder, secondAdjectiveInstances);
    expect(mockedReferenceFinder, want(wordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(listLength(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance);
}

static void mockedComplexParameterParser(Parameter candidates[]){
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfList(&candidates[1]);
}

Ensure parseParameterCanFillOutAParameterPosition() {
    Abool flags = OMNIBIT;
    Bool anyPlural;
    Parameter multipleList[MAXPARAMS+1];
    Parameter parameters[MAXENTITY+1];
    ParameterPosition parameterPosition;
    Parameter candidates[MAXENTITY+1];
    
    parameterPosition.candidates = candidates;
    setEndOfList(&candidates[0]);
    
    parseParameterPosition(&parameterPosition, parameters, flags, &anyPlural, multipleList, mockedComplexParameterParser);
    
    assert_equal(listLength(parameterPosition.candidates), 1);
}


static void mockedSimpleCandidateParser(Parameter candidates[]) {
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfList(&candidates[1]);
}

Ensure complexCanFilloutAParameterPositionForSomethingNotAll(void) {
    ParameterPosition *parameterPosition = NEW(ParameterPosition);

    ensureSpaceForPlayerWords(1);
    playerWords[0].code = 1;    /* Should not be "All" */
    playerWords[1].code = EOF;
    wordIndex = 0;

    complexParameterParserDelegate(parameterPosition, mockedSimpleCandidateParser);

    assert_equal(listLength(parameterPosition->candidates), 1);    
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
    add_test(suite, canMatchSingleParameter);
    add_test(suite, matchNounPhraseCanMatchSingleNounWithSingleMatch);
    add_test(suite, canMatchNounAndAdjectiveWithSingleMatch);
    add_test(suite, canMatchMultipleAdjectivesAndNounWithSingleMatch);
    add_test(suite, parseParameterCanFillOutAParameterPosition);
    add_test(suite, complexCanFilloutAParameterPositionForSomethingNotAll);

    return suite;
}

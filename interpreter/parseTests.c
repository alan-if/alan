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
#define ALL_DICTIONARY_INDEX 13
#define PREPOSITION_CODE 1


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
    dictionary[index].string = ((char *)&"all"-(char *)&memory[0])/sizeof(Aword);
}



static ACodeHeader acdHeader;

static void setUp(void) {
    header = &acdHeader;
    header->maxParameters = 10;
    header->instanceMax = 10;

    memory = allocate(100*sizeof(Aword));

    dictionary = makeDictionary();
}


/*----------------------------------------------------------------------*/
static void given_EndOfPlayerWords(void) {
    ensureSpaceForPlayerWords(0);
    playerWords[0].code = EOF;
    wordIndex = 0;
}


/*----------------------------------------------------------------------*/
static void given_AnEmptyParseTree(ElementEntry *elementTable) {
    setEndOfList(elementTable);
}


/*----------------------------------------------------------------------*/
static void given_AParseTreeWithOnlyEos(ElementEntry *elementTable) {
    makeEOS(&elementTable[0]);
    setEndOfList(&elementTable[1]);
}

/*----------------------------------------------------------------------*/
Ensure canMatchEndOfSyntax(void) {
    ElementEntry *element;
    ElementEntry *elementTable;

    elementTable = (ElementEntry *)&memory[20];

    given_EndOfPlayerWords();

    given_AnEmptyParseTree(elementTable);
    element = elementForEndOfSyntax(elementTable);
    assert_equal(NULL, element);

    given_AParseTreeWithOnlyEos(elementTable);
    element = elementForEndOfSyntax(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(EOS, element->code);
}


/*----------------------------------------------------------------------*/
static void given_AParseTreeWithOnlyParameter(ElementEntry *elementTable) {
    makeParameterElement(&elementTable[0]);
    setEndOfList(&elementTable[1]);
}


/*----------------------------------------------------------------------*/
static void given_AParameterTreeWithEosAndParameter(ElementEntry *elementTable) {
    makeEOS(&elementTable[0]);
    makeParameterElement(&elementTable[1]);
    setEndOfList(&elementTable[2]);
}


/*----------------------------------------------------------------------*/
Ensure canMatchParameterElement(void) {
    ElementEntry *element;
    ElementEntry *elementTable;

    elementTable = (ElementEntry *)&memory[50];

    given_AnEmptyParseTree(elementTable);
    element = elementForParameter(elementTable);
    assert_equal(NULL, element);

    given_AParseTreeWithOnlyParameter(elementTable);
    element = elementForParameter(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(0, element->code);

    given_AParameterTreeWithEosAndParameter(elementTable);
    element = elementForParameter(elementTable);
    assert_not_equal(NULL, element);
    assert_equal(0, element->code);
}


/*----------------------------------------------------------------------*/
static void given_AParseTreeWithWordAndEos(ElementEntry *elementTable) {
    makeWordElement(&elementTable[0], 1, 0);
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
}


/*----------------------------------------------------------------------*/
static void given_PlayerInputOfAPreposition() {
    playerWords[0].code = PREPOSITION_CODE;
    playerWords[1].code = EOF;
}


/*----------------------------------------------------------------------*/
static void given_AParseTreeAllowingWordFollowedByEos(ElementEntry *elementTable) {
    makeWordElement(&elementTable[0], 1, addressOf(&elementTable[1]));
    makeEOS(&elementTable[1]);
    setEndOfList(&elementTable[2]);
}


/*----------------------------------------------------------------------*/
Ensure canParseInputAccordingToParseTree(void) {
    ElementEntry *element;
    ElementEntry *elementTable;

    elementTable = (ElementEntry *)&memory[50];
    parameterPositions = NEW(ParameterPosition);
    
    given_EndOfPlayerWords();

    given_AnEmptyParseTree(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions);
    assert_equal(NULL, element);

    given_AParseTreeWithOnlyEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions);
    assert_equal(elementTable, element);

    given_AParseTreeWithWordAndEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions);
    assert_equal(&elementTable[1], element);

    makeDictionaryEntry(PREPOSITION_CODE, 1, PREPOSITION_BIT);
    given_PlayerInputOfAPreposition();
    given_AParseTreeAllowingWordFollowedByEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions);
    assert_equal(&elementTable[1], element);
}

/*----------------------------------------------------------------------*/
Ensure canSetupParameterForWord(void) {
    Parameter *messageParameters;

    dictionary = makeDictionary();

    makeDictionaryEntry(2, 23, VERB_BIT);
    memcpy(&memory[12], "qwerty", 7);
    dictionary[2].string = 12;

    messageParameters = allocateParameterArray(MAXPARAMS);
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
    Parameter *parameters = allocateParameterArray(MAXPARAMS);

    addParameterForInstance(parameters, 2);

    assert_false(isLiteral(parameters[0].instance));
    assert_equal(parameters[0].instance, 2);
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupStringParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(MAXPARAMS);

    addParameterForString(parameters, "a string");

    assert_true(isLiteral(parameters[0].instance));
    assert_string_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, "a string");
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupIntegerParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(MAXPARAMS);

    addParameterForInteger(parameters, 14);

    assert_true(isLiteral(parameters[0].instance));
    assert_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, 14);
    assert_true(isEndOfList(&parameters[1]));

    free(parameters);
}


static void mockedInstanceMatcher(Parameter parameter) {
    mock(parameter.firstWord, parameter.lastWord);
    parameter.candidates[0].instance = 17;
    parameter.candidates[1].instance = -1;
}

/*----------------------------------------------------------------------*/
Ensure canMatchEmptyParameterArray(void) {
    Parameter parameters[2];
    clearParameterList(parameters);

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

    clearParameterList(candidates);
   
    expect(mockedInstanceMatcher,
           want(parameter.firstWord, parameters[0].firstWord),
           want(parameter.lastWord, parameters[0].lastWord));
    
    matchParameters(parameters, mockedInstanceMatcher);

    assert_not_equal(parameters[0].candidates, NULL);
    assert_equal(lengthOfParameterList(parameters[0].candidates), 1);
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

    clearParameterList(candidates);
 
    givenPlayerWordsForANoun(theExpectedWordIndex);

    expect(mockedReferenceFinder, want(wordIndex, theExpectedWordIndex));
    will_return(mockedReferenceFinder, theExpectedInstance);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(lengthOfParameterList(parameter.candidates), 1);
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
    
    clearParameterList(candidates);
    
    givenADictionaryWithTwoAdjectivesAndANoun();

    expect(mockedReferenceFinder, want(wordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(wordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(lengthOfParameterList(parameter.candidates), 1);
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
    
    clearParameterList(candidates);
    
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
    assert_equal(lengthOfParameterList(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance);
}

static void mockedComplexParameterParser(ParameterPosition *parameterPosition){
    mock(parameterPosition);
    parameterPosition->candidates[0].instance = 1;
    setEndOfList(&parameterPosition->candidates[1]);
}

Ensure parseParameterCanFillOutAParameterPosition() {
    Abool flags = OMNIBIT;
    ParameterPosition *parameterPosition = NEW(ParameterPosition);
    Parameter candidates[MAXENTITY+1];
    
    parameterPosition->candidates = candidates;
    setEndOfList(&candidates[0]);
    
    parseParameterPosition(parameterPosition, flags, mockedComplexParameterParser);
    
    assert_equal(lengthOfParameterList(parameterPosition->candidates), 1);
}


static void mockedSimpleCandidateParser(Parameter candidates[]) {
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfList(&candidates[1]);
}

static void givenAPlayerWordOtherThanAll() {
    ensureSpaceForPlayerWords(1);
    playerWords[0].code = 1;    /* Should not be "All" */
    playerWords[1].code = EOF;
    wordIndex = 0;
}


/*----------------------------------------------------------------------*/
Ensure canFilloutAParameterPositionForSomethingNotAll(void) {
    ParameterPosition *parameterPosition = NEW(ParameterPosition);

    givenAPlayerWordOtherThanAll();

    complexParameterParserDelegate(parameterPosition, mockedSimpleCandidateParser, buildAllHere);

    assert_equal(lengthOfParameterList(parameterPosition->candidates), 1);    
}



static void givenADictionaryIncludingAll() {
    dictionary = makeDictionary();
    makeDictionaryEntry(ALL_DICTIONARY_INDEX, 1, ALL_BIT);
}


static void givenPlayerInputReferencingAll(void) {
    ensureSpaceForPlayerWords(1);
    playerWords[0].code = ALL_DICTIONARY_INDEX;    /* Should be "All" */
    playerWords[1].code = EOF;
    wordIndex = 0;
}


void mockedAllBuilder(Parameter candidates[])
{
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfList(&candidates[1]);
}

/*----------------------------------------------------------------------*/
Ensure complexParameterParserDelegateCanSetPlural(void) {
    ParameterPosition *parameterPosition = NEW(ParameterPosition);

    givenADictionaryIncludingAll();
    givenPlayerInputReferencingAll();

    complexParameterParserDelegate(parameterPosition, mockedSimpleCandidateParser, mockedAllBuilder);

    assert_equal(parameterPosition->explicitMultiple, 1);
}


/*----------------------------------------------------------------------*/
Ensure anyAllFindsAnyAllIndication(void) {
    ParameterPosition *parameterPositions = allocate(5*sizeof(ParameterPosition));
    
    parameterPositions[0].endOfList = FALSE;
    parameterPositions[1].endOfList = FALSE;
    parameterPositions[2].endOfList = FALSE;
    parameterPositions[3].endOfList = TRUE;

    parameterPositions[0].all = FALSE;
    parameterPositions[1].all = FALSE;
    parameterPositions[2].all = FALSE;
    parameterPositions[3].all = FALSE;
    parameterPositions[4].all = TRUE;

    assert_false(anyAll(parameterPositions));

    parameterPositions[1].all = TRUE;

    assert_true(anyAll(parameterPositions));
}


/*----------------------------------------------------------------------*/
Ensure anyAllFindsExplicitMultipleIndication(void) {
    ParameterPosition *parameterPositions = allocate(5*sizeof(ParameterPosition));
    
    parameterPositions[0].endOfList = FALSE;
    parameterPositions[1].endOfList = FALSE;
    parameterPositions[2].endOfList = FALSE;
    parameterPositions[3].endOfList = TRUE;

    parameterPositions[0].explicitMultiple = FALSE;
    parameterPositions[1].explicitMultiple = FALSE;
    parameterPositions[2].explicitMultiple = FALSE;
    parameterPositions[3].explicitMultiple = FALSE;
    parameterPositions[4].explicitMultiple = TRUE;

    assert_false(anyExplicitMultiple(parameterPositions));

    parameterPositions[1].explicitMultiple = TRUE;

    assert_true(anyExplicitMultiple(parameterPositions));
}


TestSuite *parseTests(void)
{
    TestSuite *suite = create_test_suite();

    setup(suite, setUp);
    
    add_test(suite, canSetupParameterForWord);
    add_test(suite, canMatchEndOfSyntax);
    add_test(suite, canMatchParameterElement);
    add_test(suite, canParseInputAccordingToParseTree);
    add_test(suite, canSeeBitsInFlag);
    add_test(suite, canSetupInstanceParametersForMessages);
    add_test(suite, canSetupStringParametersForMessages);
    add_test(suite, canSetupIntegerParametersForMessages);
    add_test(suite, canMatchEmptyParameterArray);
    add_test(suite, canMatchSingleParameter);
    add_test(suite, matchNounPhraseCanMatchSingleNounWithSingleMatch);
    add_test(suite, canMatchNounAndAdjectiveWithSingleMatch);
    add_test(suite, canMatchMultipleAdjectivesAndNounWithSingleMatch);
    add_test(suite, parseParameterCanFillOutAParameterPosition);
    add_test(suite, canFilloutAParameterPositionForSomethingNotAll);
    add_test(suite, complexParameterParserDelegateCanSetPlural);
    add_test(suite, anyAllFindsAnyAllIndication);
    add_test(suite, anyAllFindsExplicitMultipleIndication);

    return suite;
}

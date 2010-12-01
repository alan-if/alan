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


#define DICTIONARY_SIZE 25
#define INSTANCE1_CODE 1
#define INSTANCE1_ADJECTIVE1_DICTIONARY_INDEX 3
#define INSTANCE1_ADJECTIVE2_DICTIONARY_INDEX 11
#define INSTANCE1_NOUN1_DICTIONARY_INDEX 5
#define CONJUNCTION_DICTIONARY_INDEX 21
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

static void givenADictionary() {
    dictionary = makeDictionary();
    makeDictionaryEntry(INSTANCE1_ADJECTIVE1_DICTIONARY_INDEX, INSTANCE1_CODE, ADJECTIVE_BIT);
    makeDictionaryEntry(INSTANCE1_ADJECTIVE2_DICTIONARY_INDEX, INSTANCE1_CODE, ADJECTIVE_BIT);
    makeDictionaryEntry(INSTANCE1_NOUN1_DICTIONARY_INDEX, INSTANCE1_CODE, NOUN_BIT);
    makeDictionaryEntry(ALL_DICTIONARY_INDEX, 1, ALL_BIT);
    makeDictionaryEntry(CONJUNCTION_DICTIONARY_INDEX, 1, CONJUNCTION_BIT);
}

static void givenPlayerWordsForANoun(int firstWordIndex) {
    currentWordIndex = firstWordIndex;
    ensureSpaceForPlayerWords(firstWordIndex);
    playerWords[firstWordIndex].code = INSTANCE1_ADJECTIVE1_DICTIONARY_INDEX;
}

static void givenPlayerWordsWithTwoAdjectivesAndANoun(int firstWordIndex) {
    currentWordIndex = firstWordIndex;
    ensureSpaceForPlayerWords(firstWordIndex+2);
    playerWords[firstWordIndex].code = INSTANCE1_ADJECTIVE1_DICTIONARY_INDEX;
    playerWords[firstWordIndex+1].code = INSTANCE1_ADJECTIVE2_DICTIONARY_INDEX;
    playerWords[firstWordIndex+2].code = INSTANCE1_NOUN1_DICTIONARY_INDEX;
}

static void givenPlayerWordsForTwoParameters(int firstWordIndex) {
    currentWordIndex = firstWordIndex;
    ensureSpaceForPlayerWords(firstWordIndex+2);
    playerWords[firstWordIndex].code = INSTANCE1_NOUN1_DICTIONARY_INDEX;
    playerWords[firstWordIndex+1].code = CONJUNCTION_DICTIONARY_INDEX;
    playerWords[firstWordIndex+2].code = INSTANCE1_NOUN1_DICTIONARY_INDEX;
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
    currentWordIndex = 0;
}


/*----------------------------------------------------------------------*/
static void given_AnEmptyParseTree(ElementEntry *elementTable) {
    setEndOfArray(elementTable);
}


/*----------------------------------------------------------------------*/
static void given_AParseTreeWithOnlyEos(ElementEntry *elementTable) {
    makeEOS(&elementTable[0]);
    setEndOfArray(&elementTable[1]);
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
    setEndOfArray(&elementTable[1]);
}


/*----------------------------------------------------------------------*/
static void given_AParameterTreeWithEosAndParameter(ElementEntry *elementTable) {
    makeEOS(&elementTable[0]);
    makeParameterElement(&elementTable[1]);
    setEndOfArray(&elementTable[2]);
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
    setEndOfArray(&elementTable[2]);
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
    setEndOfArray(&elementTable[2]);
}


/*----------------------------------------------------------------------*/
Ensure canParseInputAccordingToParseTree(void) {

    int ELEMENT_TABLE_ADDRESS = 50;
    ElementEntry *element;
    ElementEntry *elementTable;
    elementTable = (ElementEntry *)&memory[ELEMENT_TABLE_ADDRESS];

    ParameterPosition *parameterPositions = NEW(ParameterPosition);

    SyntaxEntry stx;
    stx.elms = ELEMENT_TABLE_ADDRESS;

    given_EndOfPlayerWords();

    given_AnEmptyParseTree(elementTable);
    element = parseInputAccordingToSyntax(&stx, parameterPositions);
    assert_equal(NULL, element);

    given_AParseTreeWithOnlyEos(elementTable);
    element = parseInputAccordingToSyntax(&stx, parameterPositions);
    assert_equal(elementTable, element);

    given_AParseTreeWithWordAndEos(elementTable);
    element = parseInputAccordingToSyntax(&stx, parameterPositions);
    assert_equal(&elementTable[1], element);

    makeDictionaryEntry(PREPOSITION_CODE, 1, PREPOSITION_BIT);
    given_PlayerInputOfAPreposition();
    given_AParseTreeAllowingWordFollowedByEos(elementTable);
    element = parseInputAccordingToSyntax(&stx, parameterPositions);
    assert_equal(&elementTable[1], element);
}

/*----------------------------------------------------------------------*/
Ensure canSetupParameterForWord(void) {
    Parameter *messageParameters;

    dictionary = makeDictionary();

    makeDictionaryEntry(2, 23, VERB_BIT);
    memcpy(&memory[12], "qwerty", 7);
    dictionary[2].string = 12;

    messageParameters = allocateParameterArray();
    ensureSpaceForPlayerWords(2);
    playerWords[1].code = 2;
    litCount = 0;
    addParameterForWord(messageParameters, 1);

    assert_equal(instanceFromLiteral(1), messageParameters[0].instance);
    assert_equal(TRUE, messageParameters[0].useWords);
    assert_equal(1, messageParameters[0].firstWord);
    assert_equal(1, messageParameters[0].lastWord);

    assert_true(isEndOfArray(&messageParameters[1]));

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
    Parameter *parameters = allocateParameterArray();

    addParameterForInstance(parameters, 2);

    assert_false(isLiteral(parameters[0].instance));
    assert_equal(parameters[0].instance, 2);
    assert_true(isEndOfArray(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupStringParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray();

    addParameterForString(parameters, "a string");

    assert_true(isLiteral(parameters[0].instance));
    assert_string_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, "a string");
    assert_true(isEndOfArray(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupIntegerParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray();

    addParameterForInteger(parameters, 14);

    assert_true(isLiteral(parameters[0].instance));
    assert_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, 14);
    assert_true(isEndOfArray(&parameters[1]));

    free(parameters);
}


static void mockedInstanceMatcher(Parameter *parameter) {
    mock(parameter->firstWord, parameter->lastWord);
    parameter->candidates[0].instance = 17;
    parameter->candidates[1].instance = -1;
}

/*----------------------------------------------------------------------*/
Ensure canMatchSingleParameter(void) {
    Parameter parameters[2];

    clearParameter(&parameters[0], NULL);
    parameters[0].firstWord = 1;
    parameters[0].lastWord = 1;
    parameters[0].candidates = NULL;
    setEndOfArray(&parameters[1]);

    playerWords = allocate(10);

    expect(mockedInstanceMatcher,
           want(parameter.firstWord, parameters[0].firstWord),
           want(parameter.lastWord, parameters[0].lastWord));
    
    findCandidates(parameters, mockedInstanceMatcher);

    assert_not_equal(parameters[0].candidates, NULL);
    assert_equal(lengthOfParameterArray(parameters[0].candidates), 1);
    assert_equal(parameters[0].candidates[0].instance, 17);
}




static Aint *mockedReferenceFinder(int wordIndex) {
    return (Aint *)mock(wordIndex);
}

/*----------------------------------------------------------------------*/
Ensure matchNounPhraseCanMatchSingleNounWithSingleMatch(void) {
    int theExpectedInstance[2] = {23, EOF};
    int theExpectedWordIndex = 3;
    Parameter candidates[MAXENTITY+1];
    Parameter parameter;

    clearParameter(&parameter, candidates);
    parameter.firstWord = parameter.lastWord = 3;

    givenPlayerWordsForANoun(theExpectedWordIndex);

    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedWordIndex));
    will_return(mockedReferenceFinder, theExpectedInstance);

    matchNounPhrase(&parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(lengthOfParameterArray(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance[0]);
}


/*----------------------------------------------------------------------*/
Ensure canMatchNounAndAdjectiveWithSingleMatch(void) {
    int theExpectedInstance = 55;
    int firstAdjectiveInstances[4] = {23, theExpectedInstance, 33, EOF};
    int theNounInstances[4] = {25, theExpectedInstance, 34, EOF};
    int theExpectedFirstAdjectiveWordIndex = 3;
    int theExpectedNounWordIndex = 4;
    Parameter candidates[MAXENTITY+1];
    Parameter parameter;
    clearParameter(&parameter, candidates);
    parameter.firstWord = theExpectedFirstAdjectiveWordIndex;
    parameter.lastWord = theExpectedNounWordIndex;

    givenADictionary();

    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(&parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(lengthOfParameterArray(parameter.candidates), 1);
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
    Parameter *candidates = allocateParameterArray();
    Parameter *parameter = allocateParameterArray();
    clearParameter(parameter, candidates);
    parameter[0].firstWord = theExpectedFirstAdjectiveWordIndex;
    parameter[0].lastWord = theExpectedNounWordIndex;

    clearParameterArray(candidates);
    
    givenPlayerWordsWithTwoAdjectivesAndANoun(theExpectedFirstAdjectiveWordIndex);

    givenADictionary();

    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedSecondAdjectiveWordIndex));
    will_return(mockedReferenceFinder, secondAdjectiveInstances);
    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter[0].candidates, NULL);
    assert_equal(lengthOfParameterArray(parameter[0].candidates), 1);
    assert_equal(parameter[0].candidates[0].instance, theExpectedInstance);
}

void mockedAllBuilder(Parameter candidates[])
{
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfArray(&candidates[1]);
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

/*----------------------------------------------------------------------*/
Ensure parsePronounSetsPronounMarker(void) {
    Parameter parameter[2];
    clearParameter(&parameter[0], NULL);

    setEndOfArray(&parameter[0]);
    parameter[0].isPronoun = FALSE;

    parsePronoun(&parameter[0]);

    assert_true(parameter[0].isPronoun);
    assert_false(isEndOfArray(&parameter[0]));
    assert_true(isEndOfArray(&parameter[1]));
}

/*----------------------------------------------------------------------*/
Ensure parseLiteralSetsLiteralMarker(void) {
    Parameter parameter[2];

    setEndOfArray(&parameter[0]);
    parameter[0].isLiteral = FALSE;

    parseLiteral(&parameter[0]);

    assert_true(parameter[0].isLiteral);
    assert_false(isEndOfArray(&parameter[0]));
    assert_true(isEndOfArray(&parameter[1]));
}

/*----------------------------------------------------------------------*/
Ensure parseReferenceToPreviousMultipleParameterSetsThemMarker(void) {
    Parameter parameter[2];

    setEndOfArray(&parameter[0]);
    parameter[0].isThem = FALSE;

    parseReferenceToPreviousMultipleParameters(&parameter[0]);

    assert_true(parameter[0].isThem);
    assert_false(isEndOfArray(&parameter[0]));
    assert_true(isEndOfArray(&parameter[1]));
}

/*----------------------------------------------------------------------*/
Ensure simpleParameterParserCanParseExplicitMultiple(void) {
    Parameter *parameters = allocateParameterArray();

    givenADictionary();
    givenPlayerWordsForTwoParameters(1);

    simpleParameterParser(parameters);

    assert_equal(lengthOfParameterArray(parameters), 2);
}


/*----------------------------------------------------------------------*/
Ensure getPreviousMultipleParametersSetsEndOfArray(void) {
    Parameter parameters[2];
    Parameter multipleParameters[2];
    previousMultipleParameters = multipleParameters;
    assert_true(!isEndOfArray(&parameters[0]));
    setEndOfArray(&previousMultipleParameters[0]);
    getPreviousMultipleParameters(parameters);
    assert_true(isEndOfArray(&parameters[0]));
}

/*----------------------------------------------------------------------*/
Ensure parseAdjectivesAndNounsReturnsEmptyParametersOnEndOfInput() {
    Parameter parameters[2];
    given_EndOfPlayerWords();
    
    parseAdjectivesAndNoun(parameters);
    assert_equal(lengthOfParameterArray(parameters), 0);
}


// TODO: Generalise this to replace lengthOfParameterArray() and other loops
static int lengthOfArray(Pronoun *array, int elementSize) {
    int length;
    for (length = 0; !isEndOfArray(&array[length]); length++)
        ;
    return length;
}

/*----------------------------------------------------------------------*/
Ensure addPronounForInstanceDontAddSameTwice() {
    pronouns = allocate(2*sizeof(Pronoun)+1);

    pronouns[0].pronoun = 10;
    pronouns[0].instance = 3;
    setEndOfArray(&pronouns[1]);

    assert_equal(lengthOfArray(pronouns, sizeof(Pronoun)), 1);
    addPronounForInstance(7, 3);
    assert_equal(lengthOfArray(pronouns, sizeof(Pronoun)), 2);
    addPronounForInstance(10, 3);
    assert_equal(lengthOfArray(pronouns, sizeof(Pronoun)), 2);
}


static int mockedReachable(int instance) {
    return instance == 1;
}

static Bool handlerFor00NCalled = FALSE;
static Bool handlerFor01NCalled = FALSE;
static Bool handlerFor0MNCalled = FALSE;
static Bool handlerFor10NCalled = FALSE;
static Bool handlerFor11NCalled = FALSE;
static Bool handlerFor1MNCalled = FALSE;
static Bool handlerForM0NCalled = FALSE;
static Bool handlerForM1NCalled = FALSE;
static Bool handlerForMMNCalled = FALSE;
static Bool handlerFor00YCalled = FALSE;
static Bool handlerFor01YCalled = FALSE;
static Bool handlerFor0MYCalled = FALSE;
static Bool handlerFor10YCalled = FALSE;
static Bool handlerFor11YCalled = FALSE;
static Bool handlerFor1MYCalled = FALSE;
static Bool handlerForM0YCalled = FALSE;
static Bool handlerForM1YCalled = FALSE;
static Bool handlerForMMYCalled = FALSE;

static Parameter *mocked00NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor00NCalled = TRUE; return allCandidates; }
static Parameter *mocked01NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor01NCalled = TRUE; return allCandidates; }
static Parameter *mocked0MNHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor0MNCalled = TRUE; return allCandidates; }
static Parameter *mocked10NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor10NCalled = TRUE; return allCandidates; }
static Parameter *mocked11NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor11NCalled = TRUE; return allCandidates; }
static Parameter *mocked1MNHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor1MNCalled = TRUE; return allCandidates; }
static Parameter *mockedM0NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForM0NCalled = TRUE; return allCandidates; }
static Parameter *mockedM1NHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForM1NCalled = TRUE; return allCandidates; }
static Parameter *mockedMMNHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForMMNCalled = TRUE; return allCandidates; }
static Parameter *mocked00YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor00YCalled = TRUE; return allCandidates; }
static Parameter *mocked01YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor01YCalled = TRUE; return allCandidates; }
static Parameter *mocked0MYHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor0MYCalled = TRUE; return allCandidates; }
static Parameter *mocked10YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor10YCalled = TRUE; return allCandidates; }
static Parameter *mocked11YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor11YCalled = TRUE; return allCandidates; }
static Parameter *mocked1MYHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerFor1MYCalled = TRUE; return allCandidates; }
static Parameter *mockedM0YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForM0YCalled = TRUE; return allCandidates; }
static Parameter *mockedM1YHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForM1YCalled = TRUE; return allCandidates; }
static Parameter *mockedMMYHandler(Parameter allCandidates[], Parameter presentCandidates[]) { handlerForMMYCalled = TRUE; return allCandidates; }

static DisambiguationHandlerTable mockedHandlerTable =
    {   
        {   // Present == 0
            {   // Distant == 0
                mocked00NHandler, mocked00YHandler},
            {   // Distant == 1
                mocked01NHandler, mocked01YHandler},
            {   // Distant == M
                mocked0MNHandler, mocked0MYHandler}},
        {   //  Present == 1
            {   // Distant == 0
                mocked10NHandler, mocked10YHandler},
            {   // Distant == 1
                mocked11NHandler, mocked11YHandler},
            {   // Distant == M
                mocked1MNHandler, mocked1MYHandler}},
        {   // Present == M
            {   // Distant == 0
                mockedM0NHandler, mockedM0YHandler},
            {   // Distant == 1
                mockedM1NHandler, mockedM1YHandler},
            {   // Distant == M
                mockedMMNHandler, mockedMMYHandler}}
    };
            
    
/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall00NHandler() {
    Parameter *candidates = allocateParameterArray();
    setEndOfArray(&candidates[0]); /* == 0 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor00NCalled);
}


/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall00YHandler() {
    Parameter *candidates = allocateParameterArray();
    setEndOfArray(&candidates[0]); /* == 0 instance */

    disambiguateCandidates(candidates, TRUE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor00YCalled);
}


/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall01NHandler() {
    Parameter *candidates = allocateParameterArray();
    candidates[0].instance = 2; /* 1 non-present */
    setEndOfArray(&candidates[1]); /* == 1 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor01NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall0MNHandler() {
    Parameter *candidates = allocateParameterArray();
    candidates[0].instance = 2; /* M non-present */
    candidates[1].instance = 2;
    setEndOfArray(&candidates[2]); /* == 2 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor0MNCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall10NHandler() {
    Parameter *candidates = allocateParameterArray();
    candidates[0].instance = 1; /* 1 present */
    setEndOfArray(&candidates[1]); /* == 1 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor10NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall11NHandler() {
    Parameter *candidates = allocateParameterArray();
    candidates[0].instance = 1; /* 1 present */
    candidates[1].instance = 2; /* 1 non-present */
    setEndOfArray(&candidates[2]); /* == 2 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor11NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall1MNHandler() {
    Parameter *candidates = allocateParameterArray();
    candidates[0].instance = 1; /* 1 present */
    candidates[1].instance = 2; /* M non-present */
    candidates[2].instance = 2;
    setEndOfArray(&candidates[3]); /* == 3 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, mockedHandlerTable);
    assert_true(handlerFor1MNCalled);
}

TestSuite *parseTests(void)
{
    TestSuite *suite = create_test_suite();

    setup(suite, setUp);
    
    add_test(suite, canMatchEndOfSyntax);
    add_test(suite, canSetupParameterForWord);
    add_test(suite, canMatchParameterElement);
    add_test(suite, canParseInputAccordingToParseTree);
    add_test(suite, canSeeBitsInFlag);
    add_test(suite, canSetupInstanceParametersForMessages);
    add_test(suite, canSetupStringParametersForMessages);
    add_test(suite, canSetupIntegerParametersForMessages);
    add_test(suite, canMatchSingleParameter);
    add_test(suite, matchNounPhraseCanMatchSingleNounWithSingleMatch);
    add_test(suite, canMatchNounAndAdjectiveWithSingleMatch);
    add_test(suite, canMatchMultipleAdjectivesAndNounWithSingleMatch);
    add_test(suite, anyAllFindsAnyAllIndication);
    add_test(suite, anyAllFindsExplicitMultipleIndication);
    add_test(suite, parseLiteralSetsLiteralMarker);
    add_test(suite, parsePronounSetsPronounMarker);
    add_test(suite, parseReferenceToPreviousMultipleParameterSetsThemMarker);
    add_test(suite, simpleParameterParserCanParseExplicitMultiple);
    add_test(suite, getPreviousMultipleParametersSetsEndOfArray);
    add_test(suite, parseAdjectivesAndNounsReturnsEmptyParametersOnEndOfInput);
    add_test(suite, addPronounForInstanceDontAddSameTwice);
    add_test(suite, disambiguateCandidatesCanCall00NHandler);
    add_test(suite, disambiguateCandidatesCanCall01NHandler);
    add_test(suite, disambiguateCandidatesCanCall0MNHandler);
    add_test(suite, disambiguateCandidatesCanCall10NHandler);
    add_test(suite, disambiguateCandidatesCanCall11NHandler);
    add_test(suite, disambiguateCandidatesCanCall1MNHandler);
    add_test(suite, disambiguateCandidatesCanCall00YHandler);


    return suite;
}

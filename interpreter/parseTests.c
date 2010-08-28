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

static void givenPlayerInputReferencingAll(void) {
    ensureSpaceForPlayerWords(1);
    playerWords[0].code = ALL_DICTIONARY_INDEX;    /* Should be "All" */
    playerWords[1].code = EOF;
    currentWordIndex = 0;
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
    ElementEntry *element;
    ElementEntry *elementTable;

    elementTable = (ElementEntry *)&memory[50];
    parameterPositions = NEW(ParameterPosition);
    ParameterPosition *parameterPositions2 = NEW(ParameterPosition);
    
    given_EndOfPlayerWords();

    given_AnEmptyParseTree(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions, parameterPositions2);
    assert_equal(NULL, element);

    given_AParseTreeWithOnlyEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions, parameterPositions2);
    assert_equal(elementTable, element);

    given_AParseTreeWithWordAndEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions, parameterPositions2);
    assert_equal(&elementTable[1], element);

    makeDictionaryEntry(PREPOSITION_CODE, 1, PREPOSITION_BIT);
    given_PlayerInputOfAPreposition();
    given_AParseTreeAllowingWordFollowedByEos(elementTable);
    element = parseInputAccordingToElementTree(elementTable, parameterPositions, parameterPositions2);
    assert_equal(&elementTable[1], element);
}

/*----------------------------------------------------------------------*/
Ensure canSetupParameterForWord(void) {
    Parameter *messageParameters;

    dictionary = makeDictionary();

    makeDictionaryEntry(2, 23, VERB_BIT);
    memcpy(&memory[12], "qwerty", 7);
    dictionary[2].string = 12;

    messageParameters = allocateParameterArray(MAXPARAMS+1);
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
    Parameter *parameters = allocateParameterArray(MAXPARAMS+1);

    addParameterForInstance(parameters, 2);

    assert_false(isLiteral(parameters[0].instance));
    assert_equal(parameters[0].instance, 2);
    assert_true(isEndOfArray(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupStringParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(MAXPARAMS+1);

    addParameterForString(parameters, "a string");

    assert_true(isLiteral(parameters[0].instance));
    assert_string_equal((char *)literals[literalFromInstance(parameters[0].instance)].value, "a string");
    assert_true(isEndOfArray(&parameters[1]));

    free(parameters);
}


/*----------------------------------------------------------------------*/
Ensure canSetupIntegerParametersForMessages(void) {
    Parameter *parameters = allocateParameterArray(MAXPARAMS);

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
    Parameter candidates[2];

    clearParameter(&parameters[0], NULL);
    parameters[0].firstWord = 1;
    parameters[0].lastWord = 1;
    parameters[0].candidates = NULL;
    setEndOfArray(&parameters[1]);

    playerWords = allocate(10);

    clearParameterArray(candidates);
   
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

    clearParameterArray(candidates);
 
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

    clearParameterArray(candidates);
    
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
    Parameter candidates[MAXENTITY+1];
    Parameter parameter;
    clearParameter(&parameter, candidates);
    parameter.firstWord = theExpectedFirstAdjectiveWordIndex;
    parameter.lastWord = theExpectedNounWordIndex;

    clearParameterArray(candidates);
    
    givenPlayerWordsWithTwoAdjectivesAndANoun(theExpectedFirstAdjectiveWordIndex);

    givenADictionary();

    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedFirstAdjectiveWordIndex));
    will_return(mockedReferenceFinder, firstAdjectiveInstances);
    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedSecondAdjectiveWordIndex));
    will_return(mockedReferenceFinder, secondAdjectiveInstances);
    expect(mockedReferenceFinder, want(currentWordIndex, theExpectedNounWordIndex));
    will_return(mockedReferenceFinder, theNounInstances);

    matchNounPhrase(&parameter, mockedReferenceFinder, mockedReferenceFinder);
    
    assert_not_equal(parameter.candidates, NULL);
    assert_equal(lengthOfParameterArray(parameter.candidates), 1);
    assert_equal(parameter.candidates[0].instance, theExpectedInstance);
}

static void mockedComplexParameterParser(ParameterPosition *parameterPosition){
    mock(parameterPosition);
    parameterPosition->parameters[0].instance = 1;
    setEndOfArray(&parameterPosition->parameters[1]);
}


Ensure parseAndBuildParameterPositionCanFillOutAParameterPosition() {
    Abool flags = OMNIBIT;
    ParameterPosition *parameterPosition = NEW(ParameterPosition);
    Parameter candidates[MAXENTITY+1];
    
    parameterPosition->parameters = candidates;
    setEndOfArray(&candidates[0]);
    
    parseAndBuildParameterPosition(parameterPosition, flags, mockedComplexParameterParser);
    
    assert_equal(lengthOfParameterArray(parameterPosition->parameters), 1);
}


static void mockedSimpleCandidateParser(Parameter candidates[]) {
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfArray(&candidates[1]);
}

static void givenAPlayerWordOtherThanAll() {
    ensureSpaceForPlayerWords(1);
    playerWords[0].code = 1;    /* Should not be "All" */
    playerWords[1].code = EOF;
    currentWordIndex = 0;
}


/*----------------------------------------------------------------------*/
Ensure canFilloutAParameterPositionForSomethingNotAll(void) {
    ParameterPosition *parameterPosition = NEW(ParameterPosition);

    givenAPlayerWordOtherThanAll();

    complexParameterParserAndBuilderDelegate(parameterPosition, mockedSimpleCandidateParser, buildAllHere);

    assert_equal(lengthOfParameterArray(parameterPosition->parameters), 1);    
}



void mockedAllBuilder(Parameter candidates[])
{
    mock(candidates);
    candidates[0].instance = 1;
    setEndOfArray(&candidates[1]);
}

/*----------------------------------------------------------------------*/
Ensure complexParameterParserDelegateCanSetPlural(void) {
    ParameterPosition *parameterPosition = NEW(ParameterPosition);

    givenADictionary();
    givenPlayerInputReferencingAll();

    complexParameterParserAndBuilderDelegate(parameterPosition, mockedSimpleCandidateParser, mockedAllBuilder);

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
    Parameter parameters[3];
    clearParameterArray(parameters);

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

static void mocked00NHandler() { handlerFor00NCalled = TRUE; }
static void mocked01NHandler() { handlerFor01NCalled = TRUE; }
static void mocked0MNHandler() { handlerFor0MNCalled = TRUE; }
static void mocked10NHandler() { handlerFor10NCalled = TRUE; }
static void mocked11NHandler() { handlerFor11NCalled = TRUE; }
static void mocked1MNHandler() { handlerFor1MNCalled = TRUE; }
static void mockedM0NHandler() { handlerForM0NCalled = TRUE; }
static void mockedM1NHandler() { handlerForM1NCalled = TRUE; }
static void mockedMMNHandler() { handlerForMMNCalled = TRUE; }
static void mocked00YHandler() { handlerFor00YCalled = TRUE; }
static void mocked01YHandler() { handlerFor01YCalled = TRUE; }
static void mocked0MYHandler() { handlerFor0MYCalled = TRUE; }
static void mocked10YHandler() { handlerFor10YCalled = TRUE; }
static void mocked11YHandler() { handlerFor11YCalled = TRUE; }
static void mocked1MYHandler() { handlerFor1MYCalled = TRUE; }
static void mockedM0YHandler() { handlerForM0YCalled = TRUE; }
static void mockedM1YHandler() { handlerForM1YCalled = TRUE; }
static void mockedMMYHandler() { handlerForMMYCalled = TRUE; }

static DisambiguationHandlerTable handlerTable =
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
    Parameter candidates[1];
    setEndOfArray(&candidates[0]); /* == 0 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
    assert_true(handlerFor00NCalled);
}


/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall00YHandler() {
    Parameter candidates[1];
    setEndOfArray(&candidates[0]); /* == 0 instance */

    disambiguateCandidates(candidates, TRUE, mockedReachable, handlerTable);
    assert_true(handlerFor00YCalled);
}


/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall01NHandler() {
    Parameter candidates[2];
    candidates[0].instance = 2; /* 1 non-present */
    setEndOfArray(&candidates[1]); /* == 1 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
    assert_true(handlerFor01NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall0MNHandler() {
    Parameter candidates[3];
    candidates[0].instance = 2; /* M non-present */
    candidates[1].instance = 2;
    setEndOfArray(&candidates[2]); /* == 2 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
    assert_true(handlerFor0MNCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall10NHandler() {
    Parameter candidates[2];
    candidates[0].instance = 1; /* 1 present */
    setEndOfArray(&candidates[1]); /* == 1 instance */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
    assert_true(handlerFor10NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall11NHandler() {
    Parameter candidates[3];
    candidates[0].instance = 1; /* 1 present */
    candidates[1].instance = 2; /* 1 non-present */
    setEndOfArray(&candidates[2]); /* == 2 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
    assert_true(handlerFor11NCalled);
}

/*----------------------------------------------------------------------*/
Ensure disambiguateCandidatesCanCall1MNHandler() {
    Parameter candidates[3];
    candidates[0].instance = 1; /* 1 present */
    candidates[1].instance = 2; /* M non-present */
    candidates[2].instance = 2;
    setEndOfArray(&candidates[3]); /* == 3 instances */

    disambiguateCandidates(candidates, FALSE, mockedReachable, handlerTable);
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
    add_test(suite, parseAndBuildParameterPositionCanFillOutAParameterPosition);
    add_test(suite, canFilloutAParameterPositionForSomethingNotAll);
    add_test(suite, complexParameterParserDelegateCanSetPlural);
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

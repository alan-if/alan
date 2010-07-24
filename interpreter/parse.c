/*----------------------------------------------------------------------

  parse.c
 
  Command line parser unit for Alan interpreter ARUN
 
  ----------------------------------------------------------------------*/
#include "parse.h"


/* IMPORTS */
#include <stdio.h>
#include <ctype.h>

#include "AltInfo.h"
#include "inter.h"
#include "current.h"
#include "act.h"
#include "term.h"
#include "lists.h"
#include "options.h"
#include "syserr.h"
#include "Location.h"
#include "instance.h"
#include "memory.h"
#include "output.h"
#include "dictionary.h"
#include "syntax.h"
#include "word.h"
#include "msg.h"
#include "literal.h"
#include "parameterPosition.h"

#include "scan.h"


/* PRIVATE CONSTANTS */


/* PUBLIC DATA */


/* PRIVATE TYPES */
typedef struct PronounEntry { /* To remember parameter/pronoun relations */
    int pronoun;
    int instance;
} Pronoun;

/*----------------------------------------------------------------------*/
static void clearPronounList(Pronoun list[]) {
    implementationOfSetEndOfList((Aword *)list);
}


typedef Aint *(*ReferencesFinder)(int wordIndex);
typedef void (*ParameterParser)(Parameter parameters[]);



/* PRIVATE DATA */
static Pronoun *pronouns = NULL;
static int allWordIndex;        /* Word index of the ALL_WORD found */


/* Syntax Parameters */
static Parameter *previousMultipleParameters; /* Previous multiple list */


/* For parameters that are literals we need to trick message handling to
 * output the word and create a string literal instance if anyone wants to
 * refer to an attribute of it (literals inherit from entity so application
 * can have added an attribute) */

/*----------------------------------------------------------------------*/
static void addParameterForWord(Parameter *parameters, int wordIndex) {
    Parameter *parameter = findEndOfParameterList(parameters);
	
    createStringLiteral(pointerTo(dictionary[playerWords[wordIndex].code].string));
    parameter->instance = instanceFromLiteral(litCount); /* A faked literal */
    parameter->useWords = TRUE;
    parameter->firstWord = parameter->lastWord = wordIndex;
    setEndOfList(parameter+1);
}


#ifdef UNDEF_WHEN_NEEDED
/*----------------------------------------------------------------------*/
static void addParameterForWords(Parameter *parameters, int firstWordIndex, int lastWordIndex) {
    Parameter *parameter = findEndOfList(parameters);
	
    // TODO Need to concat the words, instead of just using the last word
    createStringLiteral(pointerTo(dictionary[playerWords[lastWordIndex].code].string));
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = TRUE;
    parameter->firstWord = firstWordIndex;
    parameter->lastWord = lastWordIndex;
    setEndOfList(parameter+1);
}
#endif


/*----------------------------------------------------------------------*/
static Pronoun *allocatePronounArray(Pronoun *currentList) {
    if (currentList == NULL)
        currentList = allocate(sizeof(Pronoun)*(MAXPARAMS+1));
    clearPronounList(currentList);
    return currentList;
}


/*----------------------------------------------------------------------*/
static Bool endOfWords(int wordIndex) {
    return isEndOfList(&playerWords[wordIndex]);
}


/*----------------------------------------------------------------------*/
static void nonverb(void) {
    if (isDirectionWord(currentWordIndex)) {
        currentWordIndex++;
        if (!endOfWords(currentWordIndex) && !isConjunctionWord(currentWordIndex))
            error(M_WHAT);
        else
            go(current.location, dictionary[playerWords[currentWordIndex-1].code].code);
        if (!endOfWords(currentWordIndex))
            currentWordIndex++;
    } else
        error(M_WHAT);
}

/*----------------------------------------------------------------------*/
static void errorWhichOne(Parameter alternative[]) {
    int p; /* Index into the list of alternatives */
    Parameter *parameters = allocateParameterArray(MAXPARAMS);
	
    parameters[0] = alternative[0];
    setEndOfList(&parameters[1]);
    printMessageWithParameters(M_WHICH_ONE_START, parameters);
    for (p = 1; !isEndOfList(&alternative[p+1]); p++) {
        parameters[0] = alternative[p];
        printMessageWithParameters(M_WHICH_ONE_COMMA, parameters);
    }
    parameters[0] = alternative[p];
    printMessageWithParameters(M_WHICH_ONE_OR, parameters);
    free(parameters);
    abortPlayerCommand(); /* Return with empty error message */
}

/*----------------------------------------------------------------------*/
static void errorWhichPronoun(int pronounWordIndex, Parameter alternatives[]) {
    int p; /* Index into the list of alternatives */
    Parameter *messageParameters = allocateParameterArray(MAXPARAMS);
	
    addParameterForWord(messageParameters, pronounWordIndex);
    printMessageWithParameters(M_WHICH_PRONOUN_START, messageParameters);
	
    messageParameters[0] = alternatives[0];
    setEndOfList(&messageParameters[1]);
    printMessageWithParameters(M_WHICH_PRONOUN_FIRST, messageParameters);
	
    for (p = 1; !isEndOfList(&alternatives[p+1]); p++) {
        messageParameters[0] = alternatives[p];
        printMessageWithParameters(M_WHICH_ONE_COMMA, messageParameters);
    }
    messageParameters[0] = alternatives[p];
    printMessageWithParameters(M_WHICH_ONE_OR, messageParameters);
    free(messageParameters);
    abortPlayerCommand();
}

/*----------------------------------------------------------------------*/
static void errorWhat(int playerWordIndex) {
    Parameter *messageParameters = allocateParameterArray(MAXPARAMS);
    
    addParameterForWord(messageParameters, playerWordIndex);
    printMessageWithParameters(M_WHAT_WORD, messageParameters);
    free(messageParameters);
    abortPlayerCommand();
}

/*----------------------------------------------------------------------*/
static void errorAfterExcept(int butWordIndex) {
    Parameter *messageParameters = allocateParameterArray(MAXPARAMS);
    addParameterForWord(messageParameters, butWordIndex);
    printMessageWithParameters(M_AFTER_BUT, messageParameters);
    free(messageParameters);
    abortPlayerCommand();
}

/*----------------------------------------------------------------------*/
static int fakePlayerWordForAll() {
    /* Look through the dictionary and find any ALL_WORD, then add a
       player word so that it can be used in the message */
    int p, d;
	
    for (p = 0; !isEndOfList(&playerWords[p]); p++)
        ;
    setEndOfList(&playerWords[p+1]); /* Make room for one more word */
    allWordIndex = p;
    for (d = 0; d < dictionarySize; d++)
        if (isAll(d)) {
            playerWords[p].code = d;
            return p;
        }
    syserr("No ALLWORD found");
    return 0;
}

/*----------------------------------------------------------------------*/
static void errorButAfterAll(int butWordIndex) {
    Parameter *messageParameters = allocateParameterArray(MAXPARAMS);
    addParameterForWord(messageParameters, butWordIndex);
    addParameterForWord(messageParameters, fakePlayerWordForAll());
    printMessageWithParameters(M_BUT_ALL, messageParameters);
    free(messageParameters);
    abortPlayerCommand();
}

/*----------------------------------------------------------------------*/
static Aint findInstanceForNoun(int wordIndex) {
    DictionaryEntry *d = &dictionary[wordIndex];
    if (d->nounRefs == 0 || d->nounRefs == EOF)
        syserr("No references for noun");
    return *(Aint*) pointerTo(d->nounRefs);
}

/*----------------------------------------------------------------------*/
static void errorNoSuch(Parameter parameter) {
    globalParameters[0] = parameter;
	
    /* If there was no instance, assume the last word used is the noun,
     * then find any instance with the noun he used */
    if (globalParameters[0].instance == 0)
        globalParameters[0].instance = findInstanceForNoun(playerWords[parameter.lastWord].code);
    globalParameters[0].useWords = TRUE; /* Indicate to use words and not names */
    setEndOfList(&globalParameters[1]);
    error(M_NO_SUCH);
}

/*----------------------------------------------------------------------*/
static void buildAllHere(Parameter list[]) {
    int o, i = 0;
    Bool found = FALSE;
	
    for (o = 1; o <= header->instanceMax; o++)
        if (isHere(o, FALSE)) {
            found = TRUE;
            list[i].instance = o;
            list[i++].firstWord = EOF;
        }
    if (!found)
        errorWhat(currentWordIndex);
    else
        setEndOfList(&list[i]);
    allWordIndex = currentWordIndex;
}


/*----------------------------------------------------------------------*/
static Bool endOfPronouns(int pronounIndex) {
    return isEndOfList(&pronouns[pronounIndex]);
}


/*----------------------------------------------------------------------*/
static int getPronounInstances(int word, Parameter instanceParameters[]) {
    /* Find the instance that the pronoun word could refer to, return 0
       if none or multiple */
    int p;
    int instanceCount = 0;
	
    clearParameterList(instanceParameters);
    for (p = 0; !endOfPronouns(p); p++)
        if (pronouns[p].instance != 0 && dictionary[word].code == pronouns[p].pronoun) {
            instanceParameters[instanceCount].instance = pronouns[p].instance;
            instanceParameters[instanceCount].useWords = FALSE; /* Can't use words since they are gone, pronouns
								   refer to parameters in previous command */
            setEndOfList(&instanceParameters[++instanceCount]);
        }
    return instanceCount;
}

/*----------------------------------------------------------------------*/
static Bool inOpaqueContainer(int originalInstance) {
    int instance = admin[originalInstance].location;
	
    while (isContainer(instance)) {
        if (getInstanceAttribute(instance, OPAQUEATTRIBUTE))
            return TRUE;
        instance = admin[instance].location;
    }
    return FALSE;
}

/*----------------------------------------------------------------------*/
static Bool reachable(int instance) {
    if (isA(instance, THING) || isA(instance, LOCATION))
        return isHere(instance, FALSE) && !inOpaqueContainer(instance);
    else
        return TRUE;
}

/*----------------------------------------------------------------------*/
static void enforcePresence(Parameter parameters[]) {
    // TODO Similar to disambiguate*() but this does error()
    int i;
	
    for (i = 0; !isEndOfList(&parameters[i]); i++) {
        if (!reachable(parameters[i].instance)) {
            errorNoSuch(parameters[i]);
        }
    }
}


/*----------------------------------------------------------------------*/
static Aint *nounReferencesForWord(int wordIndex) {
    return (Aint *) pointerTo(dictionary[playerWords[wordIndex].code].nounRefs);
}


/*----------------------------------------------------------------------*/
static Aint *adjectiveReferencesForWord(int wordIndex) {
    return (Aint *) pointerTo(dictionary[playerWords[wordIndex].code].adjectiveRefs);
}


/*----------------------------------------------------------------------*/
static void transformLiteralIntoSingleParameter(Parameter parameters[]) {
    parameters[0].instance = instanceFromLiteral(playerWords[currentWordIndex].code - dictionarySize);
    parameters[0].firstWord = EOF; /* No words used! */
    setEndOfList(&parameters[1]);
}


/*----------------------------------------------------------------------*/
static void transformPronounIntoSingleParameter(Parameter parsedParameters[]) {
    static Parameter *pronounInstances = NULL;
    pronounInstances = ensureParameterArrayAllocated(pronounInstances, MAXPARAMS);
	
    int pronounCandidateCount = getPronounInstances(playerWords[currentWordIndex].code, pronounInstances);
    if (pronounCandidateCount == 0)
        errorWhat(currentWordIndex);
    else if (pronounCandidateCount > 1)
        errorWhichPronoun(currentWordIndex, pronounInstances);
    else {
        parsedParameters[0].instance = pronounInstances[0].instance;
        parsedParameters[0].firstWord = EOF; /* No words used! */
        setEndOfList(&parsedParameters[1]);
    }
}


/*----------------------------------------------------------------------*/
static Bool anotherAdjective(int wordIndex) {
    return !endOfWords(wordIndex) && isAdjectiveWord(wordIndex);
}


/*----------------------------------------------------------------------*/
static Bool lastPossibleNoun(int wordIndex) {
    return isNounWord(wordIndex) && (endOfWords(wordIndex+1) || !isNounWord(wordIndex+1));
}


/*----------------------------------------------------------------------*/
static void updateWithReferences(Parameter result[], int wordIndex, Aint *(*referenceFinder)(int wordIndex)) {
    static Parameter *references = NULL; /* Instances referenced by a word */
    references = ensureParameterArrayAllocated(references, MAXENTITY);

    copyReferencesToParameterList(referenceFinder(wordIndex), references);
    if (lengthOfParameterList(result) == 0)
        copyParameterList(result, references);
    else
        intersectParameterLists(result, references);
}


/*----------------------------------------------------------------------*/
static void filterOutNonReachable(Parameter filteredCandidates[]) {
    int i;
    for (i = 0; !isEndOfList(&filteredCandidates[i]); i++)
        if (!reachable(filteredCandidates[i].instance))
            filteredCandidates[i].instance = 0;
    compressParameterList(filteredCandidates);
}



/*
 * Disambiguation is hard: there are a couple of different cases that
 * we want to handle: Omnipotent parameter position, multiple present
 * and non-present objects etc. The following table will show which
 * message we would like to give in the various situations.
 *
 * p = present, n = non-present, 1 = single, m = multiple
 * (1p1n = single present, single non-present)
 *
 * p, n, omni,	result,			why?
 * -----------------------------------------------------------------
 * 0, 0, no,	errorNoSuch(w)
 * 0, 1, no,	errorNoSuch(w)
 * 0, m, no,	errorNoSuch(w)
 * 1, 0, no,	ok(p)
 * 1, 1, no,	ok(p)
 * 1, m, no,	ok(p)
 * m, 0, no,	errorWhichOne(p)
 * m, 1, no,	errorWhichOne(p)	only present objects should be revealed
 * m, m, no,	errorWhichOne(p)	d:o

 * 0, 0, yes,	errorNoSuch(w)
 * 0, 1, yes,	ok(n)
 * 0, m, yes,	errorWhichOne(n)	already looking "beyond" presence, might reveal undiscovered distant objects
 * 1, 0, yes,	ok(p)
 * 1, 1, yes,	ok(p)			present objects have priority
 * 1, m, yes,	ok(p)			present objects have priority
 * m, 0, yes,	errorWhichOne(p)
 * m, 1, yes,	errorWhichOne(p)	present objects have priority
 * m, m, yes,	errorWhichOne(p)	present objects have priority
 */


/*----------------------------------------------------------------------*/
static void disambiguateParameters(Parameter candidates[]) {
    /* There are more than one candidate, let's see if we can figure out a single one */
    /* If there is one present, let's go for that */

    static Parameter *filteredCandidates = NULL;
    filteredCandidates = ensureParameterArrayAllocated(filteredCandidates, MAXPARAMS);

    copyParameterList(filteredCandidates, candidates);

    filterOutNonReachable(filteredCandidates);

    if (lengthOfParameterList(filteredCandidates) == 1)
        // Found a single one so we use that
        copyParameterList(candidates, filteredCandidates);

    // We could experiment with:
    // disambiguateUsingChecks(candidates, parameterPosition);
    // But, we don't have the parameterPosition here. Maybe we can do this later?
}


/*
 * There are various ways the player can refer to things, some are
 * explicit, in which case they should be kept in the input. If he said
 * 'them', 'all' or some such the list is inferred so we must filter
 * it w.r.t what it can mean. A special case is when he said 'the ball'
 * and there is only one ball here, but multiple in the game.  We need
 * to be able to distinguish between these cases!!!  'them' is
 * explicit, 'all' is inferred, exceptions can never be inferred,
 * maybe 'all' is the only inferred?
 */


/*----------------------------------------------------------------------*/
static void disambiguateCandidatesForPosition(ParameterPosition parameterPositions[], int position, Parameter candidates[]) {
    int i;
    Parameter *parameters = allocateParameterArray(MAXPARAMS);

    convertPositionsToParameters(parameterPositions, parameters);
    for (i = 0; !isEndOfList(&candidates[i]); i++) {
        if (candidates[i].instance != 0) { /* Already empty? */
            parameters[position] = candidates[i];
	    // TODO This is not filled in or used anywhere yet
	    parameterPositions[position].instance = candidates[i].instance;
            // DISAMBIGUATION!!
            if (!reachable(candidates[i].instance) || !possible(current.verb, parameters, parameterPositions))
                candidates[i].instance = 0; /* Then remove this candidate from list */
        }
    }
    parameters[position].instance = 0; /* Restore multiple marker */

    compressParameterList(candidates);
    free(parameters);
}


/*----------------------------------------------------------------------*/
static void transformAdjectivesAndNounToSingleParameter(Parameter parameters[], Parameter undisambiguatedParameters[]) {
    // REFACTOR: for the purpose of refactoring the undisambiguatedParameters[] are used to return the parameters without disambiguating them first
    Parameter savedParameters[MAXPARAMS+1]; /* Saved list for backup at EOF */

    int firstWord, lastWord;
    Bool adjectiveOrNounFound = FALSE;

    static Parameter *references = NULL; /* Instances referenced by a word */
    references = allocateParameterArray(MAXPARAMS);

    firstWord = currentWordIndex;
    while (anotherAdjective(currentWordIndex)) {
        if (lastPossibleNoun(currentWordIndex))
            break;
        copyParameterList(savedParameters, parameters); /* To save it for backtracking */
        updateWithReferences(parameters, currentWordIndex, adjectiveReferencesForWord);
        adjectiveOrNounFound = TRUE;
        currentWordIndex++;
    }

    if (!endOfWords(currentWordIndex)) {
        if (isNounWord(currentWordIndex)) {
            updateWithReferences(parameters, currentWordIndex, nounReferencesForWord);
            adjectiveOrNounFound = TRUE;
            currentWordIndex++;
        } else
            error(M_NOUN);
    } else if (adjectiveOrNounFound) {
        /* Perhaps the last word was also a noun? */
        if (isNounWord(currentWordIndex - 1)) {
            // TODO When does this get executed? Maybe if conjunctions can be nouns? Or nouns be adjectives?
            printf("DEBUG:When does this get executed?");
            copyParameterList(parameters, savedParameters); /* Restore to before last adjective */
            copyReferencesToParameterList(nounReferencesForWord(currentWordIndex-1), references);
            if (isEndOfList(&parameters[0]))
                copyParameterList(parameters, references);
            else
                intersectParameterLists(parameters, references);
        } else
            error(M_NOUN);
    }
    lastWord = currentWordIndex - 1;

    parameters[0].firstWord = firstWord;
    parameters[0].lastWord = lastWord;

    copyParameterList(undisambiguatedParameters, parameters);
    // DISAMBIGUATION!!!
    if (lengthOfParameterList(parameters) > 1)
        disambiguateParameters(parameters);
    if (lengthOfParameterList(parameters) > 1)
        errorWhichOne(parameters);
    else if (adjectiveOrNounFound && lengthOfParameterList(parameters) == 0) {
        Parameter parameter;
        parameter.instance = 0; /* Just make it anything != EOF */
        parameter.firstWord = firstWord;
        parameter.lastWord = lastWord;
        errorNoSuch(parameter);
    }
}


/*----------------------------------------------------------------------*/
static void parseReferences(Parameter parameters[], Parameter undisambiguatedParameters[]) {
    // REFACTOR: for the purpose of refactoring the undisambiguatedParameters[] are used to return the parameters without disambiguating them first
    clearParameterList(parameters);

    if (isLiteralWord(currentWordIndex)) {
        transformLiteralIntoSingleParameter(parameters);
        currentWordIndex++;
    } else if (isPronounWord(currentWordIndex)) {
        transformPronounIntoSingleParameter(parameters);
        currentWordIndex++;
    } else {
        transformAdjectivesAndNounToSingleParameter(parameters, undisambiguatedParameters);
    }
}


/*----------------------------------------------------------------------*/
static void getPreviousMultipleParameters(Parameter parameters[]) {
    int i;
    for (i = 0; !isEndOfList(&previousMultipleParameters[i]); i++)
        if (!reachable(previousMultipleParameters[i].instance))
            previousMultipleParameters[i].instance = 0;
    compressParameterList(previousMultipleParameters);
    copyParameterList(parameters, previousMultipleParameters);
}


/*----------------------------------------------------------------------*/
static void handleReferenceToPreviousMultipleParameters(Parameter parameters[]) {
    getPreviousMultipleParameters(parameters);
    if (lengthOfParameterList(parameters) == 0)
	errorWhat(currentWordIndex);
    parameters[0].firstWord = currentWordIndex;
    parameters[0].lastWord = currentWordIndex;
    currentWordIndex++;
}



/*
 * A "simple" parameter is in one of three forms:
 *
 * 1) adjectives and nouns referencing a single instance (might
 * actually match multiple instances in the game...)
 *
 * 2) multiple of 1) separated by conjunctions ("a and b and c")
 *
 * 3) a pronoun referencing a single instance

 * We also need to handle "them" here since it is also a common can be
 * the pronoun for some instances, e.g. scissor, trouser, money,
 * glasses, but it can also refer to the set of instances from the
 * previous command. If it is a pronoun but there are also multiple
 * parameter from the previous command, we give precedence to the
 * multiple previous.
 */

/*----------------------------------------------------------------------*/
static void simple(Parameter parameters[]) {
    static Parameter *tlst = NULL;
    tlst = ensureParameterArrayAllocated(tlst, MAXENTITY);

    /* This will loop until all references are collected (typically "a and b and c") */
    for (;;) {
	// TODO Maybe this should go in the complex()?
	if (isThemWord(currentWordIndex) && (!isPronounWord(currentWordIndex) ||
					     (isPronounWord(currentWordIndex) && lengthOfParameterList(previousMultipleParameters) > 0))) {
	    handleReferenceToPreviousMultipleParameters(parameters);
        } else {
            parseReferences(parameters, parameters);
            if (lengthOfParameterList(parameters) == 0) { /* Failed! */
                // TODO this gets executed in case of "take all except", any other cases?
                // printf("DEBUG: parseForCandidates() returned 0 candidates to simple()\n");
                return;
            }
        }

        mergeParameterLists(tlst, parameters);

        if (!endOfWords(currentWordIndex)
            && (isConjunctionWord(currentWordIndex) && (isAdjectiveWord(currentWordIndex+1)
                                                 || isNounWord(currentWordIndex+1)))) {
            /* Since this is a conjunction and the next seems to be another instance reference,
               let's continue with that by eating the conjunction */
            currentWordIndex++;
        } else {
            copyParameterList(parameters, tlst);
            return;
        }
    }
}


/*----------------------------------------------------------------------*/
static void parseExceptions(ParameterPosition *parameterPosition, ParameterParser simpleParameterParser) {
    int exceptWordIndex = currentWordIndex;
    currentWordIndex++;
    parameterPosition->exceptions = ensureParameterArrayAllocated(parameterPosition->exceptions, MAXPARAMS);    
    simpleParameterParser(parameterPosition->exceptions);
    if (lengthOfParameterList(parameterPosition->exceptions) == 0)
	errorAfterExcept(exceptWordIndex);
    subtractListFromParameterList(parameterPosition->parameters, parameterPosition->exceptions);
    if (lengthOfParameterList(parameterPosition->parameters) == 0)
	error(M_NOT_MUCH);
}


/*
 * Complex instance references are of the form:
 *
 * 1) all
 *
 * 2) all except
 *
 * 3) a simple (see above) instance reference(s)
 */

/*----------------------------------------------------------------------*/
static void complexParameterParserDelegate(ParameterPosition *parameterPosition, ParameterParser simpleParameterParser, void (*allBuilder)(Parameter allCandidates[])) {
    parameterPosition->parameters = ensureParameterArrayAllocated(parameterPosition->parameters, MAXPARAMS);    

    parameterPosition->all = FALSE;
    parameterPosition->explicitMultiple = FALSE;

    if (isAllWord(currentWordIndex)) {
        parameterPosition->all = TRUE;
        parameterPosition->explicitMultiple = TRUE;
        allBuilder(parameterPosition->parameters); /* Build list of all possible objects */
        currentWordIndex++;
        if (!endOfWords(currentWordIndex) && isExceptWord(currentWordIndex))
	    parseExceptions(parameterPosition, simpleParameterParser);
    } else {
        simpleParameterParser(parameterPosition->parameters);
        if (lengthOfParameterList(parameterPosition->parameters) > 1)
            parameterPosition->explicitMultiple = TRUE;
    }

}

/*----------------------------------------------------------------------*/
static void complexReferencesParser(ParameterPosition *parameterPosition) {
    complexParameterParserDelegate(parameterPosition, simple, buildAllHere);
}


/*----------------------------------------------------------------------*/
static Bool restrictionCheck(RestrictionEntry *restriction, int instance) {
    if (restriction->class == RESTRICTIONCLASS_CONTAINER)
        return instances[instance].container != 0;
    else
        return isA(instance, restriction->class);
}


/*----------------------------------------------------------------------*/
static void runRestriction(RestrictionEntry *restriction, Parameter parameters[]) {
    if (sectionTraceOption)
        printf("\n<SYNTAX parameter #%ld Is Not of class %ld:>\n",
               restriction->parameterNumber, restriction->class);
    if (restriction->stms) {
        setParameters(parameters);
        interpret(restriction->stms);
    } else
        error(M_CANT0);
}


/*----------------------------------------------------------------------*/
static int remapParameterOrder(int syntaxNumber, ParameterPosition parameterPositions[]) {
    /* Find the syntax map, use the verb code from it and remap the parameters */
    ParameterMapEntry *parameterMapTable;
    Aword *parameterMap;
    Aint parameterNumber;
    ParameterPosition savedParameterPositions[MAXPARAMS+1];

    for (parameterMapTable = pointerTo(header->parameterMapAddress); !isEndOfList(parameterMapTable); parameterMapTable++)
        if (parameterMapTable->syntaxNumber == syntaxNumber)
            break;
    if (isEndOfList(parameterMapTable))
        syserr("Could not find syntax in mapping table.");

    parameterMap = pointerTo(parameterMapTable->parameterMapping);

    copyParameterPositions(parameterPositions, savedParameterPositions);

    for (parameterNumber = 1; !savedParameterPositions[parameterNumber-1].endOfList; parameterNumber++) {
        parameterPositions[parameterNumber-1] = savedParameterPositions[parameterMap[parameterNumber-1]-1];
    }
	
    return parameterMapTable->verbCode;
}


/*----------------------------------------------------------------------*/
static Bool hasBit(Aword flags, Aint bit) {
    return (flags & bit) != 0;
}

/*----------------------------------------------------------------------*/
static Bool multipleAllowed(Aword flags) {
    return hasBit(flags, MULTIPLEBIT);
}



/*
 * TODO There are a number of ways that the number of parameters might
 * be more than one:
 *
 * 1) Player used ALL and it matched more than one
 *
 * 2) Player explicitly refered to multiple objects
 *
 * 3) Player did a single (or multiple) reference that was ambiguous
 * in which case we need to disambiguate it (them). If we want to do
 * this after complete parsing we must be able to see the possible
 * parameters for each of these references, e.g.:
 *
 * > take the vase and the book
 *
 * In this case it is a single parameterPosition, but multiple
 * explicit references and each of those might match multiple
 * instances in the game.
 */

/*----------------------------------------------------------------------*/
static void parseParameterPosition(ParameterPosition *parameterPosition, Aword flags, void (*potentiallyComplexReferencesParser)(ParameterPosition *parameterPosition)) {
    parameterPosition->parameters = ensureParameterArrayAllocated(parameterPosition->parameters, MAXENTITY);
    
    potentiallyComplexReferencesParser(parameterPosition);
    if (lengthOfParameterList(parameterPosition->parameters) == 0) /* No object!? */
        error(M_WHAT);
    
    /* TODO This should not be done here, it should be posponed to the disambiguation phase */
    if (!hasBit(flags, OMNIBIT))
        /* If its not an omnipotent parameter, resolve by presence */
        if (!parameterPosition->explicitMultiple) /* if so, complex() has already done this */
            enforcePresence(parameterPosition->parameters);

    if (parameterPosition->explicitMultiple && !multipleAllowed(flags))
	error(M_MULTIPLE);
}

/*----------------------------------------------------------------------*/
static ElementEntry *elementForParameter(ElementEntry *elms) {
    /* Require a parameter if elms->code == 0! */
    while (!isEndOfList(elms) && elms->code != 0)
        elms++;
    if (isEndOfList(elms))
        return NULL;
    return elms;
}

/*----------------------------------------------------------------------*/
static ElementEntry *elementForEndOfSyntax(ElementEntry *elms) {
    while (!isEndOfList(elms) && elms->code != EOS)
        elms++;
    if (isEndOfList(elms)) /* No match for EOS! */
        return NULL;
    return elms;
}

/*----------------------------------------------------------------------*/
static ElementEntry *elementForWord(ElementEntry *elms, Aint wordCode) {
    while (!isEndOfList(elms) && elms->code != wordCode)
        elms++;
    if (isEndOfList(elms))
        return NULL;
    return elms;
}


/*----------------------------------------------------------------------*/
static Bool isInstanceReferenceWord(int wordIndex) {
    return isNounWord(wordIndex) || isAdjectiveWord(wordIndex) || isAllWord(wordIndex)
	|| isLiteralWord(wordIndex) || isItWord(wordIndex) || isThemWord(wordIndex) || isPronounWord(wordIndex);
}


/*----------------------------------------------------------------------*/
static Bool endOfPlayerCommand(int wordIndex) {
    return endOfWords(wordIndex) || isConjunctionWord(wordIndex);
}


/*----------------------------------------------------------------------*/
static ElementEntry *parseInputAccordingToElementTree(ElementEntry *startingElement, ParameterPosition parameterPositions[]) {
    ElementEntry *currentElement = startingElement;
    ElementEntry *nextElement = startingElement;

    int parameterCount = 0;
    while (nextElement != NULL) {
        parameterPositions[parameterCount].endOfList = TRUE;

        /* Traverse the possible branches of currentElement to find a match, let the actual input control what we look for */

        if (endOfPlayerCommand(currentWordIndex)) {
            // TODO If a conjunction word is also some other type of word, like noun? What happens?
            return elementForEndOfSyntax(currentElement);
        }

        /* Or an instance reference ? */
        if (isInstanceReferenceWord(currentWordIndex)) {
            /* If so, save word info for this parameterPosition */
            nextElement = elementForParameter(currentElement);
            if (nextElement != NULL) {
                parseParameterPosition(&parameterPositions[parameterCount], nextElement->flags, complexReferencesParser);
                currentElement = (ElementEntry *) pointerTo(nextElement->next);
                parameterPositions[parameterCount].endOfList = FALSE;
                parameterCount++;
                continue;
            }
        }

        /* Or maybe preposition? */
        if (isPrepositionWord(currentWordIndex)) {
            /* A preposition? Or rather, an intermediate word? */
            nextElement = elementForWord(currentElement, dictionary[playerWords[currentWordIndex].code].code);
            if (nextElement != NULL) {
                currentWordIndex++; /* Word matched, go to next */
                currentElement = (ElementEntry *) pointerTo(nextElement->next);
                continue;
            }
        }

        /* Anything else is an error, but we'll handle 'but' specially here */
        if (isExceptWord(currentWordIndex))
            errorButAfterAll(currentWordIndex);
		
        /* If we get here we couldn't match anything... */
        nextElement = NULL;
    }
    return NULL;
}

/*----------------------------------------------------------------------*/
static SyntaxEntry *findSyntaxTreeForVerb(int verbCode) {
    SyntaxEntry *stx;
    for (stx = stxs; !isEndOfList(stx); stx++)
        if (stx->code == verbCode)
            return stx;
    /* No matching syntax */
    error(M_WHAT);
    return NULL;
}


/*----------------------------------------------------------------------*/
static Bool anyExplicitMultiple(ParameterPosition parameterPositions[]) {
    int i;

    for (i = 0; !parameterPositions[i].endOfList; i++)
        if (parameterPositions[i].explicitMultiple)
            return TRUE;
    return FALSE;
}


/*----------------------------------------------------------------------*/
static Bool anyAll(ParameterPosition parameterPositions[]) {
    int i;

    for (i = 0; !parameterPositions[i].endOfList; i++)
        if (parameterPositions[i].all)
            return TRUE;
    return FALSE;
}


/*----------------------------------------------------------------------*/
static void checkRestrictedParameters(ParameterPosition parameterPositions[], ElementEntry elms[]) {
    RestrictionEntry *restriction;
    Parameter localParameters[MAXPARAMS+1];
    int i;
    
    for (i=0; !parameterPositions[i].endOfList; i++)
        localParameters[i] = parameterPositions[i].parameters[0];

    for (restriction = (RestrictionEntry *) pointerTo(elms->next); !isEndOfList(restriction); restriction++) {
        ParameterPosition *parameterPosition = &parameterPositions[restriction->parameterNumber-1];
        if (parameterPosition->explicitMultiple) {
            /* This was a multiple parameter position, so check all multipleCandidates */
            int i;
            for (i = 0; !isEndOfList(&parameterPosition->parameters[i]); i++) {
                localParameters[restriction->parameterNumber-1] = parameterPosition->parameters[i];
                if (!restrictionCheck(restriction, parameterPosition->parameters[i].instance)) {
                    /* Multiple could be both an explicit list of instance references and an expansion of ALL */
                    if (!parameterPosition->all) {
                        char marker[80];
                        /* It wasn't ALL, we need to say something about it, so
                         * prepare a printout with $1/2/3
                         */
                        sprintf(marker, "($%ld)", restriction->parameterNumber);
                        setParameters(localParameters);
                        output(marker);
                        runRestriction(restriction, localParameters);
                        para();
                    }
                    parameterPosition->parameters[i].instance = 0; /* In any case remove it from the list */
                }
            }
        } else {
            if (!restrictionCheck(restriction, parameterPosition->parameters[0].instance)) {
                runRestriction(restriction, localParameters);
                abortPlayerCommand();
            }
        }
        parameterPositions[restriction->parameterNumber - 1].checked = TRUE;
    }
}


/*----------------------------------------------------------------------*/
static void checkNonRestrictedParameters(ParameterPosition parameterPositions[]) {
    int parameterIndex;
    for (parameterIndex = 0; !parameterPositions[parameterIndex].endOfList; parameterIndex++)
        if (!parameterPositions[parameterIndex].checked) {
            if (parameterPositions[parameterIndex].explicitMultiple) {
                /* This was a multiple parameter position, check all multiple candidates and remove failing */
                int i;
                for (i = 0; !isEndOfList(&parameterPositions[parameterIndex].parameters[i]); i++)
                    if (parameterPositions[parameterIndex].parameters[i].instance != 0) /* Skip any empty slots */
                        if (!isObject(parameterPositions[parameterIndex].parameters[i].instance))
                            parameterPositions[parameterIndex].parameters[i].instance = 0;
            } else if (!isObject(parameterPositions[parameterIndex].parameters[0].instance))
                error(M_CANT0);
        }
}


/*----------------------------------------------------------------------*/
static void restrictParametersAccordingToSyntax(ParameterPosition parameterPositions[], ElementEntry *elms) {
    uncheckAllParameterPositions(parameterPositions);
    checkRestrictedParameters(parameterPositions, elms);
    checkNonRestrictedParameters(parameterPositions);
}


/*----------------------------------------------------------------------*/
static void matchNounPhrase(Parameter parameter, ReferencesFinder adjectiveReferencesFinder, ReferencesFinder nounReferencesFinder) {
    int i;
    
    for (i = parameter.firstWord; i < parameter.lastWord; i++)
        updateWithReferences(parameter.candidates, i, adjectiveReferencesFinder);
    updateWithReferences(parameter.candidates, parameter.lastWord, nounReferencesFinder);
}


/*----------------------------------------------------------------------*/
static void instanceMatcher(Parameter parameter) {
    if (parameter.firstWord != EOF && parameter.firstWord != 0) {
        // TODO: Old way does not preserve word indications in these circumstances
        // so we need to have the legacy code mark these in some other fashion as
        // to preserve the word indexes in every case
        if (isThemWord(parameter.firstWord) || isPronounWord(parameter.firstWord))
            ; // TBD
        else if (isAllWord(parameter.firstWord))
            ; // TBD
        else
            matchNounPhrase(parameter, adjectiveReferencesForWord, nounReferencesForWord);
    }
}


/*----------------------------------------------------------------------*/
static void matchParameters(Parameter parameters[], void (*instanceMatcher)(Parameter parameter)) 
{
    int i;
     
    for (i = 0; i < lengthOfParameterList(parameters); i++) {
	if (parameters[i].candidates == NULL)
	    parameters[i].candidates = allocateParameterArray(MAXENTITY);
	instanceMatcher(parameters[i]);
    }
}


/*----------------------------------------------------------------------*/
static void handleFailedParse(ElementEntry *elms) {
    if (elms == NULL)
        error(M_WHAT);
    if (elms->next == 0) { /* No verb code, verb not declared! */
        /* TODO Does this ever happen? */
        error(M_CANT0);
    }
}

static void convertMultipleCandidatesToMultipleParameters(Parameter multipleParameters[]) {
    int parameterCount;
    for (parameterCount=0; !parameterPositions[parameterCount].endOfList; parameterCount++)
	if (parameterPositions[parameterCount].explicitMultiple) {
            compressParameterList(parameterPositions[parameterCount].parameters);
            copyParameterList(multipleParameters, parameterPositions[parameterCount].parameters);
        }
}



/*----------------------------------------------------------------------*/
static void try(Parameter parameters[], Parameter multipleParameters[]) {
    // TODO This is much too long, try to refactor out some functions
    ElementEntry *element;         /* Pointer to element list */
    SyntaxEntry *stx;           /* Pointer to syntax parse list */
    int position;

    /*
     * TODO This code, and much of the above, should be refactored to
     * not do both parsing and parameter matching at the same time. It
     * should first parse and add to the parameterPosition array where
     * each entry indicates which words in the command line was
     * "eaten" by this parameter. Then each parameter position can be
     * resolved using those words.
     */

    stx = findSyntaxTreeForVerb(verbWordCode);

    /*
     * Then match the player input words, instance references and
     * other words, following the syntax elements in the parse
     * tree.
     */
    element = parseInputAccordingToElementTree(elementTreeOf(stx), parameterPositions);
    handleFailedParse(element);
	
    /*
     * Then, in case it was a syntax synonym, we can map the
     * parameters to their canonical order.  The flags field of EOS
     * element is actually the syntax number!
     */
    current.verb = remapParameterOrder(element->flags, parameterPositions);

    /* TODO Work In Progress! Match parameters to instances ... */
    for (position = 0; !parameterPositions[position].endOfList; position++)
	matchParameters(parameterPositions[position].parameters, instanceMatcher);

    /* Now perform restriction checks */
    restrictParametersAccordingToSyntax(parameterPositions, element);

    // TODO Here we should ensure that parameterPositions[].instance is set to candidates[0] if there was only a single one left

    /* Finally, if the player used ALL, try to find out what was applicable */
    int multiplePosition = findMultipleParameterPosition(parameterPositions);
    if (anyAll(parameterPositions)) {
        // DISAMBIGUATION!!!
        disambiguateCandidatesForPosition(parameterPositions, multiplePosition, parameterPositions[multiplePosition].parameters);
        if (lengthOfParameterList(parameterPositions[multiplePosition].parameters) == 0)
            errorWhat(allWordIndex);

    } else if (anyExplicitMultiple(parameterPositions)) {
        compressParameterList(parameterPositions[multiplePosition].parameters);
        if (lengthOfParameterList(parameterPositions[multiplePosition].parameters) == 0) {
            /* If there where multiple parameters but non left, exit without a */
            /* word, assuming we have already said enough */
            abortPlayerCommand();
        }
    }

    // TODO: Now we need to convert back to legacy parameter and multipleParameter format
    convertPositionsToParameters(parameterPositions, parameters);
    markExplicitMultiple(parameterPositions, parameters);
    convertMultipleCandidatesToMultipleParameters(multipleParameters);

}


/*----------------------------------------------------------------------*/
static void parseOneCommand(Parameter parameters[], Parameter multipleParameters[])
{
    try(parameters, multipleParameters); /* ... to understand what he said */
	
    /* More on this line? */
    if (!endOfWords(currentWordIndex)) {
        if (isConjunctionWord(currentWordIndex))
            currentWordIndex++; /* If so skip the conjunction */
        else
            error(M_WHAT);
    }
}

/*======================================================================*/
void initParsing(void) {
    int dictionaryIndex;
    int pronounIndex = 0;
	
    currentWordIndex = 0;
    continued = FALSE;
    ensureSpaceForPlayerWords(0);
    clearWordList(playerWords);
	
    pronouns = allocatePronounArray(pronouns);
    globalParameters = ensureParameterArrayAllocated(globalParameters, MAXPARAMS);
    previousMultipleParameters = ensureParameterArrayAllocated(previousMultipleParameters, MAXPARAMS);
	
    if (parameterPositions == NULL)
        parameterPositions = allocate(sizeof(ParameterPosition)*MAXPARAMS);
    
    // TODO Refactor out the pronoun handling, e.g registerPronoun()
    for (dictionaryIndex = 0; dictionaryIndex < dictionarySize; dictionaryIndex++)
        if (isPronoun(dictionaryIndex)) {
            pronouns[pronounIndex].pronoun = dictionary[dictionaryIndex].code;
            pronouns[pronounIndex].instance = 0;
            pronounIndex++;
        }
}

/*----------------------------------------------------------------------*/
static int pronounWordForInstance(int instance) {
    /* Scan through the dictionary to find any pronouns that can be used
       for this instance */
    int w;
	
    for (w = 0; w < dictionarySize; w++)
        if (isPronoun(w)) {
            Aword *reference = pointerTo(dictionary[w].pronounRefs);
            while (*reference != EOF) {
                if (*reference == instance)
                    return dictionary[w].code;
                reference++;
            }
        }
    return 0;
}

/*----------------------------------------------------------------------*/
static void addPronounForInstance(int pronoun, int instanceCode) {
    int pronounIndex;
	
    for (pronounIndex = 0; !endOfPronouns(pronounIndex); pronounIndex++)
        ;
    pronouns[pronounIndex].pronoun = pronoun;
    pronouns[pronounIndex].instance = instanceCode;
    setEndOfList(&pronouns[pronounIndex + 1]);
}

/*----------------------------------------------------------------------*/
static void notePronounsForParameters(Parameter parameters[]) {
    /* For all parameters note which ones can be referred to by a pronoun */
    Parameter *p;
	
    clearPronounList(pronouns);
    for (p = parameters; !isEndOfList(p); p++) {
        int pronoun = pronounWordForInstance(p->instance);
        if (pronoun > 0)
            addPronounForInstance(pronoun, p->instance);
    }
}


/*======================================================================*/
void parse(Parameter parameters[]) {
    static Parameter *multipleParameters = NULL;
    multipleParameters = ensureParameterArrayAllocated(multipleParameters, MAXPARAMS);

    if (endOfWords(currentWordIndex)) {
        currentWordIndex = 0;
        scan();
    } else if (anyOutput)
        para();
	
    capitalize = TRUE;
    clearParameterList(parameters);
	
    firstWord = currentWordIndex;
    if (isVerbWord(currentWordIndex)) {
        verbWord = playerWords[currentWordIndex].code;
        verbWordCode = dictionary[verbWord].code;
        currentWordIndex++;
        parseOneCommand(parameters, multipleParameters);
        notePronounsForParameters(parameters);
        fail = FALSE;
        action(current.verb, parameters, multipleParameters);
    } else {
        clearParameterList(previousMultipleParameters);
        clearPronounList(pronouns);
        nonverb();
    }
    lastWord = currentWordIndex - 1;
    if (isConjunctionWord(lastWord))
        lastWord--;
	
    if (anyExplicitMultiple(parameterPositions))
        copyParameterList(previousMultipleParameters, multipleParameters);
    else
        clearParameterList(previousMultipleParameters);
}

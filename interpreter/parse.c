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

#include "scan.h"


#ifdef HAVE_GLK
#include "glkio.h"
#endif


/* PRIVATE CONSTANTS */


/* PUBLIC DATA */


/* PRIVATE DATA */
static Bool plural = FALSE;

/* Syntax Parameters */
static int paramidx;
static Parameter *previousMultipleParameters; /* Previous multiple list */


/*======================================================================*/
void forceNewPlayerInput() {
    setEndOfList(&playerWords[wordIndex]);
}


/*----------------------------------------------------------------------
  SCAN DATA & PROCEDURES
 
  All procedures for getting a command and turning it into a list of
  dictionary entries are placed here.
 
  ---------------------------------------------------------------------*/

/* PRIVATE DATA */



/*----------------------------------------------------------------------

  Parameter allocation, saving and restoring
 
  ----------------------------------------------------------------------*/

/* For parameters that are literals we need to trick message handling to
 * output the word and create a string literal instance if anyone wants to
 * refer to an attribute of it (literals inherit from entity so application
 * can have added an attribute) */

/*----------------------------------------------------------------------*/
static void addParameterForWord(Parameter *parameters, int wordIndex) {
    Parameter *parameter = findEndOfList(parameters);
	
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
	
    // TODO Need to concat the words into a string literal, instead of just using the last word
    createStringLiteral(pointerTo(dictionary[playerWords[lastWordIndex].code].string));
    parameter->instance = instanceFromLiteral(litCount);
    parameter->useWords = TRUE;
    parameter->firstWord = firstWordIndex;
    parameter->lastWord = lastWordIndex;
    setEndOfList(parameter+1);
}
#endif


/*----------------------------------------------------------------------*
  PARSE DATA & PROCEDURES
 
  All procedures and data for getting a command and parsing it
 
  \*---------------------------------------------------------------------- */

/* Private Types */
static int allWordIndex; /* Word index of the ALL_WORD found */
static int multipleLength; /* No. of objects matching 'all' */

typedef struct PronounEntry { /* To remember parameter/pronoun relations */
    int pronoun;
    int instance;
} Pronoun;

static Pronoun *pronouns = NULL;


/*----------------------------------------------------------------------*/
static Pronoun *allocatePronounArray(Pronoun *pronouns) {
    if (pronouns != NULL)
        return pronouns;
    else
        return allocate((MAXPARAMS+1) * sizeof(Pronoun));
}


/*----------------------------------------------------------------------*/
static Bool endOfWords(int wordIndex) {
    return isEndOfList(&playerWords[wordIndex]);
}


/*----------------------------------------------------------------------*/
static void nonverb(void) {
    if (isDirectionWord(wordIndex)) {
        wordIndex++;
        if (!endOfWords(wordIndex) && !isConjunctionWord(wordIndex))
            error(M_WHAT);
        else
            go(current.location, dictionary[playerWords[wordIndex-1].code].code);
        if (!endOfWords(wordIndex))
            wordIndex++;
    } else
        error(M_WHAT);
}

/*----------------------------------------------------------------------*/
static void errorWhichOne(Parameter alternative[]) {
    int p; /* Index into the list of alternatives */
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);
	
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
    Parameter *messageParameters = allocateParameterArray(NULL, MAXPARAMS);
	
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
    Parameter *messageParameters = allocateParameterArray(NULL, MAXPARAMS);
    addParameterForWord(messageParameters, playerWordIndex);
    printMessageWithParameters(M_WHAT_WORD, messageParameters);
    free(messageParameters);
    abortPlayerCommand();
}

/*----------------------------------------------------------------------*/
static void errorAfterBut(int butWordIndex) {
    Parameter *messageParameters = allocateParameterArray(NULL, MAXPARAMS);
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
	
    for (p = 0; isEndOfList(&playerWords[p]); p++)
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
    Parameter *messageParameters = allocateParameterArray(NULL, MAXPARAMS);
    addParameterForWord(messageParameters, butWordIndex);
    addParameterForWord(messageParameters, fakePlayerWordForAll());
    printMessageWithParameters(M_BUT_ALL, messageParameters);
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
    parameters[0] = parameter;
	
    /* If there was no instance, assume the last word used is the noun,
     * then find any instance with the noun he used */
    if (parameters[0].instance == 0)
        parameters[0].instance = findInstanceForNoun(playerWords[parameter.lastWord].code);
    parameters[0].useWords = TRUE; /* Indicate to use words and not names */
    setEndOfList(&parameters[1]);
    error(M_NO_SUCH);
}

/*----------------------------------------------------------------------*/
static void buildAll(Parameter list[]) {
    int o, i = 0;
    Bool found = FALSE;
	
    for (o = 1; o <= header->instanceMax; o++)
        if (isHere(o, FALSE)) {
            found = TRUE;
            list[i].instance = o;
            list[i++].firstWord = EOF;
        }
    if (!found)
        errorWhat(wordIndex);
    else
        list[i].instance = EOF;
    allWordIndex = wordIndex;
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
    int i = 0;
	
    clearList(instanceParameters);
    for (p = 0; !endOfPronouns(p); p++)
        if (dictionary[word].code == pronouns[p].pronoun) {
            instanceParameters[i].instance = pronouns[p].instance;
            instanceParameters[i].useWords = FALSE; /* Can't use words since they are gone, pronouns
                                                       refer to parameters in previous command */
            setEndOfList(&instanceParameters[++i]);
        }
    return i;
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
static void resolve(Parameter parameters[]) {
    // TODO Similar to disambiguate*() but this does error()
    /* In case the syntax did not indicate omnipotent powers (allowed
       access to remote object), we need to remove non-present
       parameters */
	
    int i;
	
    if (multipleLength > 0) /* ALL has already done this */
        return; /* TODO: NO IT HASN'T ALWAYS SINCE THIS CAN BE ANOTHER PARAMETER!!! */
	
    /* Resolve ambiguities by presence */
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
static void transformLiteralIntoSingleParameter(Parameter parsedParameters[]) {
    parsedParameters[0].instance = instanceFromLiteral(playerWords[wordIndex].code - dictionarySize);
    parsedParameters[0].firstWord = EOF; /* No words used! */
    setEndOfList(&parsedParameters[1]);
}


/*----------------------------------------------------------------------*/
static void transformPronounIntoSingleParameter(Parameter parsedParameters[]) {
    static Parameter *pronounInstances = NULL;
    pronounInstances = allocateParameterArray(pronounInstances, MAXPARAMS);
	
    int pronounCandidateCount = getPronounInstances(playerWords[wordIndex].code, pronounInstances);
    if (pronounCandidateCount == 0)
        errorWhat(wordIndex);
    else if (pronounCandidateCount > 1)
        errorWhichPronoun(wordIndex, pronounInstances);
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
static void updateWithReferences(int wordIndex, Parameter result[], Aint *(*referenceFinder)(int wordIndex)) {
    static Parameter *references = NULL; /* Instances referenced by a word */
    references = allocateParameterArray(references, MAXPARAMS);

    copyReferences(references, referenceFinder(wordIndex));
    if (listLength(result) == 0)
        copyParameterList(result, references);
    else
        intersect(result, references);
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
 * 0, 0, no,	errorNoSuch()
 * 0, 1, no,	errorNoSuch()
 * 0, m, no,	errorNoSuch()
 * 1, 0, no,	ok(p)
 * 1, 1, no,	ok(p)
 * 1, m, no,	ok(p)
 * m, 0, no,	errorWhichOne(p)
 * m, 1, no,	errorWhichOne(p)	only present objects should be revealed
 * m, m, no,	errorWhichOne(p)	d:o
 * 0, 0, yes,	errorNoSuch()
 * 0, 1, yes,	ok(n)
 * 0, m, yes,	errorWhichOne(n)	already looking "beyond" presence, although
 *					might reveal undiscovered distant objects
 * 1, 0, yes,	ok(p)
 * 1, 1, yes,	ok(p)			present objects have priority
 * 1, m, yes,	ok(p)			present objects have priority
 * m, 0, yes,	errorWhichOne(p)
 * m, 1, yes,	errorWhichOne(p)	present objects have priority
 * m, m, yes,	errorWhichOne(p)	present objects have priority
 */


/*----------------------------------------------------------------------*/
static void disambiguateParameter(Parameter candidates[]) {
    /* There are more than one candidate, let's see if we can figure out a single one */

    /* If there is one present, let's go for that */

    int i;
    static Parameter *filteredCandidates = NULL;
    filteredCandidates = allocateParameterArray(filteredCandidates, MAXPARAMS);

    copyParameterList(filteredCandidates, candidates);

    for (i = 0; !isEndOfList(&filteredCandidates[i]); i++)
        if (!reachable(filteredCandidates[i].instance))
            filteredCandidates[i].instance = 0;
    compress(filteredCandidates);

    if (listLength(filteredCandidates) == 1)
        // Found a single one so we use that
        copyParameterList(candidates, filteredCandidates);

    // We could experiment with:
    // disambiguateUsingChecks(candidates, parameterPosition);
    // But, we don't have the parameterPosition here. Maybe we can do this later?
}

/*----------------------------------------------------------------------*/
static void disambiguateCandidatesForPosition(Parameter parameters[], int position, Parameter candidates[]) {
    int i;
    for (i = 0; !isEndOfList(&candidates[i]); i++) {
        if (candidates[i].instance != 0) { /* Already empty? */
            parameters[position] = candidates[i];
            if (!reachable(candidates[i].instance) || !possible())
                candidates[i].instance = 0; /* Then remove this candidate from list */
        }
    }
    parameters[position].instance = 0; /* Restore multiple marker */
    compress(candidates);
}


/*----------------------------------------------------------------------*/
static void parseUnambiguousNounPhrase(Parameter parameterCandidates[]) {
    // TODO This is really too long, need to break out some parts
    int firstWord, lastWord; /* The words the player used */

    Parameter savedParameters[MAXPARAMS+1]; /* Saved list for backup at EOF */

    clearList(parameterCandidates);

    if (isLiteralWord(wordIndex)) {
        transformLiteralIntoSingleParameter(parameterCandidates);
        wordIndex++;
    } else if (isPronounWord(wordIndex)) {
        transformPronounIntoSingleParameter(parameterCandidates);
        wordIndex++; /* Consume the pronoun */
    } else {
        Bool adjectiveOrNounFound = FALSE;

        static Parameter *references = NULL; /* Instances referenced by a word */
        references = allocateParameterArray(references, MAXPARAMS);

        firstWord = wordIndex;
        while (anotherAdjective(wordIndex)) {
            if (lastPossibleNoun(wordIndex))
                break;
            copyParameterList(savedParameters, parameterCandidates); /* To save it for backtracking */
            updateWithReferences(wordIndex, parameterCandidates, adjectiveReferencesForWord);
            adjectiveOrNounFound = TRUE;
            wordIndex++;
        }

        if (!endOfWords(wordIndex)) {
            if (isNounWord(wordIndex)) {
                updateWithReferences(wordIndex, parameterCandidates, nounReferencesForWord);
                adjectiveOrNounFound = TRUE;
                wordIndex++;
            } else
                error(M_NOUN);
        } else if (adjectiveOrNounFound) {
            /* Perhaps the last word was also a noun? */
            if (isNounWord(wordIndex - 1)) {
                printf("DEBUG:When does this get executed?");
                // TODO When does this get executed?
                // Maybe if conjunctions can be nouns?
                copyParameterList(parameterCandidates, savedParameters); /* Restore to before last adjective */
                copyReferences(references, nounReferencesForWord(wordIndex-1));
                if (isEndOfList(&parameterCandidates[0]))
                    copyParameterList(parameterCandidates, references);
                else
                    intersect(parameterCandidates, references);
            } else
                error(M_NOUN);
        }
        lastWord = wordIndex - 1;

         if (listLength(parameterCandidates) > 1)
            disambiguateParameter(parameterCandidates);

        if (listLength(parameterCandidates) > 1)
            errorWhichOne(parameterCandidates);
        else if (adjectiveOrNounFound && listLength(parameterCandidates) == 0) {
            Parameter parameter;
            parameter.instance = 0; /* Just make it anything != EOF */
            parameter.firstWord = firstWord;
            parameter.lastWord = lastWord;
            errorNoSuch(parameter);
        }

        parameterCandidates[0].firstWord = firstWord;
        parameterCandidates[0].lastWord = lastWord;

    }
}

/*----------------------------------------------------------------------*/
static void simple(Parameter olst[]) {
    static Parameter *tlst = NULL;
    int savidx = wordIndex;
    Bool savplur = FALSE;
    int i;

    tlst = allocateParameterArray(tlst, MAXENTITY);

    for (;;) {
        /* Special handling here since THEM_WORD is a common pronoun, so
           we check if it is also a pronoun, if it is but there is a list of
           multi parameters from the previous command, we assume that is
           what he ment */
        if (isThemWord(wordIndex) && ((isPronounWord(wordIndex)
                                       && listLength(previousMultipleParameters) > 0) || !isPronounWord(wordIndex))) {
            plural = TRUE;
            for (i = 0; !isEndOfList(&previousMultipleParameters[i]); i++)
                if (!reachable(previousMultipleParameters[i].instance))
                    previousMultipleParameters[i].instance = 0;
            compress(previousMultipleParameters);
            if (listLength(previousMultipleParameters) == 0)
                errorWhat(wordIndex);
            copyParameterList(olst, previousMultipleParameters);
            olst[0].firstWord = EOF; /* No words used */
            wordIndex++;
        } else {
            parseUnambiguousNounPhrase(olst); /* Look for unambigous noun phrase */
            if (listLength(olst) == 0) { /* Failed! */
                // TODO this gets executed in case of "take all except", any other cases?
                //printf("DEBUG: parseUnambiguousNounPhrase() failed\n");
                copyParameterList(olst, tlst);
                wordIndex = savidx;
                plural = savplur;
                return;
            }
        }
        mergeLists(tlst, olst);
        if (!endOfWords(wordIndex)
            && (isConjunctionWord(wordIndex) && (isAdjectiveWord(wordIndex+1)
                                                 || isNounWord(wordIndex+1)))) {
            /* More parameters in a conjunction separated list ? */
            savplur = plural;
            savidx = wordIndex;
            wordIndex++;
            plural = TRUE;
        } else {
            copyParameterList(olst, tlst);
            return;
        }
    }
}

/*----------------------------------------------------------------------*/
static void complex(Parameter parsedParameters[]) {
    static Parameter *allList = NULL;

    allList = allocateParameterArray(allList, MAXENTITY);

    if (isAllWord(wordIndex)) {
        plural = TRUE;
        buildAll(allList); /* Build list of all objects */
        wordIndex++;
        if (!endOfWords(wordIndex) && isButWord(wordIndex)) {
            int butWordIndex = wordIndex;
            wordIndex++;
            simple(parsedParameters);
            if (listLength(parsedParameters) == 0)
                errorAfterBut(butWordIndex);
            subtractListFromList(allList, parsedParameters);
            if (listLength(allList) == 0)
                error(M_NOT_MUCH);
        }
        copyParameterList(parsedParameters, allList);
        multipleLength = listLength(parsedParameters);
    } else
        simple(parsedParameters); /* Look for simple noun group */
}


/*----------------------------------------------------------------------*/
static Bool restrictionCheck(RestrictionEntry *restriction, Parameter parameters[]) {
    if (restriction->class == RESTRICTIONCLASS_CONTAINER)
        return instances[parameters[restriction->parameterNumber-1].instance].container != 0;
    else
        return isA(parameters[restriction->parameterNumber-1].instance,
                   restriction->class);
}


/*----------------------------------------------------------------------*/
static void runRestriction(RestrictionEntry *restriction) {
    if (sectionTraceOption)
        printf("\n<SYNTAX parameter #%ld Is Not of class %ld:>\n",
               restriction->parameterNumber, restriction->class);
    if (restriction->stms)
        interpret(restriction->stms);
    else
        error(M_CANT0);
}


/*----------------------------------------------------------------------*/
static int mapSyntax(int syntaxNumber, Parameter parameters[]) {
    /* Find the syntax map, use the verb code from it and remap the parameters */
    ParameterMapEntry *parameterMapTable;
    Aword *parameterMap;
    Aint parameterNumber;
    Parameter originalParameters[MAXPARAMS+1];

    for (parameterMapTable = pointerTo(header->parameterMapAddress); !isEndOfList(parameterMapTable); parameterMapTable++)
        if (parameterMapTable->syntaxNumber == syntaxNumber)
            break;
    if (isEndOfList(parameterMapTable))
        syserr("Could not find syntax in mapping table.");

    parameterMap = pointerTo(parameterMapTable->parameterMapping);
    copyParameterList(originalParameters, parameters);
    for (parameterNumber = 1; !isEndOfList(&originalParameters[parameterNumber-1]); parameterNumber++)
        parameters[parameterNumber-1] = originalParameters[parameterMap[parameterNumber-1]-1];
	
    return parameterMapTable->verbCode;
}


/*----------------------------------------------------------------------*/
static Bool hasBit(Aword flags, Aint bit) {
    return (flags & bit) != 0;
}

/*----------------------------------------------------------------------*/
static void parseParameter(Parameter parameters[], Aword flags, Bool *anyPlural, Parameter multipleList[]) {
    Parameter *candidates = allocateParameterArray(NULL, MAXPARAMS); /* List of parameters parsed, possibly multiple */
	
    plural = FALSE;
    complex(candidates);
    if (listLength(candidates) == 0) /* No object!? */
        error(M_WHAT);
    if (!hasBit(flags, OMNIBIT))
	/* If its not an omnipotent parameter, resolve by presence */
        resolve(candidates);
    if (plural) {
        if (!hasBit(flags, MULTIPLEBIT)) /* Allowed multiple? */
            error(M_MULTIPLE);
        else {
            /* Mark this as the multiple position in which to insert actual
               parameter values later */
            parameters[paramidx++].instance = 0;
            copyParameterList(multipleList, candidates);
            *anyPlural = TRUE;
        }
    } else
        parameters[paramidx++] = candidates[0];
	
    setEndOfList(&parameters[paramidx]);
	
    free(candidates);
}

/*----------------------------------------------------------------------*/
static ElementEntry *requireParameterElement(ElementEntry *elms) {
    /* Require a parameter if elms->code == 0! */
    while (!isEndOfList(elms) && elms->code != 0)
        elms++;
    if (isEndOfList(elms))
        return NULL;
    return elms;
}

/*----------------------------------------------------------------------*/
static ElementEntry *requireEndOfSyntax(ElementEntry *elms) {
    while (!isEndOfList(elms) && elms->code != EOS)
        elms++;
    if (isEndOfList(elms)) /* No match for EOS! */
        return NULL;
    return elms;
}

/*----------------------------------------------------------------------*/
static ElementEntry *requireWordElement(ElementEntry *elms, Aint wordCode) {
    while (!isEndOfList(elms) && elms->code != wordCode)
        elms++;
    if (isEndOfList(elms))
        return NULL;
    return elms;
}

/*----------------------------------------------------------------------*/
static Bool isParameterWord(int wordIndex) {
    return isNounWord(wordIndex) || isAdjectiveWord(wordIndex) || isAllWord(wordIndex)
	|| isLiteralWord(wordIndex) || isItWord(wordIndex) || isThemWord(wordIndex) || isPronounWord(wordIndex);
}

/*----------------------------------------------------------------------*/
static ElementEntry *parseElementTree(Parameter parameters[], ElementEntry *elms, Bool *anyPlural, Parameter multipleList[]) {
    ElementEntry *currentEntry = elms;
	
    while (TRUE) { /* Traverse the possible branches to find a match */
        ElementEntry *elms;
		
        /* End of input? */
        // TODO If conjunction word is also some other type of word, like noun?
        if (endOfWords(wordIndex) || isConjunctionWord(wordIndex)) {
            elms = requireEndOfSyntax(currentEntry);
            if (elms == NULL)
                return NULL;
            else
                return elms;
        }
		
        if (isParameterWord(wordIndex)) {
            elms = requireParameterElement(currentEntry);
            if (elms != NULL) {
                parseParameter(parameters, elms->flags, anyPlural, multipleList);
                currentEntry = (ElementEntry *) pointerTo(elms->next);
                continue;
            } /* didn't allow a parameter here so fall through
                 and try with a preposition... */
        }
		
        if (isPrepositionWord(wordIndex)) {
            /* A preposition? Or rather, an intermediate word? */
            elms = requireWordElement(currentEntry, dictionary[playerWords[wordIndex].code].code);
            if (elms != NULL) {
                wordIndex++; /* Word matched, go to next */
                currentEntry = (ElementEntry *) pointerTo(elms->next);
                continue;
            }
        }
		
        if (isButWord(wordIndex))
            errorButAfterAll(wordIndex);
		
        /* If we get here we couldn't match anything... */
        return NULL;
    }
    /* And this should never happen ... */
    syserr("Fall-through in 'parseElementTree()'");
    return NULL;
}

/*----------------------------------------------------------------------*/
static SyntaxEntry *findSyntax(int verbCode) {
    SyntaxEntry *stx;
    for (stx = stxs; !isEndOfList(stx); stx++)
        if (stx->code == verbCode)
            return stx;
    /* No matching syntax */
    error(M_WHAT);
    return NULL;
}


/*----------------------------------------------------------------------*/
static void checkRestrictedParameters(Parameter parameters[], ElementEntry elms[], Parameter multipleCandidates[], Bool checked[]) {
    RestrictionEntry *restriction;
    for (restriction = (RestrictionEntry *) pointerTo(elms->next); !isEndOfList(restriction); restriction++) {
	if (parameters[restriction->parameterNumber-1].instance == 0) {
	    /* This was a multiple parameter position, so check all multipleCandidates and remove failing */
	    int i;
	    for (i = 0; !isEndOfList(&multipleCandidates[i]); i++) {
		parameters[restriction->parameterNumber-1] = multipleCandidates[i];
		if (!restrictionCheck(restriction, parameters)) {
		    /* Multiple could be both an explicit list of params and an ALL */
		    if (multipleLength == 0) {
			char marker[80];
			/* It wasn't ALL, we need to say something about it, so
			 * prepare a printout with $1/2/3
			 */
			sprintf(marker, "($%ld)", restriction->parameterNumber);
			output(marker);
			runRestriction(restriction);
			para();
		    }
		    multipleCandidates[i].instance = 0; /* In any case remove it from the list */
		}
	    }
	    parameters[restriction->parameterNumber-1].instance = 0;
	} else {
	    if (!restrictionCheck(restriction, parameters)) {
		runRestriction(restriction);
		abortPlayerCommand();
	    }
	}
	checked[restriction->parameterNumber - 1] = TRUE; /* Remember that it's already checked */
    }
}


/*----------------------------------------------------------------------*/
static void checkNonRestrictedParameters(Parameter parameters[], Bool checked[], Parameter multipleCandidates[]) {
    int parameterPosition;
    for (parameterPosition = 0; parameters[parameterPosition].instance != EOF; parameterPosition++)
        if (!checked[parameterPosition]) {
            if (parameters[parameterPosition].instance == 0) {
                /* This was a multiple parameter position, check all multiple candidates and remove failing */
                int i;
                for (i = 0; !isEndOfList(&multipleCandidates[i]); i++)
                    if (multipleCandidates[i].instance != 0) /* Skip any empty slots */
                        if (!isObject(multipleCandidates[i].instance))
                            multipleCandidates[i].instance = 0;
            } else if (!isObject(parameters[parameterPosition].instance))
                error(M_CANT0);
        }
}


/*----------------------------------------------------------------------*/
static void uncheckAllParameterPositions(Bool checked[]) {
    int position;
    for (position = 0; position < MAXPARAMS; position++)
        checked[position] = FALSE;
}


/*----------------------------------------------------------------------*/
static void restrictParameters(Parameter parameters[], Parameter multipleParameters[], ElementEntry *elms) {
    Bool checked[MAXPARAMS+1];  /* Is corresponding parameter checked? */
	
    uncheckAllParameterPositions(checked);
    checkRestrictedParameters(parameters, elms, multipleParameters, checked);
    checkNonRestrictedParameters(parameters, checked, multipleParameters);
}



/*----------------------------------------------------------------------*/
static void matchNounPhrase(Parameter parameter, Aint *(*adjectiveReferencesFinder)(int wordIndex), Aint *(*nounReferencesFinder)(int wordIndex)) {
    int i;
    
    for (i = parameter.firstWord; i < parameter.lastWord; i++)
        updateWithReferences(i, parameter.candidates, adjectiveReferencesFinder);
     updateWithReferences(parameter.lastWord, parameter.candidates, nounReferencesFinder);
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
static void matchParameters(Parameter parameters[], void (*matcher)(Parameter parameter)) 
{
     int i;
     
     for (i = 0; i < listLength(parameters); i++) {
          if (parameters[i].candidates == NULL)
               parameters[i].candidates = allocateParameterArray(NULL, MAXENTITY);
          matcher(parameters[i]);
     }
}



/*----------------------------------------------------------------------*/
static void try(Parameter parameters[], Parameter multipleParameters[]) {
    // TODO This is much too long, try to refactor out some functions
    ElementEntry *elms; /* Pointer to element list */
    SyntaxEntry *stx; /* Pointer to syntax parse list */
    Bool anyPlural = FALSE; /* Any parameter that was plural? */

    /*
     * TODO This code, and much of the above, should be refactored to
     * not do both parsing and parameter matching at the same time. It
     * should first parse and create a parameter array where each
     * entry indicates which words in the command line was "eaten" by
     * this parameter. Then each parameter position can be resolved
     * using those words.
     */

    /*
     * First find which syntax tree to start in
     */
    stx = findSyntax(verbWordCode);

    /*
     * Then match the parameters, and other words, according to the
     * syntax elements in the parse tree.
     */
    elms = parseElementTree(parameters, (ElementEntry *) pointerTo(stx->elms), &anyPlural, multipleParameters);
    if (elms == NULL)
        error(M_WHAT);
    if (elms->next == 0) { /* No verb code, verb not declared! */
        /* TODO Does this ever happen? */
        error(M_CANT0);
    }
	
    /*
     * Then we can map the parameters to the correct order, if it was a syntax synonym.
     * The flags field of EOS element is actually the syntax number!
     */
    current.verb = mapSyntax(elms->flags, parameters);

    /* WIP! Match parameters to instances */
    matchParameters(parameters, instanceMatcher);

    /*
     * Now perform restriction checks
     */
    restrictParameters(parameters, multipleParameters, elms);

    /* Finally, if we found some multiple, try to find out what was applicable */
    if (multipleLength > 0) {
        int multiplePosition = findMultiplePosition(parameters);
        disambiguateCandidatesForPosition(parameters, multiplePosition, multipleParameters);
        if (listLength(multipleParameters) == 0) {
            clearList(parameters);
            errorWhat(allWordIndex);
        }
    } else if (anyPlural) {
        compress(multipleParameters);
        if (listLength(multipleParameters) == 0)
            /* If there where multiple parameters but non left, exit without a */
            /* word, assuming we have already said enough */
            abortPlayerCommand();
    }
    plural = anyPlural; /* Remember if we found plural objects */
}


/*----------------------------------------------------------------------*/
static void parseCommand(Parameter parameters[], Parameter multipleParameters[])
{
    try(parameters, multipleParameters); /* ... to understand what he said */
	
    /* More on this line? */
    if (!endOfWords(wordIndex)) {
        if (isConjunctionWord(wordIndex))
            wordIndex++; /* If so skip the conjunction */
        else
            error(M_WHAT);
    }
}

/*======================================================================*/
void initParse(void) {
    int dictionaryIndex;
    int pronounIndex = 0;
	
    wordIndex = 0;
    continued = FALSE;
    ensureSpaceForPlayerWords(0);
    clearList(playerWords);
	
    pronouns = allocatePronounArray(pronouns);
    parameters = allocateParameterArray(parameters, MAXPARAMS);
    previousMultipleParameters = allocateParameterArray(previousMultipleParameters, MAXPARAMS);
	
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
static void notePronounParameters(Parameter parameters[]) {
    /* For all parameters note which ones can be referred to by a pronoun */
    Parameter *p;
	
    clearList(pronouns);
    for (p = parameters; !isEndOfList(p); p++) {
        int pronoun = pronounWordForInstance(p->instance);
        if (pronoun > 0)
            addPronounForInstance(pronoun, p->instance);
    }
}


/*======================================================================*/
void parse(Parameter parameters[]) {
    static Parameter *multipleParameters = NULL;
    multipleParameters = allocateParameterArray(multipleParameters, MAXPARAMS);

    if (endOfWords(wordIndex)) {
        wordIndex = 0;
        scan();
    } else if (anyOutput)
        para();
	
    capitalize = TRUE;
    multipleLength = 0;
    paramidx = 0;
    clearList(parameters);
	
    firstWord = wordIndex;
    if (isVerbWord(wordIndex)) {
        verbWord = playerWords[wordIndex].code;
        verbWordCode = dictionary[verbWord].code;
        wordIndex++;
        parseCommand(parameters, multipleParameters);
        notePronounParameters(parameters);
        fail = FALSE;
        action(parameters, multipleParameters);
    } else {
        clearList(previousMultipleParameters);
        clearList(pronouns);
        nonverb();
    }
    lastWord = wordIndex - 1;
    if (isConjunctionWord(lastWord))
        lastWord--;
	
    if (plural)
        copyParameterList(previousMultipleParameters, multipleParameters);
    else
        clearList(previousMultipleParameters);
}

/*----------------------------------------------------------------------*
  parse.c

  Command line parser unit for Alan interpreter ARUN

  \*----------------------------------------------------------------------*/

#include "parse.h"


/* IMPORTS */
#include <stdio.h>
#include <ctype.h>

#ifdef USE_READLINE
#include "readline.h"
#endif

#include "AltInfo.h"
#include "inter.h"
#include "current.h"
#include "act.h"
#include "term.h"
#include "lists.h"
#include "debug.h"
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
// TODO Remove dependency on exe.h (quitGame(), undo() and longjump)
#include "exe.h"


#ifdef HAVE_GLK
#include "glkio.h"
#endif


/* PRIVATE CONSTANTS */


/* PUBLIC DATA */


/* PRIVATE DATA */
static Bool plural = FALSE;

/* Syntax Parameters */
static int paramidx;
static Parameter *previousMultipleMatches; /* Previous multiple list */


/*======================================================================*/
void forceNewPlayerInput() {
    setEndOfList(&playerWords[wordIndex]);
}


/*----------------------------------------------------------------------*
  SCAN DATA & PROCEDURES

  All procedures for getting a command and turning it into a list of
  dictionary entries are placed here.

  \*----------------------------------------------------------------------*/

/* PRIVATE DATA */

static char buf[1000]; /* The input buffer */
static char isobuf[1000]; /* The input buffer in ISO */
static Bool eol = TRUE; /* Looking at End of line? Yes, initially */
static char *token;

/*----------------------------------------------------------------------*/
static void unknown(char token[]) {
    char *str = strdup(token);
    Parameter *messageParameters = allocateParameterArray(NULL);

#if ISO == 0
    fromIso(str, str);
#endif
    addParameterForString(messageParameters, str);
    printMessageWithParameters(M_UNKNOWN_WORD, messageParameters);
    error(NO_MSG);
    free(str);
}

/*----------------------------------------------------------------------*/
static int lookup(char wrd[]) {
    int i;

    for (i = 0; !isEndOfList(&dictionary[i]); i++) {
	if (compareStrings(wrd, (char *) pointerTo(dictionary[i].string)) == 0)
	    return (i);
    }
    unknown(wrd);
    return (EOF);
}

/*----------------------------------------------------------------------*/
static int number(char token[]) {
    int i;

    sscanf(token, "%d", &i);
    return i;
}

/*----------------------------------------------------------------------*/
static Bool isWordCharacter(int ch) {
    return isISOLetter(ch) || isdigit(ch) || ch == '\'' || ch == '-' || ch == '_';
}

/*----------------------------------------------------------------------*/
static char *gettoken(char *buf) {
    static char *marker;
    static char oldch;

    if (buf == NULL)
	*marker = oldch;
    else
	marker = buf;
    while (*marker != '\0' && isSpace(*marker) && *marker != '\n')
	marker++;
    buf = marker;
    if (isISOLetter(*marker))
	while (*marker && isWordCharacter(*marker))
	    marker++;
    else if (isdigit(*marker))
	while (isdigit(*marker))
	    marker++;
    else if (*marker == '\"') {
	marker++;
	while (*marker != '\"')
	    marker++;
	marker++;
    } else if (*marker == '\0' || *marker == '\n' || *marker == ';')
	return NULL;
    else
	marker++;
    oldch = *marker;
    *marker = '\0';
    return buf;
}

/*----------------------------------------------------------------------*/
// TODO replace dependency to exe.c with injection of quitGame() and undo()
static void getLine(void) {
    para();
    do {
#if defined(HAVE_ANSI) || defined(HAVE_GLK)
	statusline();
#endif
	printAndLog("> ");
#ifdef USE_READLINE
	if (!readline(buf)) {
	    newline();
	    quitGame();
	}
#else
	fflush(stdout);
	if (fgets(buf, LISTLEN, stdin) == NULL) {
	    newline();
	    quitGame();
	}
#endif
	getPageSize();
	anyOutput = FALSE;
	if (transcriptOption || logOption) {
#ifdef HAVE_GLK
	    glk_put_string_stream(logFile, buf);
	    glk_put_char_stream(logFile, '\n');
#else
#ifdef __amiga__
	    fprintf(logFile, "%s", buf);
#else
	    fprintf(logFile, "%s\n", buf);
#endif
#endif
	}
	/* If the player input an empty command he forfeited his command */
	if (strlen(buf) == 0) {
	    clearList(playerWords);
	    longjmp(forfeitLabel, 0);
	}

#if ISO == 0
	toIso(isobuf, buf, NATIVECHARSET);
#else
	strcpy(isobuf, buf);
#endif
	token = gettoken(isobuf);
	if (token != NULL) {
	    if (strcmp("debug", token) == 0 && header->debug) {
		debugOption = TRUE;
		debug(FALSE, 0, 0);
		token = NULL;
	    } else if (strcmp("undo", token) == 0) {
		token = gettoken(NULL);
		if (token != NULL) /* More tokens? */
		    error(M_WHAT);
		undo();
	    }
	}
    } while (token == NULL);
    eol = FALSE;
}


static Bool continued = FALSE;
/*----------------------------------------------------------------------*/
static void scan(void) {
    int i;
    int w;

    if (continued) {
	/* Player used '.' to separate commands. Read next */
	para();
	token = gettoken(NULL); /* Or did he just finish the command with a full stop? */
	if (token == NULL)
	    getLine();
	continued = FALSE;
    } else
	getLine();

    freeLiterals();
    playerWords[0].code = 0; // TODO This means what?
    i = 0;
    do {
	ensureSpaceForPlayerWords(i);
	playerWords[i].start = token;
	playerWords[i].end = strchr(token, '\0');
	if (isISOLetter(token[0])) {
	    w = lookup(token);
	    if (!isNoise(w))
		playerWords[i++].code = w;
	} else if (isdigit(token[0]) || token[0] == '\"') {
	    if (isdigit(token[0])) {
		createIntegerLiteral(number(token));
	    } else {
		char *unquotedString = strdup(token);
		unquotedString[strlen(token) - 1] = '\0';
		createStringLiteral(&unquotedString[1]);
		free(unquotedString);
	    }
	    playerWords[i++].code = dictionarySize + litCount; /* Word outside dictionary = literal */
	} else if (token[0] == ',') {
	    playerWords[i++].code = conjWord;
	} else if (token[0] == '.') {
	    continued = TRUE;
	    setEndOfList(&playerWords[i]);
	    eol = TRUE;
	    break;
	} else
	    unknown(token);
	setEndOfList(&playerWords[i]);
	eol = (token = gettoken(NULL)) == NULL;
    } while (!eol);
}

/*----------------------------------------------------------------------*
  Parameter allocation, saving and restoring

  \*----------------------------------------------------------------------*/

/* For parameters which are literals we need to trick message handling to
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
static int allLength; /* No. of objects matching 'all' */

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
    Parameter *parameters = allocateParameterArray(NULL);

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
    error(NO_MSG); /* Return with empty error message */
}

/*----------------------------------------------------------------------*/
static void errorWhichPronoun(int pronounWordIndex, Parameter alternatives[]) {
    int p; /* Index into the list of alternatives */
    Parameter *messageParameters = allocateParameterArray(NULL);

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
    error(NO_MSG);
}

/*----------------------------------------------------------------------*/
static void errorWhat(int playerWordIndex) {
    Parameter *messageParameters = allocateParameterArray(NULL);
    addParameterForWord(messageParameters, playerWordIndex);
    printMessageWithParameters(M_WHAT_WORD, messageParameters);
    free(messageParameters);
    error(NO_MSG);
}

/*----------------------------------------------------------------------*/
static void errorAfterBut(int butWordIndex) {
    Parameter *messageParameters = allocateParameterArray(NULL);
    addParameterForWord(messageParameters, butWordIndex);
    printMessageWithParameters(M_AFTER_BUT, messageParameters);
    free(messageParameters);
    error(NO_MSG);
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
    Parameter *messageParameters = allocateParameterArray(NULL);
    addParameterForWord(messageParameters, butWordIndex);
    addParameterForWord(messageParameters, fakePlayerWordForAll());
    printMessageWithParameters(M_BUT_ALL, messageParameters);
    error(NO_MSG);
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
static void resolve(Parameter matchedParameters[]) {
    // TODO Similar to disambiguate*() but this does error()
    /* In case the syntax did not indicate omnipotent powers (allowed
       access to remote object), we need to remove non-present
       parameters */

    int i;

    if (allLength > 0) /* ALL has already done this */
	return; /* TODO: NO IT HASN'T ALWAYS SINCE THIS CAN BE ANOTHER PARAMETER!!! */

    /* Resolve ambiguities by presence */
    for (i = 0; !isEndOfList(&matchedParameters[i]); i++) {
	if (!reachable(matchedParameters[i].instance)) {
	    errorNoSuch(matchedParameters[i]);
	}
    }
}


/*----------------------------------------------------------------------*/
static Aword *nounReferencesForWord(int wordIndex) {
    return (Aword *) pointerTo(dictionary[playerWords[wordIndex].code].nounRefs);
}


/*----------------------------------------------------------------------*/
static Aword *adjectiveReferencesForWord(int wordIndex) {
    return (Aword *) pointerTo(dictionary[playerWords[wordIndex].code].adjectiveRefs);
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
    pronounInstances = allocateParameterArray(pronounInstances);

    int pronounMatches = getPronounInstances(playerWords[wordIndex].code, pronounInstances);
    if (pronounMatches == 0)
	errorWhat(wordIndex);
    else if (pronounMatches > 1)
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
static void updateWithAdjectiveReferences(int wordIndex, Parameter parsedParameters[], Bool foundPreviousReferences) {
    static Parameter *references = NULL; /* Instances referenced by a word */
    references = allocateParameterArray(references);

    copyReferences(references, adjectiveReferencesForWord(wordIndex));
    if (foundPreviousReferences)
	intersect(parsedParameters, references);
    else
	copyParameterList(parsedParameters, references);
}


/*----------------------------------------------------------------------*/
static void updateWithNounReferences(int wordIndex, Parameter parsedParameters[], Bool foundPreviousReferences) {
    static Parameter *references = NULL; /* Instances referenced by a word */
    references = allocateParameterArray(references);

    copyReferences(references, nounReferencesForWord(wordIndex));
    if (foundPreviousReferences)
	intersect(parsedParameters, references);
    else
	copyParameterList(parsedParameters, references);
}


/*----------------------------------------------------------------------*/
static void disambiguateParameter(Parameter candidates[]) {
    int i;
#ifndef DISAMBIGUATE_USING_CHECKS
    for (i = 0; !isEndOfList(&candidates[i]); i++)
	if (!reachable(candidates[i].instance))
	    candidates[i].instance = 0;
    compress(candidates);
#else
    disambiguateUsingChecks(candidates, parameterPosition);
    /* We don't have the parameterPosition here.
       Maybe we can do this later? */
#endif
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
static void unambiguousNounPhrase(Parameter parameterCandidates[]) {
    // TODO This is really to long, need to break out some parts
    int firstWord, lastWord; /* The words the player used */

    static Parameter *savedParameters = NULL; /* Saved list for backup at EOF */

    savedParameters = allocateParameterArray(savedParameters);

    clearList(parameterCandidates);

    if (isLiteralWord(wordIndex)) {
	transformLiteralIntoSingleParameter(parameterCandidates);
	wordIndex++;
    } else if (isPronounWord(wordIndex)) {
	transformPronounIntoSingleParameter(parameterCandidates);
	wordIndex++; /* Consume the pronoun */
    } else {
	Bool found = FALSE; /* Adjective or noun found ? */

	static Parameter *references = NULL; /* Instances referenced by a word */
	references = allocateParameterArray(references);

	firstWord = wordIndex;
	while (anotherAdjective(wordIndex)) {
	    if (lastPossibleNoun(wordIndex))
		break;
	    copyParameterList(savedParameters, parameterCandidates); /* To save it for backtracking */
	    updateWithAdjectiveReferences(wordIndex, parameterCandidates, found);
	    found = TRUE;
	    wordIndex++;
	}
	if (!endOfWords(wordIndex)) {
	    if (isNounWord(wordIndex)) {
		updateWithNounReferences(wordIndex, parameterCandidates, found);
		found = TRUE;
		wordIndex++;
	    } else
		error(M_NOUN);
	} else if (found) {
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
	} else {
	    //printf("end and not found");
	}
	lastWord = wordIndex - 1;

	if (listLength(parameterCandidates) > 1)
	    disambiguateParameter(parameterCandidates);

	if (listLength(parameterCandidates) > 1)
	    errorWhichOne(parameterCandidates);
	else if (found && listLength(parameterCandidates) == 0) {
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

    if (tlst == NULL)
	tlst = (Parameter *) allocate(sizeof(Parameter) * (MAXENTITY+1));
    clearList(tlst);

    for (;;) {
	/* Special handling here since THEM_WORD is a common pronoun, so
	   we check if it is also a pronoun, if it is but there is a list of
	   multi parameters from the previous command, we assume that is
	   what he ment */
	if (isThemWord(wordIndex) && ((isPronounWord(wordIndex)
				       && listLength(previousMultipleMatches) > 0) || !isPronounWord(wordIndex))) {
	    plural = TRUE;
	    for (i = 0; !isEndOfList(&previousMultipleMatches[i]); i++)
		if (!reachable(previousMultipleMatches[i].instance))
		    previousMultipleMatches[i].instance = 0;
	    compress(previousMultipleMatches);
	    if (listLength(previousMultipleMatches) == 0)
		errorWhat(wordIndex);
	    copyParameterList(olst, previousMultipleMatches);
	    olst[0].firstWord = EOF; /* No words used */
	    wordIndex++;
	} else {
	    unambiguousNounPhrase(olst); /* Look for unambigous noun phrase */
	    if (listLength(olst) == 0) { /* Failed! */
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

    if (allList == NULL)
	allList = (Parameter *) allocate((MAXENTITY+1) * sizeof(Parameter));

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
	allLength = listLength(parsedParameters);
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
    Parameter originalParameters[MAXPARAMS];

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

static Bool hasBit(Aword flags, Aword bit) {
    return (flags & bit) != 0;
}

/*----------------------------------------------------------------------*/
static void parseParameter(Parameter parameters[], Aword flags, Bool *anyPlural, Parameter multipleList[]) {
    Parameter *parsedParameters = allocateParameterArray(NULL); /* List of parameters parsed, possibly multiple */

    plural = FALSE;
    complex(parsedParameters);
    if (listLength(parsedParameters) == 0) /* No object!? */
	error(M_WHAT);
    if (!hasBit(flags, OMNIBIT))
	/* If its not an omnipotent parameter, resolve by presence */
	resolve(parsedParameters);
    if (plural) {
	if (!hasBit(flags, MULTIPLEBIT)) /* Allowed multiple? */
	    error(M_MULTIPLE);
	else {
	    /* Mark this as the multiple position in which to insert actual
	       parameter values later */
	    parameters[paramidx++].instance = 0;
	    copyParameterList(multipleList, parsedParameters);
	    *anyPlural = TRUE;
	}
    } else
	parameters[paramidx++] = parsedParameters[0];

    setEndOfList(&parameters[paramidx]);

    free(parsedParameters);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchParameterElement(ElementEntry *elms) {
    /* Require a parameter if elms->code == 0! */
    while (!isEndOfList(elms) && elms->code != 0)
	elms++;
    if (isEndOfList(elms))
	return NULL;
    return (elms);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchEndOfSyntax(ElementEntry *elms) {
    while (!isEndOfList(elms) && elms->code != EOS)
	elms++;
    if (isEndOfList(elms)) /* No match for EOS! */
	return NULL;
    return (elms);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchWordElement(ElementEntry *elms, Aint wordCode) {
    while (!isEndOfList(elms) && elms->code != wordCode)
	elms++;
    if (isEndOfList(elms))
	return NULL;
    return (elms);
}

/*----------------------------------------------------------------------*/
static Bool isParameterWord(int wordIndex) {
    return isNounWord(wordIndex) || isAdjectiveWord(wordIndex) || isAllWord(wordIndex)
	|| isLiteralWord(wordIndex) || isItWord(wordIndex) || isThemWord(wordIndex) || isPronounWord(wordIndex);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchParseTree(Parameter parameters[], ElementEntry *elms, Bool *anyPlural, Parameter multipleList[]) {
    ElementEntry *currentEntry = elms;

    while (TRUE) { /* Traverse the possible branches to find a match */
	ElementEntry *elms;

	/* End of input? */
	// TODO If conjunction word is also some other type of word, like noun?
	if (endOfWords(wordIndex) || isConjunctionWord(wordIndex)) {
	    elms = matchEndOfSyntax(currentEntry);
	    if (elms == NULL)
		return NULL;
	    else
		return elms;
	}

	if (isParameterWord(wordIndex)) {
	    elms = matchParameterElement(currentEntry);
	    if (elms != NULL) {
		parseParameter(parameters, elms->flags, anyPlural, multipleList);
		currentEntry = (ElementEntry *) pointerTo(elms->next);
		continue;
	    } /* didn't allow a parameter so fall through
		 and try with a preposition... */
	}

	if (isPrepositionWord(wordIndex)) {
	    /* A preposition? Or rather, an intermediate word? */
	    elms = matchWordElement(currentEntry, dictionary[playerWords[wordIndex].code].code);
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
    syserr("Fall-through in 'matchParseTree()'");
    return NULL;
}

/*----------------------------------------------------------------------*/
static SyntaxEntry *findSyntax(int verbCode) {
    SyntaxEntry *stx;
    for (stx = stxs; !isEndOfList(stx); stx++)
	if (stx->code == verbCode)
	    return stx;
    return NULL;
}


/*----------------------------------------------------------------------*/
static void uncheckAllParameterPositions(Parameter parameters[], Bool checked[]) {
    int position;
    for (position = 0; !isEndOfList(&parameters[position]); position++)
	checked[position] = FALSE;
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
		    if (allLength == 0) {
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
		error(MSGMAX); /* Return to player without saying anything */
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
static void try(Parameter parameters[], Parameter multipleParameters[]) {
    // TODO This is much too long, try to refactor out some functions
    ElementEntry *elms; /* Pointer to element list */
    SyntaxEntry *stx; /* Pointer to syntax parse list */
    Bool anyPlural = FALSE; /* Any parameter that was plural? */
    static Bool *checked = NULL; /* Corresponding parameter checked? */

    if (checked == NULL)
	checked = (Bool *) allocate((MAXENTITY+1) * sizeof(Bool));

    stx = findSyntax(verbWordCode);
    if (stx == NULL)
	error(M_WHAT);

    elms = matchParseTree(parameters, (ElementEntry *) pointerTo(stx->elms), &anyPlural, multipleParameters);
    if (elms == NULL)
	error(M_WHAT);

    /* Set verb code and map parameters */
    /* Flags field of EOS element is actually the syntax number! */
    current.verb = mapSyntax(elms->flags, parameters);

    /* Now perform class restriction checks */
    if (elms->next == 0) /* No verb code, verb not declared! */
	error(M_CANT0);

    uncheckAllParameterPositions(parameters, checked);
    checkRestrictedParameters(parameters, elms, multipleParameters, checked);
    checkNonRestrictedParameters(parameters, checked, multipleParameters);

    /* Finally, if ALL was used, try to find out what was applicable */
    if (allLength > 0) {
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
	    error(MSGMAX);
    }
    plural = anyPlural; /* Remember if we found plural objects */
}


/*----------------------------------------------------------------------*/
static void match(Parameter parameters[], Parameter multipleParameters[])
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
    parameters = allocateParameterArray(parameters);
    previousMultipleMatches = allocateParameterArray(previousMultipleMatches);

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
    static Parameter *multipleMatches = NULL;
    multipleMatches = allocateParameterArray(multipleMatches);

    if (endOfWords(wordIndex)) {
	wordIndex = 0;
	scan();
    } else if (anyOutput)
	para();

    capitalize = TRUE;
    allLength = 0;
    paramidx = 0;
    clearList(parameters);

    firstWord = wordIndex;
    if (isVerbWord(wordIndex)) {
	verbWord = playerWords[wordIndex].code;
	verbWordCode = dictionary[verbWord].code;
	wordIndex++;
	match(parameters, multipleMatches);
	notePronounParameters(parameters);
	fail = FALSE;
	action(parameters, multipleMatches);
    } else {
	clearList(previousMultipleMatches);
	clearList(pronouns);
	nonverb();
    }
    lastWord = wordIndex - 1;
    if (isConjunctionWord(lastWord))
	lastWord--;

    if (plural)
	copyParameterList(previousMultipleMatches, multipleMatches);
    else
	clearList(previousMultipleMatches);
}

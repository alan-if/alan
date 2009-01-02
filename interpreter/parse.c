/*----------------------------------------------------------------------*
 parse.c

 Command line parser unit for Alan interpreter ARUN

 \*----------------------------------------------------------------------*/

#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#ifdef USE_READLINE
#include "readline.h"
#endif

#include "AltInfoArray.h"
#include "main.h"
#include "inter.h"
#include "exe.h"
#include "act.h"
#include "term.h"
#include "debug.h"
#include "params.h"
#include "options.h"
#include "syserr.h"
#include "Location.h"

#ifdef HAVE_GLK
#include "glkio.h"
#endif

#define LISTLEN 100
/* PUBLIC DATA */

/* List of parsed words, index into dictionary */
WordEntry playerWords[LISTLEN/2] = { { EOF, NULL, NULL}};
int wordIndex; /* An index into it that list */
int firstWord, lastWord;  /* Index for the first and last words for this command */

static Bool plural = FALSE;

/* Syntax Parameters */
static int paramidx;
ParamEntry *parameters; /* List of params */
static ParamEntry *previousMultipleMatches; /* Previous multiple list */


/* Literals */
LiteralEntry literal[MAXPARAMS+1];
static int litCount;

/* What did the user say? */
int verbWord; /* The word he used, dictionary index */
int verbWordCode; /* The code for that verb */

/*======================================================================*/
void forceNewPlayerInput() {
	setEndOfList(&playerWords[wordIndex]);
}

/*======================================================================*/
char *playerWordsAsCommandString(void) {
	int n;
	char *commandString;
	n = playerWords[lastWord].end - playerWords[firstWord].start;
	commandString = allocate(n + 1);
	strncpy(commandString, playerWords[firstWord].start, n);
	return commandString;
}

/*----------------------------------------------------------------------*
 SCAN DATA & PROCEDURES

 All procedures for getting a command and turning it into a list of
 dictionary entries are placed here.

 \*----------------------------------------------------------------------*/

/* PRIVATE DATA */

static char buf[LISTLEN+1]; /* The input buffer */
static char isobuf[LISTLEN+1]; /* The input buffer in ISO */
static Bool eol = TRUE; /* Looking at End of line? Yes, initially */
static char *token;

/*----------------------------------------------------------------------*/
static void unknown(char token[]) {
	char *str = strdup(token);
	ParamEntry *messageParameters = createParameterList(NULL);

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
	return isISOLetter(ch) || isdigit(ch) || ch == '\'' || ch == '-' || ch
			== '_';
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

/*======================================================================*/
int literalFromInstance(Aint instance) {
	return instance - header->instanceMax;
}

/*======================================================================*/
Aint instanceFromLiteral(int literalIndex) {
	return literalIndex + header->instanceMax;
}

/*----------------------------------------------------------------------*/
static void createIntegerLiteral(int integerValue) {
	litCount++;
	if (litCount > MAXPARAMS)
		syserr("Too many player command parameters.");

	literal[litCount].class = header->integerClassId;
	literal[litCount].type = NUMERIC_LITERAL;
	literal[litCount].value = integerValue;
}

/*----------------------------------------------------------------------*/
static void createStringLiteral(char *unquotedString) {
	litCount++;
	if (litCount > MAXPARAMS)
		syserr("Too many player command parameters.");
	literal[litCount].class = header->stringClassId;
	literal[litCount].type = STRING_LITERAL;
	literal[litCount].value = (Aword) strdup(unquotedString);
}

/*----------------------------------------------------------------------*/
static void freeLiterals() {
	int i;

	for (i = 0; i < litCount; i++)
		if (literal[i].type == STRING_LITERAL && literal[i].value != 0)
			free((char*) literal[i].value);
}

static Bool continued = FALSE;
/*----------------------------------------------------------------------*/
static void scan(void) {
	int i;
	int w;

	if (continued) {
		/* Player used '.' to separate commands. Read next */
		para();
		token = gettoken(NULL);
		if (token == NULL)
			getLine();
		continued = FALSE;
	} else
		getLine();

	freeLiterals();
	litCount = 0;
	playerWords[0].code = 0; // TODO This means what?
	i = 0;
	do {
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
			playerWords[i++].code = dictsize + litCount; /* Word outside dictionary = literal */
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

/*======================================================================*/
static void allocateParamEntryArea(ParamEntry **area) {
	if (*area == NULL) {
		*area = (ParamEntry *) allocate(sizeof(ParamEntry)
				* (header->instanceMax + 1));
		(*area)->instance = EOF;
	}
}


/* For parameters which are literals we need to trick message handling to output the word
 * and create a string literal instance if anyone wants to refer to an attribute of it
 * (literals inherit from entity so application can have added an attribute) */

/*----------------------------------------------------------------------*/
static void addParameterForWord(ParamEntry *parameters, int wordIndex) {
	ParamEntry *parameter = findEndOfList(parameters);

	createStringLiteral(pointerTo(dictionary[playerWords[wordIndex].code].string));
	parameter->instance = instanceFromLiteral(litCount); /* A faked literal */
	parameter->useWords = TRUE;
	parameter->firstWord = parameter->lastWord = wordIndex;
	setEndOfList(parameter+1);
}

/*----------------------------------------------------------------------*/
static void addParameterForWords(ParamEntry *parameters, int firstWordIndex, int lastWordIndex) {
	ParamEntry *parameter = findEndOfList(parameters);

	// TODO Need to concat the words into a string literal, instead of just using the last word
	createStringLiteral(pointerTo(dictionary[playerWords[lastWordIndex].code].string));
	parameter->instance = instanceFromLiteral(litCount);
	parameter->useWords = TRUE;
	parameter->firstWord = firstWordIndex;
	parameter->lastWord = lastWordIndex;
	setEndOfList(parameter+1);
}

/*======================================================================*/
void addParameterForInteger(ParamEntry *parameters, Aint value) {
	ParamEntry *parameter = findEndOfList(parameters);

	createIntegerLiteral(value);
	parameter->instance = instanceFromLiteral(litCount);
	parameter->useWords = FALSE;

	setEndOfList(parameter+1);
}

/*======================================================================*/
void addParameterForString(ParamEntry *parameters, char *value) {
	ParamEntry *parameter = findEndOfList(parameters);

	createStringLiteral(value);
	parameter->instance = instanceFromLiteral(litCount);
	parameter->useWords = FALSE;

	setEndOfList(parameter+1);
}

/*======================================================================*/
void addParameterForInstance(ParamEntry *parameters, Aint instance) {
	ParamEntry *parameter = findEndOfList(parameters);

	parameter->instance = instance;
	parameter->useWords = FALSE;

	setEndOfList(parameter+1);
}


/*----------------------------------------------------------------------*
 PARSE DATA & PROCEDURES

 All procedures and data for getting a command and parsing it

 \*---------------------------------------------------------------------- */

/* Private Types */

typedef struct PronounEntry { /* To remember parameter/pronoun relations */
	int pronoun;
	int instance;
} PronounEntry;

static PronounEntry *pronounList = NULL;
static int allWordIndex; /* Word index of the ALL_WORD found */
static int butWordIndex; /* Word index of the BUT_WORD found */
static int allLength; /* No. of objects matching 'all' */

/*----------------------------------------------------------------------*/
static void nonverb(void) {
	if (isDir(playerWords[wordIndex].code)) {
		wordIndex++;
		if (!isEndOfList(&playerWords[wordIndex])
				&& !isConj(playerWords[wordIndex].code))
			error(M_WHAT);
		else
			go(current.location,
					dictionary[playerWords[wordIndex - 1].code].code);
		if (!isEndOfList(&playerWords[wordIndex]))
			wordIndex++;
	} else
		error(M_WHAT);
}

/*----------------------------------------------------------------------*/
static void errorWhichOne(ParamEntry alternative[]) {
	int p; /* Index into the list of alternatives */
	ParamEntry *parameters = createParameterList(NULL);

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
static void errorWhichPronoun(int pronounWordIndex, ParamEntry alternatives[]) {
	int p; /* Index into the list of alternatives */
	ParamEntry *parameters = createParameterList(NULL);

	addParameterForWord(parameters, pronounWordIndex);
	printMessageWithParameters(M_WHICH_PRONOUN_START, parameters);

	parameters[0] = alternatives[0];
	setEndOfList(&parameters[1]);
	printMessageWithParameters(M_WHICH_PRONOUN_FIRST, parameters);

	for (p = 1; !isEndOfList(&alternatives[p+1]); p++) {
		parameters[0] = alternatives[p];
		printMessageWithParameters(M_WHICH_ONE_COMMA, parameters);
	}
	parameters[0] = alternatives[p];
	printMessageWithParameters(M_WHICH_ONE_OR, parameters);
	free(parameters);
	error(NO_MSG); /* Return with empty error message */
}

/*----------------------------------------------------------------------*/
static void errorWhat(int playerWordIndex) {
	ParamEntry *messageParameters = createParameterList(NULL);
	addParameterForWord(messageParameters, playerWordIndex);
	printMessageWithParameters(M_WHAT_WORD, messageParameters);
	free(messageParameters);
	error(NO_MSG);
}

/*----------------------------------------------------------------------*/
static void errorAfterBut() {
	ParamEntry *messageParameters = createParameterList(NULL);
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
	for (d = 0; d < dictsize; d++)
		if (isAll(d)) {
			playerWords[p].code = d;
			return p;
		}
	syserr("No ALLWORD found");
	return 0;
}

/*----------------------------------------------------------------------*/
static void errorButAfterAll(int butWordIndex) {
	ParamEntry *messageParameters = createParameterList(NULL);
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
static void errorNoSuch(ParamEntry parameter) {
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
static void buildAll(ParamEntry list[]) {
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
static int getPronounInstances(int word, ParamEntry instanceParameters[]) {
	/* Find the instance that the pronoun word could refer to, return 0
	 if none or multiple */
	int p;
	int i = 0;

	clearList(instanceParameters);
	for (p = 0; !isEndOfList(&pronounList[p]); p++)
		if (dictionary[word].code == pronounList[p].pronoun) {
			instanceParameters[i].instance = pronounList[p].instance;
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
		if (attributeOf(instance, OPAQUEATTRIBUTE))
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
static void resolve(ParamEntry matchedParameters[]) {
	/* In case the syntax did not indicate omnipotent powers (allowed
	 access to remote object), we need to remove non-present
	 parameters */

	int i;

	if (allLength > 0) /* ALL has already done this */
		return; /* TODO: NO IT HASN'T ALWAYS SINCE THIS CAN BE ANOTHER PARAMETER!!! */

	/* Resolve ambiguities by presence */
	for (i = 0; !isEndOfList(&matchedParameters[i]); i++) {
		if (isLiteral(matchedParameters[i].instance)) /* Literals are always 'here' */
			continue;
		if (instances[matchedParameters[i].instance].parent == ENTITY)
			// TODO This should really traverse the inheritance to ensure
			// Location and Object is not in it.
			/* Pure entities are also here */
			continue;
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
static Aword *adjectivReferencesForWord(int wordIndex) {
	return (Aword *) pointerTo(dictionary[playerWords[wordIndex].code].adjectiveRefs);
}


/*----------------------------------------------------------------------*/
static void unambiguousNounPhrase(ParamEntry parsedNouns[]) {
	int i;
	Bool foundNoun = FALSE; /* Adjective or noun found ? */
	static ParamEntry *references; /* Instances referenced by a word */
	static ParamEntry *savedNouns; /* Saved list for backup at EOF */
	int firstWord, lastWord; /* The words the player used */

	allocateParamEntryArea(&references);
	allocateParamEntryArea(&savedNouns);

	if (isLiteralWord(playerWords[wordIndex].code)) {
		/* Transform the word into a reference to the literal value */
		/* words > dictsize are literals with index = word-dictsize */
		parsedNouns[0].instance = (playerWords[wordIndex].code - dictsize)
				+ header->instanceMax;
		parsedNouns[0].firstWord = EOF; /* No words used! */
		setEndOfList(&parsedNouns[1]);
		wordIndex++;
	} else {

		clearList(parsedNouns);
		if (isPronoun(playerWords[wordIndex].code)) {
			ParamEntry *pronounInstances = createParameterList(NULL);
			int pronounMatches = getPronounInstances(playerWords[wordIndex].code, pronounInstances);
			if (pronounMatches == 0)
				errorWhat(wordIndex);
			else if (pronounMatches > 1) {
				/* Set up parameters for error message... */
				parameters[0].instance = 0; /* Just make it anything != EOF */
				setEndOfList(&parameters[1]);
				errorWhichPronoun(wordIndex, pronounInstances);
			}
			wordIndex++; /* Consume the pronoun */
			parsedNouns[0].instance = pronounInstances[0].instance;
			parsedNouns[0].firstWord = EOF; /* No words used! */
			setEndOfList(&parsedNouns[1]);
			free(pronounInstances);
			return;
		}

		firstWord = wordIndex;
		while (!isEndOfList(&playerWords[wordIndex]) && isAdjective(playerWords[wordIndex].code)) {
			/* If this word can be a noun and there is no noun following, break the loop */
			if (isNoun(playerWords[wordIndex].code) && (isEndOfList(&playerWords[wordIndex+1]) || !isNoun(playerWords[wordIndex+1].code)))
				break;
			copyReferences(references, adjectivReferencesForWord(wordIndex));
			copyParameterList(savedNouns, parsedNouns); /* To save it for backtracking */
			if (foundNoun)
				intersect(parsedNouns, references);
			else {
				copyParameterList(parsedNouns, references);
				foundNoun = TRUE;
			}
			wordIndex++;
		}
		if (!isEndOfList(&playerWords[wordIndex])) {
			if (isNoun(playerWords[wordIndex].code)) {
				copyReferences(references, nounReferencesForWord(wordIndex));
				if (foundNoun)
					intersect(parsedNouns, references);
				else {
					copyParameterList(parsedNouns, references);
					foundNoun = TRUE;
				}
				wordIndex++;
			} else
				error(M_NOUN);
		} else if (foundNoun) {
			/* Perhaps the last word was also a noun? */
			if (isNoun(playerWords[wordIndex - 1].code)) {
				// TODO When does this get executed?
				copyParameterList(parsedNouns, savedNouns); /* Restore to before last adjective */
				copyReferences(references, nounReferencesForWord(wordIndex-1));
				if (isEndOfList(&parsedNouns[0]))
					copyParameterList(parsedNouns, references);
				else
					intersect(parsedNouns, references);
			} else
				error(M_NOUN);
		}
		lastWord = wordIndex - 1;

		/* Allow remote objects, but resolve ambiguities by reachability */
		if (listLength(parsedNouns) > 1) {
			for (i = 0; !isEndOfList(&parsedNouns[i]); i++)
				if (!reachable(parsedNouns[i].instance))
					parsedNouns[i].instance = 0;
			compress(parsedNouns);
		}
#ifdef DISAMBIGUATE_USING_CHECKS
		if (listLength(parsedNouns)> 1)
		disambiguateUsingChecks(parsedNouns, parameterPosition);
		/* We don't have the parameterPosition here.
		 Maybe we can do this later? */
#endif

		if (listLength(parsedNouns) > 1)
			errorWhichOne(parsedNouns);
		else if (foundNoun && listLength(parsedNouns) == 0) {
			ParamEntry parameter;
			parameter.instance = 0; /* Just make it anything != EOF */
			parameter.firstWord = firstWord;
			parameter.lastWord = lastWord;
			errorNoSuch(parameter);
		}

		parsedNouns[0].firstWord = firstWord;
		parsedNouns[0].lastWord = lastWord;

	}
}

/*----------------------------------------------------------------------*/
static void simple(ParamEntry olst[]) {
	static ParamEntry *tlst = NULL;
	int savidx = wordIndex;
	Bool savplur = FALSE;
	int i;

	if (tlst == NULL)
		tlst = (ParamEntry *) allocate(sizeof(ParamEntry) * (MAXENTITY+1));
	clearList(tlst);

	for (;;) {
		/* Special handling here since THEM_WORD is a common pronoun, so
		 we check if it is also a pronoun, if it is but there is a list of
		 multi parameters from the previous command, we assume that is
		 what he ment */
		if (isThem(playerWords[wordIndex].code) && ((isPronoun(playerWords[wordIndex].code)
				&& listLength(previousMultipleMatches) > 0) || !isPronoun(playerWords[wordIndex].code))) {
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
		if (playerWords[wordIndex].code != EOF && (isConj(
				playerWords[wordIndex].code) && (isAdjective(
				playerWords[wordIndex + 1].code) || isNoun(
				playerWords[wordIndex + 1].code)))) {
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
static void complex(ParamEntry olst[]) {
	static ParamEntry *alst = NULL;

	if (alst == NULL)
		alst = (ParamEntry *) allocate((MAXENTITY+1) * sizeof(ParamEntry));

	if (isAll(playerWords[wordIndex].code)) {
		plural = TRUE;
		buildAll(alst); /* Build list of all objects */
		wordIndex++;
		if (!isEndOfList(&playerWords[wordIndex]) && isBut(playerWords[wordIndex].code)) {
			butWordIndex = wordIndex;
			wordIndex++;
			simple(olst);
			if (listLength(olst) == 0)
				errorAfterBut();
			subtractListFromList(alst, olst);
			if (listLength(alst) == 0)
				error(M_NOT_MUCH);
		}
		copyParameterList(olst, alst);
		allLength = listLength(olst);
	} else
		simple(olst); /* Look for simple noun group */
}

/*----------------------------------------------------------------------*/
static Bool restrictionCheck(RestrictionEntry *restriction) {
	if (restriction->class == RESTRICTIONCLASS_CONTAINER)
		return instances[parameters[restriction->parameter - 1].instance].container
				!= 0;
	else
		return isA(parameters[restriction->parameter - 1].instance,
				restriction->class);
}

/*----------------------------------------------------------------------*/
static void runRestriction(RestrictionEntry *restriction) {
	if (sectionTraceOption)
		printf("\n<SYNTAX parameter #%ld Is Not of class %ld:>\n",
				restriction->parameter, restriction->class);
	if (restriction->stms)
		interpret(restriction->stms);
	else
		error(M_CANT0);
}

/*----------------------------------------------------------------------*/
static Aint mapSyntax(Aint syntaxNumber) {
	/*
	 Find the syntax map, use the verb code from it and remap the parameters
	 */
	ParameterMapEntry *parameterMapTable;
	Aword *parameterMap;
	Aint parameterNumber;
	ParamEntry originalParameters[MAXPARAMS];

	for (parameterMapTable = pointerTo(header->parameterMapAddress); !isEndOfList(parameterMapTable); parameterMapTable++)
		if (parameterMapTable->syntaxNumber == syntaxNumber)
			break;
	if (isEndOfList(parameterMapTable)) syserr(
				"Could not find syntax in mapping table.");

	parameterMap = pointerTo(parameterMapTable->parameterMapping);
	copyParameterList(originalParameters, parameters);
	for (parameterNumber = 1; !isEndOfList(&originalParameters[parameterNumber-1]); parameterNumber++)
		parameters[parameterNumber - 1]
				= originalParameters[parameterMap[parameterNumber - 1] - 1];

	return parameterMapTable->verbCode;
}

static Bool hasBit(Aword flags, Aword bit) {
	return (flags & bit) != 0;
}

/*----------------------------------------------------------------------*/
static void parseParameter(Aword flags, Bool *anyPlural,
		ParamEntry multipleList[]) {
	ParamEntry *parsedParameters = createParameterList(NULL); /* List of parameters parsed, possibly multiple */

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
static Bool isParameterWord(int word) {
	return isNoun(word) || isAdjective(word) || isAll(word) || isLiteralWord(
			word) || isIt(word) || isThem(word) || isPronoun(word);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchParseTree(ParamEntry multipleList[],
		ElementEntry *elms, Bool *anyPlural) {
	ElementEntry *currentEntry = elms;

	while (TRUE) { /* Traverse the possible branches to
	 find a match */
		ElementEntry *elms;

		/* End of input? */
		if (isEndOfList(&playerWords[wordIndex]) || isConj(playerWords[wordIndex].code)) {
			elms = matchEndOfSyntax(currentEntry);
			if (elms == NULL)
				return NULL;
			else
				return elms;
		}

		if (isParameterWord(playerWords[wordIndex].code)) {
			elms = matchParameterElement(currentEntry);
			if (elms != NULL) {
				parseParameter(elms->flags, anyPlural, multipleList);
				currentEntry = (ElementEntry *) pointerTo(elms->next);
				continue;
			} /* didn't allow a parameter so fall through
			 and try with a preposition... */
		}

		if (isPreposition(playerWords[wordIndex].code)) {
			/* A preposition? Or rather, an intermediate word? */
			elms = matchWordElement(currentEntry,
					dictionary[playerWords[wordIndex].code].code);
			if (elms != NULL) {
				wordIndex++; /* Word matched, go to next */
				currentEntry = (ElementEntry *) pointerTo(elms->next);
				continue;
			}
		}

		if (isBut(playerWords[wordIndex].code))
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
			return (stx);
	return (NULL);
}

/*----------------------------------------------------------------------*/
static void disambiguate(ParamEntry candidates[], int position) {
	int i;
	for (i = 0; !isEndOfList(&candidates[i]); i++) {
		if (candidates[i].instance != 0) { /* Already empty? */
			parameters[position] = candidates[i];
			if (!reachable(candidates[i].instance) || !possible())
				candidates[i].instance = 0; /* Then remove this candidate from list */
		}
	}
	compress(candidates);
}

/*----------------------------------------------------------------------*/
static void try(ParamEntry multipleParameters[]) {
	ElementEntry *elms; /* Pointer to element list */
	SyntaxEntry *stx; /* Pointer to syntax parse list */
	RestrictionEntry *restriction; /* Pointer to class restrictions */
	Bool anyPlural = FALSE; /* Any parameter that was plural? */
	int i, multiplePosition;
	static ParamEntry *tlst = NULL; /* List of params found by complex() */
	static Bool *checked = NULL; /* Corresponding parameter checked? */

	if (tlst == NULL) {
		tlst = (ParamEntry *) allocate((MAXENTITY+1) * sizeof(ParamEntry));
		checked = (Bool *) allocate((MAXENTITY+1) * sizeof(Bool));
	}

	stx = findSyntax(verbWordCode);
	if (stx == NULL)
		error(M_WHAT);

	elms = matchParseTree(tlst, (ElementEntry *) pointerTo(stx->elms), &anyPlural);
	if (elms == NULL)
		error(M_WHAT);
	if (anyPlural)
		copyParameterList(multipleParameters, tlst);

	/* Set verb code and map parameters */
	current.verb = mapSyntax(elms->flags); /* Flags of EOS element is
	 actually syntax number! */

	/* Now perform class restriction checks */
	if (elms->next == 0) /* No verb code, verb not declared! */
		error(M_CANT0);

	for (multiplePosition = 0; !isEndOfList(&parameters[multiplePosition]); multiplePosition++)
		/* Mark all parameters unchecked */
		checked[multiplePosition] = FALSE;
	for (restriction = (RestrictionEntry *) pointerTo(elms->next); !isEndOfList(restriction); restriction++) {
		if (parameters[restriction->parameter - 1].instance == 0) {
			/* This was a multiple parameter, so check all and remove failing */
			for (i = 0; multipleParameters[i].instance != EOF; i++) {
				parameters[restriction->parameter - 1] = multipleParameters[i];
				if (!restrictionCheck(restriction)) {
					/* Multiple could be both an explicit list of params and an ALL */
					if (allLength == 0) {
						char marker[80];
						/* It wasn't ALL, we need to say something about it, so
						 * prepare a printout with $1/2/3
						 */
						sprintf(marker, "($%ld)", restriction->parameter);
						output(marker);
						runRestriction(restriction);
						para();
					}
					multipleParameters[i].instance = 0; /* In any case remove it from the list */
				}
			}
			parameters[restriction->parameter - 1].instance = 0;
		} else {
			if (!restrictionCheck(restriction)) {
				runRestriction(restriction);
				error(MSGMAX); /* Return to player without saying anything */
			}
		}
		checked[restriction->parameter - 1] = TRUE; /* Remember that it's already checked */
	}
	/* Now check the rest of the parameters, must be objects */
	for (multiplePosition = 0; parameters[multiplePosition].instance != EOF; multiplePosition++)
		if (!checked[multiplePosition]) {
			if (parameters[multiplePosition].instance == 0) {
				/* This was a multiple parameter, check all and remove failing */
				for (i = 0; !isEndOfList(&multipleParameters[i]); i++)
					if (multipleParameters[i].instance != 0) /* Skip any empty slots */
						if (!isObject(multipleParameters[i].instance))
							multipleParameters[i].instance = 0;
			} else if (!isObject(parameters[multiplePosition].instance))
				error(M_CANT0);
		}

	/* Finally, if ALL was used, try to find out what was applicable */
	if (allLength > 0) {
		for (multiplePosition = 0; parameters[multiplePosition].instance != 0; multiplePosition++)
			; /* Iterate over parameters to find multiple position */
		disambiguate(multipleParameters, multiplePosition);
		parameters[multiplePosition].instance = 0; /* Restore multiple marker */
		if (listLength(multipleParameters) == 0) {
			setEndOfList(&parameters[0]);
			errorWhat(allWordIndex);
		}
	} else if (anyPlural) {
		compress(multipleParameters);
		if (listLength(multipleParameters) == 0)
			/* If there where multiple parameters but non left, exit without a */
			/* word, assuming we have already said enough */
			error(MSGMAX);
	}
	plural = anyPlural; /* Remember that we found plural objects */
}

/*----------------------------------------------------------------------*/
static void match(ParamEntry *mlst) /* OUT - List of params allowed by multiple */
{
	try(mlst); /* ... to understand what he said */
	if (!isEndOfList(&playerWords[wordIndex]) && !isConj(playerWords[wordIndex].code))
		error(M_WHAT);
	if (!isEndOfList(&playerWords[wordIndex])) /* More on this line? */
		wordIndex++; /* If so skip the AND */
}

/*======================================================================*/
void initParse(void) {
	int dictionaryIndex;
	int pronounIndex = 0;

	wordIndex = 0;
	continued = FALSE;
	clearList(playerWords);

	if (pronounList == NULL)
		pronounList = allocate((MAXPARAMS+1) * sizeof(PronounEntry));

	parameters = createParameterList(parameters);
	previousMultipleMatches = createParameterList(previousMultipleMatches);

	for (dictionaryIndex = 0; dictionaryIndex < dictsize; dictionaryIndex++)
		if (isPronoun(dictionaryIndex)) {
			pronounList[pronounIndex].pronoun
					= dictionary[dictionaryIndex].code;
			pronounList[pronounIndex].instance = 0;
			pronounIndex++;
		}
}

/*----------------------------------------------------------------------*/
static int pronounForInstance(int instance) {
	/* Scan through the dictionary to find any pronouns that can be used
	 for this instance */
	int w;

	for (w = 0; w < dictsize; w++)
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
static void enterPronoun(int pronoun, int instanceCode) {
	int pronounIndex;

	for (pronounIndex = 0; !isEndOfList(&pronounList[pronounIndex]); pronounIndex++)
		;
	pronounList[pronounIndex].pronoun = pronoun;
	pronounList[pronounIndex].instance = instanceCode;
	setEndOfList(&pronounList[pronounIndex + 1]);
}

/*----------------------------------------------------------------------*/
static void notePronounParameters(ParamEntry *parameters) {
	/* For all parameters note which ones can be referred to by a pronoun */
	ParamEntry *p;

	clearList(pronounList);
	for (p = parameters; !isEndOfList(p); p++) {
		int pronoun = pronounForInstance(p->instance);
		if (pronoun > 0)
			enterPronoun(pronoun, p->instance);
	}
}


/*======================================================================*/
void parse(void) {
	ParamEntry *multipleMatches = createParameterList(NULL); /* Multiple objects list */

	if (isEndOfList(&playerWords[wordIndex])) {
		wordIndex = 0;
		scan();
	} else if (anyOutput)
		para();

	capitalize = TRUE;
	allLength = 0;
	paramidx = 0;
	clearList(parameters);

	firstWord = wordIndex;
	if (isVerb(playerWords[wordIndex].code)) {
		verbWord = playerWords[wordIndex].code;
		verbWordCode = dictionary[verbWord].code;
		wordIndex++;
		match(multipleMatches);
		notePronounParameters(parameters);
		action(parameters, multipleMatches);
	} else {
		clearList(previousMultipleMatches);
		clearList(pronounList);
		nonverb();
	}
	lastWord = wordIndex - 1;
	if (isConj(playerWords[lastWord].code))
		lastWord--;

	copyParameterList(previousMultipleMatches, multipleMatches);
	free(multipleMatches);
}

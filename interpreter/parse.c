/*----------------------------------------------------------------------*\

  parse.c

  Command line parser unit for Alan interpreter ARUN

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>

#include "types.h"

#ifdef USE_READLINE
#include "readline.h"
#endif

#include "main.h"
#include "inter.h"
#include "exe.h"
#include "act.h"
#include "term.h"
#include "debug.h"
#include "params.h"
#include "syserr.h"
#include "parse.h"

#ifdef HAVE_GLK
#include "glkio.h"
#endif


#define LISTLEN 100
/* PUBLIC DATA */

int playerWords[LISTLEN/2] = {EOF}; /* List of parsed words, index
				       into dictionary */
int wordIndex;			/* and an index into it the list */

Bool plural = FALSE;


/* Syntax Parameters */
int paramidx;			/* Index in params */
ParamEntry *parameters;		/* List of params */
static ParamEntry *previousParameters;	/* Previous parameter list */
static ParamEntry *previousMultipleList;	/* Previous multiple list */

/* Literals */
LiteralEntry literal[MAXPARAMS+1];
int litCount;

/* What did the user say? */
int verbWord;			/* The word he used, dictionary index */
int verbWordCode;		/* The code for that verb */



/*----------------------------------------------------------------------*\

  SCAN DATA & PROCEDURES

  All procedures for getting a command and turning it into a list of
  dictionary entries are placed here.

\*----------------------------------------------------------------------*/

/* PRIVATE DATA */

static char buf[LISTLEN+1];	/* The input buffer */
static char isobuf[LISTLEN+1];	/* The input buffer in ISO */
static Bool eol = TRUE;		/* Looking at End of line? Yes, initially */
static char *token;



/*----------------------------------------------------------------------*/
static void unknown(char token[]) {
  char *str = allocate((int)strlen(token)+4);

  sprintf(str, "'%s'?", token);
#if ISO == 0
  fromIso(str, str);
#endif
  output(str);
  free(str);
  eol = TRUE;
  error(M_UNKNOWN_WORD);
}


/*----------------------------------------------------------------------*/
static int lookup(char wrd[]) {
  int i;

  for (i = 0; !endOfTable(&dictionary[i]); i++) {
    if (compareStrings(wrd, (char *) pointerTo(dictionary[i].wrd)) == 0)
      return (i);
  }
  unknown(wrd);
  return(EOF);
}


/*----------------------------------------------------------------------*/
static int number(char token[]) {
  int i;

  sscanf(token, "%d", &i);
  return i;
}


/*----------------------------------------------------------------------*/
static Bool isWordCharacter(int ch) {
  return isISOLetter(ch)||isdigit(ch)||ch=='\''||ch=='-'||ch=='_';
}


/*----------------------------------------------------------------------*/
static char *gettoken(char *buf) {
  static char *marker;
  static char oldch;

  if (buf == NULL)
    *marker = oldch;
  else
    marker = buf;
  while (*marker != '\0' && isSpace(*marker) && *marker != '\n') marker++;
  buf = marker;
  if (isISOLetter(*marker))
    while (*marker&&isWordCharacter(*marker)) marker++;
  else if (isdigit(*marker))
    while (isdigit(*marker)) marker++;
  else if (*marker == '\"') {
    marker++;
    while (*marker != '\"') marker++;
    marker++;
  } else if (*marker == '\0' || *marker == '\n')
    return NULL;
  else
    marker++;
  oldch = *marker;
  *marker = '\0';
  return buf;
}

/*----------------------------------------------------------------------*/
static void getline(void)
{
  para();
  do {
#if defined(HAVE_ANSI) || defined(HAVE_GLK)
    statusline();
#endif
    logPrint("> ");
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
      playerWords[0] = EOF;
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
	undo();
	playerWords[wordIndex] = EOF;		/* Force new player input */
	longjmp(errorLabel, TRUE);
      }
    }
  } while (token == NULL);
  eol = FALSE;
}


/*----------------------------------------------------------------------*/
static void scan(void)
{
  int i;
  int w;
  char *str;
  static Bool continued = FALSE;

  if (continued) {
    /* Player used '.' to separate commands. Read next */
    para();
    token = gettoken(NULL);
    if (token == NULL)
      getline();
    continued = FALSE;
  } else
    getline();

  playerWords[0] = 0;
  for (i = 0; i < litCount; i++)
    if (literal[i].type == STRING_LITERAL && literal[i].value != 0)
      free((char *) literal[i].value);
  i = 0;
  litCount = 0;
  do {
    if (isISOLetter(token[0])) {
      /*      (void) stringLower(token); */
      w = lookup(token);
      if (!isNoise(w))
	playerWords[i++] = w;
    } else if (isdigit(token[0]) || token[0] == '\"') {
      litCount++;
      if (litCount > MAXPARAMS)
	syserr("Too many parameters.");
      playerWords[i++] = dictsize+litCount; /* Word outside dictionary = literal */
      if (isdigit(token[0])) {
	literal[litCount].class = header->integerClassId;
	literal[litCount].type = NUMERIC_LITERAL;
	literal[litCount].value = number(token);
      } else {
	literal[litCount].class = header->stringClassId;
	literal[litCount].type = STRING_LITERAL;
	/* Remove the string quotes while copying */
	str = strdup(&token[1]);
	str[strlen(token)-2] = '\0';
	literal[litCount].value = (Aword) str;
      }
    } else if (token[0] == ',') {
      playerWords[i++] = conjWord;
    } else if (token[0] == '.') {
      continued = TRUE;
      playerWords[i] = EOF;
      eol = TRUE;
      break;
    } else
      unknown(token);
    playerWords[i] = EOF;
    eol = (token = gettoken(NULL)) == NULL;
  } while (!eol);
}



/*----------------------------------------------------------------------*\

  PARSE DATA & PROCEDURES

  All procedures and data for getting a command and parsing it

\*---------------------------------------------------------------------- */


/* Private Types */

typedef struct PronounEntry {	/* To remember parameter/pronoun relations */
  int pronoun;
  int instance;
} PronounEntry;

static PronounEntry *pronounList = NULL;
static int allWord;		/* Word index of the ALL_WORD found */
static int allLength;		/* No. of objects matching 'all' */


/*----------------------------------------------------------------------*/
static int noOfPronouns()
{
  int w;
  int count = 0;

  for (w = 0; w < dictsize; w++)
    if (isPronoun(w))
      count++;
  return count;
}


/*----------------------------------------------------------------------*/
static void nonverb(void) {
  if (isDir(playerWords[wordIndex])) {
    wordIndex++;
    if (playerWords[wordIndex] != EOF && !isConj(playerWords[wordIndex]))
      error(M_WHAT);
    else
      go(dictionary[playerWords[wordIndex-1]].code);
    if (playerWords[wordIndex] != EOF)
      wordIndex++;
  } else
    error(M_WHAT);
}


/*----------------------------------------------------------------------*/
static void errorWhich(ParamEntry alternative[]) {
  int p;			/* Index into the list of alternatives */

  printMessage(M_WHICH_ONE_START);
  sayForm(alternative[0].code, SAY_DEFINITE);
  for (p = 1; !endOfTable(&alternative[p+1]); p++) {
    printMessage(M_WHICH_ONE_COMMA);
    sayForm(alternative[p].code, SAY_DEFINITE);
  }
  printMessage(M_WHICH_ONE_OR);
  sayForm(alternative[p].code, SAY_DEFINITE);
  printMessage(M_WHICH_ONE_END);
  error(MSGMAX);		/* Return with empty error message */
}


/*----------------------------------------------------------------------*/
static void errorWhat(int word) {
  char quotedWord[100];

  printMessage(M_WHAT_START);
  sprintf(quotedWord, "'%s'", (char*)pointerTo(dictionary[word].wrd));
  output(quotedWord);
  printMessage(M_WHAT_END);
  error(MSGMAX);		/* Return with empty error message */
}


/*----------------------------------------------------------------------*/
static void errorNoSuch(ParamEntry parameter) {
#ifdef NEW
  int w;

  printMessage(M_NO_SUCH_START);
  for (w = parameter.firstWord; w <= parameter.lastWord; w++) {
    output((char*)pointerTo(dictionary[playerWords[w]].wrd));
  }
  printMessage(M_NO_SUCH_END);
#else
  parameters[0] = parameter;
  parameters[0].code = 0;	/* Indicate to use words and not names */
  parameters[1].code = EOF;
  printMessage(M_NO_SUCH);
#endif
  error(MSGMAX);		/* Return with empty error message */
}


/*----------------------------------------------------------------------*/
static void buildAll(ParamEntry list[]) {
  int o, i = 0;
  Bool found = FALSE;
  
  for (o = 1; o <= header->instanceMax; o++)
    if (isHere(o, FALSE)) {
      found = TRUE;
      list[i].code = o;
      list[i++].firstWord = EOF;
    }
  if (!found)
    errorWhat(playerWords[wordIndex]);
  else
    list[i].code = EOF;
  allWord = playerWords[wordIndex];
}


/*----------------------------------------------------------------------*/
static int getPronounInstance(int word) {
  /* Find the instance that the pronoun word could refer to, return 0
     if none or multiple */
  int p;

  for (p = 0; p < noOfPronouns(); p++)
    if (dictionary[word].code == pronounList[p].pronoun)
      return pronounList[p].instance;
  return 0;
}


/*----------------------------------------------------------------------*/
static Bool inOpaqueContainer(int originalInstance)
{
  int instance = admin[originalInstance].location;

  while (isContainer(instance)) {
    if (attributeOf(instance, OPAQUEATTRIBUTE))
      return TRUE;
    instance = admin[instance].location;
  }
  return FALSE;
}


/*----------------------------------------------------------------------*/
static Bool reachable(int instance)
{
  if (isA(instance, THING))
    return isHere(instance, FALSE) && !inOpaqueContainer(instance);
  else
    return TRUE;
}
    
	
/*----------------------------------------------------------------------*/
static void resolve(ParamEntry plst[])
{
  /*
    In case the syntax did not indicate omnipotent powers (allowed
    access to remote object), we need to remove non-present
    parameters
  */

  int i;

  if (allLength > 0) return;	/* ALL has already done this */

  /* Resolve ambiguities by presence */
  for (i=0; plst[i].code != EOF; i++) {
    if (isLiteral(plst[i].code))	/* Literals are always 'here' */
      continue;
    if (instance[plst[i].code].parent == header->entityClassId)
      /* and so are pure entities */
      continue;
    if (!reachable(plst[i].code)) {
      errorNoSuch(plst[i]);
    }
  }
}


/*----------------------------------------------------------------------*/
static void unambig(ParamEntry plst[])
{
  int i;
  Bool foundNoun = FALSE;	/* Adjective or noun found ? */
  static ParamEntry *refs;	/* Entities referenced by word */
  static ParamEntry *savlst;	/* Saved list for backup at EOF */
  int firstWord, lastWord;	/* The words the player used */

  if (refs == NULL)
    refs = (ParamEntry *)allocate((MAXENTITY+1)*sizeof(ParamEntry));

  if (savlst == NULL)
    savlst = (ParamEntry *)allocate((MAXENTITY+1)*sizeof(ParamEntry));

  if (isLiteralWord(playerWords[wordIndex])) {
    /* Transform the word into a reference to the literal value */
    /* words > dictsize are literals with index = word-dictsize */
    plst[0].code = (playerWords[wordIndex]-dictsize) + header->instanceMax;
    plst[0].firstWord = EOF;	/* No words used! */
    plst[1].code = EOF;
    wordIndex++;
    return;
  }

  plst[0].code = EOF;		/* Make an empty parameter list */
  if (isPronoun(playerWords[wordIndex])) {
    int p = getPronounInstance(playerWords[wordIndex]);
    if (p == 0)
      errorWhat(playerWords[wordIndex]);
    wordIndex++;		/* Consume the pronoun */
    plst[0].code = p;
    plst[0].firstWord = EOF;	/* No words used! */
    plst[1].code = EOF;
    return;
  }

  firstWord = wordIndex;
  while (playerWords[wordIndex] != EOF && isAdjective(playerWords[wordIndex])) {
    /* If this word can be a noun and there is no noun following break loop */
    if (isNoun(playerWords[wordIndex]) && (playerWords[wordIndex+1] == EOF || !isNoun(playerWords[wordIndex+1])))
      break;
    copyReferences(refs, (Aword *)pointerTo(dictionary[playerWords[wordIndex]].adjectiveRefs));
    copyParameterList(savlst, plst);	/* To save it for backtracking */
    if (foundNoun)
      intersect(plst, refs);
    else {
      copyParameterList(plst, refs);
      foundNoun = TRUE;
    }
    wordIndex++;
  }
  if (playerWords[wordIndex] != EOF) {
    if (isNoun(playerWords[wordIndex])) {
      copyReferences(refs, (Aword *)pointerTo(dictionary[playerWords[wordIndex]].nounRefs));
      if (foundNoun)
	intersect(plst, refs);
      else {
	copyParameterList(plst, refs);
	foundNoun = TRUE;
      }
      wordIndex++;
    } else
      error(M_NOUN);
  } else if (foundNoun) {
    if (isNoun(playerWords[wordIndex-1])) {
      /* Perhaps the last word was also a noun? */
      copyParameterList(plst, savlst);	/* Restore to before last adjective */
      copyReferences(refs, (Aword *)pointerTo(dictionary[playerWords[wordIndex-1]].nounRefs));
      if (plst[0].code == EOF)
	copyParameterList(plst, refs);
      else
	intersect(plst, refs);
    } else
      error(M_NOUN);
  }
  lastWord = wordIndex-1;

  /* Allow remote objects, but resolve ambiguities by reachability */
  if (listLength(plst) > 1) {
    for (i=0; plst[i].code != EOF; i++)
      if (!reachable(plst[i].code))
	plst[i].code = 0;
    compress(plst);
  }
    
  if (listLength(plst) > 1 || (foundNoun && listLength(plst) == 0)) {
    parameters[0].code = 0;		/* Just make it anything != EOF */
    parameters[0].firstWord = firstWord; /* Remember words for errors below */
    parameters[0].lastWord = lastWord;
    parameters[1].code = EOF;	/* But be sure to terminate */
    if (listLength(plst) > 1)
      errorWhich(plst);
    else if (foundNoun && listLength(plst) == 0)
      errorNoSuch(parameters[0]);
  } else {
    plst[0].firstWord = firstWord;
    plst[0].lastWord = lastWord;
  }
}
  
  
/*----------------------------------------------------------------------*/
static void simple(ParamEntry olst[]) {
  static ParamEntry *tlst = NULL;
  int savidx = wordIndex;
  Bool savplur = FALSE;
  int i;

  if (tlst == NULL)
    tlst = (ParamEntry *) allocate(sizeof(ParamEntry)*(MAXENTITY+1));
  tlst[0].code = EOF;

  for (;;) {
    /* Special handling here since THEM_WORD is a common pronoun, so
       we check if it is also a pronoun, if it is but there is a list of
       multi parameters from the previous command, we assume that is
       what he ment */
    if (isThem(playerWords[wordIndex])
	&& ((isPronoun(playerWords[wordIndex]) && listLength(previousMultipleList) > 0)
	    || !isPronoun(playerWords[wordIndex]))) {
      plural = TRUE;
      for (i = 0; previousMultipleList[i].code != EOF; i++)
	if (!reachable(previousMultipleList[i].code))
	  previousMultipleList[i].code = 0;
      compress(previousMultipleList);
      if (listLength(previousMultipleList) == 0)
	errorWhat(playerWords[wordIndex]);
      copyParameterList(olst, previousMultipleList);
      olst[0].firstWord = EOF;	/* No words used */
      wordIndex++;
    } else {
      unambig(olst);		/* Look for unambigous noun phrase */
      if (listLength(olst) == 0) {	/* Failed! */
	copyParameterList(olst, tlst);
	wordIndex = savidx;
	plural = savplur;
	return;
      }
    }
    mergeLists(tlst, olst);
    if (playerWords[wordIndex] != EOF
	&& (isConj(playerWords[wordIndex]) &&
	    (isAdjective(playerWords[wordIndex+1]) || isNoun(playerWords[wordIndex+1])))) {
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
  
  
/*----------------------------------------------------------------------

  complex()

  Above this procedure we can use the is* tests, but not below since
  they work on words. Below all is converted to indices into the
  entity tables. Particularly this goes for literals...

*/
static void complex(ParamEntry olst[])
{
  static ParamEntry *alst = NULL;

  if (alst == NULL)
    alst = (ParamEntry *) allocate((MAXENTITY+1)*sizeof(ParamEntry));

  if (isAll(playerWords[wordIndex])) {
    plural = TRUE;
    buildAll(alst);		/* Build list of all objects */
    wordIndex++;
    if (playerWords[wordIndex] != EOF && isBut(playerWords[wordIndex])) {
      wordIndex++;
      simple(olst);
      if (listLength(olst) == 0)
	error(M_AFTER_BUT);
      subtractListFromList(alst, olst);
      if (listLength(alst) == 0)
	error(M_NOT_MUCH);
    }
    copyParameterList(olst, alst);
    allLength = listLength(olst);
  } else
    simple(olst);		/* Look for simple noun group */
}


/*----------------------------------------------------------------------*/
static Bool restrictionCheck(RestrictionEntry *restriction)
{
  Bool ok = FALSE;

  if (restriction->class == RESTRICTIONCLASS_CONTAINER)
    ok = instance[parameters[restriction->parameter-1].code].container != 0;
  else
    ok = isA(parameters[restriction->parameter-1].code, restriction->class);
  return ok;
}


/*----------------------------------------------------------------------*/
static void runRestriction(RestrictionEntry *restriction)
{
  if (sectionTraceOption)
    printf("\n<SYNTAX parameter #%ld Is Not of class %ld:>\n",
	   restriction->parameter,
	   restriction->class);
  if (restriction->stms)
    interpret(restriction->stms);
  else
    error(M_CANT0);
}


/*----------------------------------------------------------------------*/
static Aint mapSyntax(Aint syntaxNumber)
{
  /* 
     Find the syntax map, use the verb code from it and remap the parameters
   */
  SyntaxEntry *syntax;
  Aword *parameterMap;
  Aint parameterIndex;
  ParamEntry originalParameters[MAXPARAMS];

  for (syntax = pointerTo(header->syntaxTableAddress); !endOfTable(syntax); syntax++)
    if (syntax->syntaxNumber == syntaxNumber)
      break;
  if (endOfTable(syntax)) syserr("Could not find syntax in mapping table.");

  parameterMap = pointerTo(syntax->parameterMapping);
  copyParameterList(originalParameters, parameters);
  for (parameterIndex = 1; originalParameters[parameterIndex-1].code != EOF;
       parameterIndex++)
    parameters[parameterIndex-1] = originalParameters[parameterMap[parameterIndex-1]-1];

  return syntax->verbCode;
}


/*----------------------------------------------------------------------*/
static void parseParameter(Aword flags, Bool *anyPlural, ParamEntry mlst[]) {

  plural = FALSE;
  complex(mlst);
  if (listLength(mlst) == 0) /* No object!? */
    error(M_WHAT);
  if ((flags & OMNIBIT) == 0) /* Omnipotent parameter? */
    /* If its not an omnipotent parameter, resolve by presence */
    resolve(mlst);
  if (plural) {
    if ((flags & MULTIPLEBIT) == 0)	/* Allowed multiple? */
      error(M_MULTIPLE);
    else {
      /*
	Mark this as the multiple position in which to insert
	actual parameter values later
      */
      parameters[paramidx++].code = 0;
      *anyPlural = TRUE;
    }
  } else
    parameters[paramidx++] = mlst[0];
  parameters[paramidx].code = EOF;
}


/*----------------------------------------------------------------------*/
static ElementEntry *matchParameterElement(ElementEntry *elms) {
  /* Require a parameter if elms->code == 0! */
  while (!endOfTable(elms) && elms->code != 0)
    elms++;
  if (endOfTable(elms))
    return NULL;
  return(elms);
}


/*----------------------------------------------------------------------*/
static ElementEntry *matchEndOfSyntax(ElementEntry *elms) {
  while (!endOfTable(elms) && elms->code != EOS)
    elms++;
  if (endOfTable(elms))		/* No match for EOS! */
    return NULL;
  return(elms);
}

/*----------------------------------------------------------------------*/
static ElementEntry *matchWordElement(ElementEntry *elms, Aint wordCode) {
  while (!endOfTable(elms) && elms->code != wordCode)
    elms++;
  if (endOfTable(elms))
    return NULL;
  return(elms);
}

/*----------------------------------------------------------------------*/
static Bool isParameterWord(int word) {
  return isNoun(word) || isAdjective(word) || isAll(word)
    || isLiteralWord(word) || isIt(word) || isThem(word) || isPronoun(word);
}


/*----------------------------------------------------------------------*/
static ElementEntry *matchParseTree(ParamEntry multipleList[],
				    ElementEntry *elms,
				    Bool *anyPlural) {
  ElementEntry *currentEntry = elms;

  while (TRUE) {		/* Traverse the possible branches to
				   find a match */
    ElementEntry *elms;

    /* End of input? */
    if (playerWords[wordIndex] == EOF || isConj(playerWords[wordIndex])) {
      elms = matchEndOfSyntax(currentEntry);
      if (elms == NULL)
	return NULL;
      else
	return elms;
    }

    if (isParameterWord(playerWords[wordIndex])) {
      elms = matchParameterElement(currentEntry);
      if (elms != NULL) {
	parseParameter(elms->flags, anyPlural, multipleList);
	currentEntry = (ElementEntry *) pointerTo(elms->next);
	continue;
      } /* didn't allow a parameter so fall through
	   and try with a preposition... */
    }

    if (isPreposition(playerWords[wordIndex])) {
      /* A preposition? Or rather an intermediate word? */
      elms = matchWordElement(currentEntry, dictionary[playerWords[wordIndex]].code);
      if (elms != NULL) {
	wordIndex++;		/* Word matched, go to next */
	currentEntry = (ElementEntry *) pointerTo(elms->next);
	continue;
      }
    }

    /* If we get here we couldn't match anything... */
    return NULL;
  }
  /* And this should never happen ... */
  syserr("Fall-through");
  return NULL;
}


/*----------------------------------------------------------------------*/
static ParseEntry *findSyntax(int verbCode) {
  ParseEntry *stx;
  for (stx = stxs; !endOfTable(stx); stx++)
    if (stx->code == verbCode)
      return(stx);
  return(NULL);
}


/*----------------------------------------------------------------------*/
static void try(ParamEntry multipleParameters[])
{
  ElementEntry *elms;		/* Pointer to element list */
  ParseEntry *stx;		/* Pointer to syntax parse list */
  RestrictionEntry *restriction; /* Pointer to class restrictions */
  Bool anyPlural = FALSE;	/* Any parameter that was plural? */
  int i, p;
  static ParamEntry *tlst = NULL; /* List of params found by complex() */
  static Bool *checked = NULL; /* Corresponding parameter checked? */

  if (tlst == NULL) {
    tlst = (ParamEntry *) allocate((MAXENTITY+1)*sizeof(ParamEntry));
    checked = (Bool *) allocate((MAXENTITY+1)*sizeof(Bool));
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
  if (elms->next == 0)	/* No verb code, verb not declared! */
    error(M_CANT0);

  for (p = 0; parameters[p].code != EOF; p++) /* Mark all parameters unchecked */
    checked[p] = FALSE;
  for (restriction = (RestrictionEntry *) pointerTo(elms->next); !endOfTable(restriction); restriction++) {
    if (parameters[restriction->parameter-1].code == 0) {
      /* This was a multiple parameter, so check all and remove failing */
      for (i = 0; multipleParameters[i].code != EOF; i++) {
	parameters[restriction->parameter-1] = multipleParameters[i];
	if (!restrictionCheck(restriction)) {
	  /* Multiple could be both an explicit list of params and an ALL */
	  if (allLength == 0) {
	    char marker[80];
	    /*
	       It wasn't ALL, we need to say something about it, so
	       prepare a printout with $1/2/3
	     */
	    sprintf(marker, "($%ld)", restriction->parameter); 
	    output(marker);
	    runRestriction(restriction);
	    para();
	  }
	  multipleParameters[i].code = 0;	  /* In any case remove it from the list */
	}
      }
      parameters[restriction->parameter-1].code = 0;
    } else {
      if (!restrictionCheck(restriction)) {
	runRestriction(restriction);
	error(MSGMAX);		/* Return to player without saying anything */
      }
    }
    checked[restriction->parameter-1] = TRUE; /* Remember that it's already checked */
  }
  /* Now check the rest of the parameters, must be objects */
  for (p = 0; parameters[p].code != EOF; p++)
    if (!checked[p]) {
      if (parameters[p].code == 0) {
	/* This was a multiple parameter, check all and remove failing */
	for (i = 0; multipleParameters[i].code != EOF; i++)
	  if (multipleParameters[i].code != 0) /* Skip any empty slots */
	    if (!isObj(multipleParameters[i].code))
	      multipleParameters[i].code = 0;
      } else if (!isObj(parameters[p].code))
	error(M_CANT0);
    }

  /* Finally, if ALL was used, try to find out what was applicable */
  if (allLength > 0) {
    for (p = 0; parameters[p].code != 0; p++); /* Find multiple marker */
    for (i = 0; i < allLength; i++) {
      if (multipleParameters[i].code != 0) {	/* Already empty? */
	parameters[p] = multipleParameters[i];
	if (!possible())
	  multipleParameters[i].code = 0;	/* Remove this from list */
      }
    }
    parameters[p].code = 0;		/* Restore multiple marker */
    compress(multipleParameters);
    if (listLength(multipleParameters) == 0) {
      parameters[0].code = EOF;
      errorWhat(allWord);
    }
  } else if (anyPlural) {
    compress(multipleParameters);
    if (listLength(multipleParameters) == 0)
      /* If there where multiple parameters but non left, exit without a */
      /* word, assuming we have already said enough */
      error(MSGMAX);
  }
  plural = anyPlural;		/* Remember that we found plural objects */
}


/*----------------------------------------------------------------------*/  
static void match(ParamEntry *mlst) /* OUT - List of params allowed by multiple */
{
  try(mlst);			/* ... to understand what he said */
  if (playerWords[wordIndex] != EOF && !isConj(playerWords[wordIndex]))
    error(M_WHAT);
  if (playerWords[wordIndex] != EOF)	/* More on this line? */
    wordIndex++;			/* If so skip the AND */
}


/*======================================================================*/
void initParse(void) {
  int dictionaryIndex;
  int pronounIndex = 0;

  wordIndex = 0;
  playerWords[0] = EOF;

  if (pronounList == NULL)
    pronounList = allocate(noOfPronouns()*sizeof(PronounEntry));

  for (dictionaryIndex = 0; dictionaryIndex < dictsize; dictionaryIndex++)
    if (isPronoun(dictionaryIndex)) {
      pronounList[pronounIndex].pronoun = dictionary[dictionaryIndex].code;
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

  for (pronounIndex = 0; pronounIndex < noOfPronouns(); pronounIndex++)
    if (pronounList[pronounIndex].pronoun == pronoun)
      pronounList[pronounIndex].instance = instanceCode;
}


/*----------------------------------------------------------------------*/
static void clearPronounEntries() {
  int i;

  for (i = 0; i < noOfPronouns(); i++)
    pronounList[i].instance = 0;
}


/*----------------------------------------------------------------------*/
static void notePronounParameters(ParamEntry *parameters) {
  /* For all parameters note which ones can be refered to by a pronoun */
  ParamEntry *p;

  clearPronounEntries();
  for (p = parameters; !endOfTable(p); p++) {
    int pronoun = pronounForInstance(p->code);
    if (pronoun > 0)
      enterPronoun(pronoun, p->code);
  }
}


/*======================================================================*/
void parse(void) {
  static ParamEntry *multipleList;	/* Multiple objects list */

  if (multipleList == NULL) {		/* Allocate large enough paramlists */
    multipleList = (ParamEntry *) allocate(sizeof(ParamEntry)*(MAXENTITY+1));
    multipleList[0].code = EOF;
    previousMultipleList = (ParamEntry *) allocate(sizeof(ParamEntry)*(MAXENTITY+1));
    parameters = (ParamEntry *) allocate(sizeof(ParamEntry)*(MAXENTITY+1));
    parameters[0].code = EOF;
    previousParameters = (ParamEntry *) allocate(sizeof(ParamEntry)*(MAXENTITY+1));
    previousParameters[0].code = EOF;
  }

  if (playerWords[wordIndex] == EOF) {
    wordIndex = 0;
    scan();
  } else if (anyOutput)
    para();

  capitalize = TRUE;
  allLength = 0;
  paramidx = 0;
  copyParameterList(previousParameters, parameters);
  parameters[0].code = EOF;
  copyParameterList(previousMultipleList, multipleList);
  multipleList[0].code = EOF;
  if (isVerb(playerWords[wordIndex])) {
    verbWord = playerWords[wordIndex];
    verbWordCode = dictionary[verbWord].code;
    wordIndex++;
    match(multipleList);
    action(multipleList);		/* contains possible multiple params */
    notePronounParameters(parameters);
  } else {
    parameters[0].code = EOF;
    previousMultipleList[0].code = EOF;
    nonverb();
  }
}


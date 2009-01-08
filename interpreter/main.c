/*----------------------------------------------------------------------*\

  MAIN.C

  Main module of interpreter for ALAN Adventure Language

\*----------------------------------------------------------------------*/

#include "sysdep.h"

#include "acode.h"
#include "types.h"
#include "set.h"
#include "state.h"
#include "lists.h"
#include "main.h"
#include "syserr.h"
#include "parse.h"
#include "params.h"
#include "options.h"
#include "utils.h"

#include <time.h>
#ifdef USE_READLINE
#include "readline.h"
#endif

#include "alan.version.h"

#include "args.h"
#include "parse.h"
#include "inter.h"
#include "rules.h"
#include "reverse.h"
#include "debug.h"
#include "stack.h"
#include "exe.h"
#include "term.h"
#include "set.h"
#include "instance.h"

#ifdef HAVE_GLK
#include "glk.h"
#include "glkio.h"
#ifdef HAVE_WINGLK
#include "winglk.h"
#endif
#endif

/* PUBLIC DATA */

/* The Amachine memory */
Aword *memory = NULL;
static ACodeHeader dummyHeader;	/* Dummy to use until memory allocated */
ACodeHeader *header = &dummyHeader;

int memTop = 0;			/* Top of load memory */

int conjWord;			/* First conjunction in dictonary, for ',' */


/* Amachine variables */
CurVars current;

/* The event queue */
int eventQueueSize = 0;
EventQueueEntry *eventQueue = NULL; /* Event queue */
Aint eventQueueTop = 0;		/* Event queue top pointer */

/* Amachine structures - Dynamic */
Aword *scores;			/* Score table pointer */

/* Amachine structures - Static */
ContainerEntry *container;	/* Container table pointer */
ClassEntry *classes;		/* Class table pointer */
DictionaryEntry *dictionary;	/* Dictionary pointer */
VerbEntry *vrbs;		/* Verb table pointer */
SyntaxEntry *stxs;		/* Syntax table pointer */
RulEntry *ruls;			/* Rule table pointer */
EventEntry *events;		/* Event table pointer */
MessageEntry *msgs;			/* Message table pointer */
Aword *freq;			/* Cumulative character frequencies */

int dictionarySize;

Bool fail = FALSE;
Bool anyOutput = FALSE;


/* The files and filenames */
char *adventureName;		/* The name of the game */
char *adventureFileName;
FILE *textFile;
#ifdef HAVE_GLK
strid_t logFile;
#else
FILE *logFile;
#endif

/* Screen formatting info */
int col, lin;
int pageLength, pageWidth;

Bool capitalize = FALSE;
Bool needSpace = FALSE;
Bool skipSpace = FALSE;

/* Restart jump buffer */
jmp_buf restartLabel;		/* Restart long jump return point */
jmp_buf returnLabel;		/* Error (or undo) long jump return point */
jmp_buf forfeitLabel;		/* Player forfeit by an empty command */


/* PRIVATE DATA */
static Bool onStatusLine = FALSE; /* Don't log when printing status */
#define STACKSIZE 100


/*======================================================================*/
void error(MsgKind msgno)	/* IN - The error message number */
{
  /* Print an error message and longjmp to main loop. */
  if (msgno != NO_MSG)
    printMessage(msgno);
  longjmp(returnLabel, ERROR_RETURN);
}


#if defined(HAVE_GLK) || defined(RUNNING_UNITTESTS)
/*----------------------------------------------------------------------*/
static int updateColumn(int currentColumn, char *string) {
  char *newlinePosition = strrchr(string, '\n');
  if (newlinePosition != NULL)
    return &string[strlen(string)] - newlinePosition;
  else
    return currentColumn + strlen(string);
}
#endif


/*======================================================================*/
void printAndLog(char string[])
{
#ifdef HAVE_GLK
  static int column = 0;
  char *stringCopy;
  char *stringPart;
#endif

  printf(string);
  if (!onStatusLine && transcriptOption) {
#ifdef HAVE_GLK
    if (strlen(string) > 70-column) {
      stringCopy = strdup(string);	/* Make sure we can write NULLs */
      stringPart = stringCopy;
      while (strlen(stringPart) > 70-column) {
	int p;
	for (p = 70-column; p>0 && !isspace(stringPart[p]); p--);
	stringPart[p] = '\0';
	glk_put_string_stream(logFile, stringPart);
	glk_put_char_stream(logFile, '\n');
	column = 0;
	stringPart = &stringPart[p+1];
      }
      glk_put_string_stream(logFile, stringPart);
      column = updateColumn(column, stringPart);
      free(stringCopy);
    } else {
      glk_put_string_stream(logFile, string);
      column = updateColumn(column, string);
    }
#else
    fprintf(logFile, string);
#endif
  }
}



/*======================================================================*/
void newline(void)
{
#ifndef HAVE_GLK
  char buf[256];

  if (!regressionTestOption && lin >= header->pageLength - 1) {
    printAndLog("\n");
    needSpace = FALSE;
    printMessage(M_MORE);
    statusline();
    fflush(stdout);
    fgets(buf, 256, stdin);
    getPageSize();
    lin = 0;
  } else
    printAndLog("\n");

  lin++;
#else
  printAndLog("\n");
#endif
  col = 1;
  needSpace = FALSE;
}


/*======================================================================*/
void para(void)
{
  /* Make a new paragraph, i.e one empty line (one or two newlines). */

#ifdef HAVE_GLK
  if (glk_gestalt(gestalt_Graphics, 0) == 1)
    glk_window_flow_break(glkMainWin);
#endif
  if (col != 1)
    newline();
  newline();
  capitalize = TRUE;
}


/*======================================================================*/
void clear(void)
{
#ifdef HAVE_GLK
  glk_window_clear(glkMainWin);
#else
#ifdef HAVE_ANSI
  if (!statusLineOption) return;
  printf("\x1b[2J");
  printf("\x1b[%d;1H", pageLength);
#endif
#endif
}


#ifndef SMARTALLOC
/*======================================================================*/
void *allocate(unsigned long lengthInBytes)
{
  void *p = (void *)calloc((size_t)lengthInBytes, 1);

  if (p == NULL)
    syserr("Out of memory.");

  return p;
}
#endif


/*======================================================================*/
void *duplicate(void *original, unsigned long len)
{
  void *p = allocate(len+1);

  memcpy(p, original, len);
  return p;
}


/*----------------------------------------------------------------------*/
static void capitalizeFirst(char *str) {
  int i = 0;

  /* Skip over space... */
  while (i < strlen(str) && isSpace(str[i])) i++;
  if (i < strlen(str)) {
    str[i] = toUpper(str[i]);
    capitalize = FALSE;
  }
}


/*----------------------------------------------------------------------*/
static void justify(char str[])
{
  if (capitalize)
    capitalizeFirst(str);

#ifdef HAVE_GLK
  printAndLog(str);
#else
  int i;
  char ch;

  if (col >= pageWidth && !skipSpace)
    newline();

  while (strlen(str) > pageWidth - col) {
    i = pageWidth - col - 1;
    while (!isSpace(str[i]) && i > 0) /* First find wrap point */
      i--;
    if (i == 0 && col == 1)	/* If it doesn't fit at all */
      /* Wrap immediately after this word */
      while (!isSpace(str[i]) && str[i] != '\0')
	i++;
    if (i > 0) {		/* If it fits ... */
      ch = str[i];		/* Save space or NULL */
      str[i] = '\0';		/* Terminate string */
      printAndLog(str);		/* and print it */
      skipSpace = FALSE;		/* If skipping, now we're done */
      str[i] = ch;		/* Restore character */
      /* Skip white after printed portion */
      for (str = &str[i]; isSpace(str[0]) && str[0] != '\0'; str++);
    }
    newline();			/* Then start a new line */
    while(isSpace(str[0])) str++; /* Skip any leading space on next part */
  }
  printAndLog(str);		/* Print tail */
#endif
  col = col + strlen(str);	/* Update column */
}


/*----------------------------------------------------------------------*/
static void space(void)
{
  if (skipSpace)
    skipSpace = FALSE;
  else {
    if (needSpace) {
      printAndLog(" ");
      col++;
    }
  }
  needSpace = FALSE;
}


/*----------------------------------------------------------------------*/
static void sayPlayerWordsForParameter(int p) {
  int i;

  for (i = parameters[p].firstWord; i <= parameters[p].lastWord; i++) {
    justify((char *)pointerTo(dictionary[playerWords[i].code].string));
    if (i < parameters[p].lastWord)
      justify(" ");
  }
}


/*----------------------------------------------------------------------*/
static void sayParameter(int p, int form)
{
  int i;

  for (i = 0; i <= p; i++)
    if (isEndOfList(&parameters[i]))
      syserr("Nonexistent parameter referenced.");

#ifdef ALWAYS_SAY_PARAMETERS_USING_PLAYER_WORDS
  if (params[p].firstWord != EOF) /* Any words he used? */
    /* Yes, so use them... */
    sayPlayerWordsForParameter(p);
  else
    sayForm(params[p].code, form);
#else
  if (parameters[p].useWords) {
    /* Ambiguous instance referenced, so use the words he used */
    sayPlayerWordsForParameter(p);
  } else
    sayForm(parameters[p].instance, form);
#endif
}


/*----------------------------------------------------------------------

  Print an expanded symbolic reference.

  N = newline
  I = indent on a new line
  P = new paragraph
  L = current location name
  O = current object -> first parameter!
  <n> = n:th parameter
  +<n> = definite form of n:th parameter
  0<n> = indefinite form of n:th parameter
  !<n> = pronoun for the n:th parameter
  V = current verb
  A = current actor
  T = tabulation
  $ = no space needed after this, and don't capitalize
 */
static char *printSymbol(char *str)	/* IN - The string starting with '$' */
{
  int advance = 2;

  if (*str == '\0') printAndLog("$");
  else switch (toLower(str[1])) {
  case 'n':
    newline();
    needSpace = FALSE;
    break;
  case 'i':
    newline();
    printAndLog("    ");
    col = 5;
    needSpace = FALSE;
    break;
  case 'o':
    space();
    sayParameter(0, 0);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case '+':
  case '0':
  case '-':
  case '!':
    space();
    if (isdigit(str[2])) {
      int form;
      switch (str[1]) {
      case '+': form = SAY_DEFINITE; break;
      case '0': form = SAY_INDEFINITE; break;
      case '-': form = SAY_NEGATIVE; break;
      case '!': form = SAY_PRONOUN; break;
      default: form = SAY_SIMPLE; break;
      }
      sayParameter(str[2]-'1', form);
      needSpace = TRUE;
    }
    advance = 3;
    break;
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    space();
    sayParameter(str[1]-'1', SAY_SIMPLE);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'l':
    space();
    say(current.location);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'a':
    space();
    say(current.actor);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'v':
    space();
    justify((char *)pointerTo(dictionary[verbWord].string));
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'p':
    para();
    needSpace = FALSE;
    break;
  case 't': {
    int i;
    int spaces = 4-(col-1)%4;

    for (i = 0; i<spaces; i++) printAndLog(" ");
    col = col + spaces;
    needSpace = FALSE;
    break;
  }
  case '$':
    skipSpace = TRUE;
    capitalize = FALSE;
    break;
  default:
    printAndLog("$");
    break;
  }

  return &str[advance];
}


/*----------------------------------------------------------------------*/
static Bool inhibitSpace(char *str) {
  return str[0] == '$' && str[1] == '$';
}


/*----------------------------------------------------------------------*/
static Bool isSpaceEquivalent(char str[]) {
  if (str[0] == ' ')
    return TRUE;
  else
    return strncmp(str, "$p", 2) == 0
      || strncmp(str, "$n", 2) == 0
      || strncmp(str, "$i", 2) == 0
      || strncmp(str, "$t", 2) == 0;
}


/*----------------------------------------------------------------------*/
static Bool punctuationNext(char *str) {
  char *punctuation = strchr(".,!?", str[0]);
  Bool end = str[1] == '\0';
  Bool space = isSpaceEquivalent(&str[1]);
  return (punctuation != NULL && (end || space));
}


/*----------------------------------------------------------------------*/
static char lastCharOf(char *str) {
  return str[strlen(str)-1];
}


/*======================================================================*/
void output(char original[])
{
  char ch;
  char *str, *copy;
  char *symptr;

  copy = strdup(original);
  str = copy;

  if (inhibitSpace(str) || punctuationNext(str))
    needSpace = FALSE;
  else
    space();			/* Output space if needed (& not inhibited) */

  /* Output string up to symbol and handle the symbol */
  while ((symptr = strchr(str, '$')) != (char *) NULL) {
    ch = *symptr;		/* Terminate before symbol */
    *symptr = '\0';
    if (strlen(str) > 0) {
      skipSpace = FALSE;	/* Only let skipSpace through if it is
				   last in the string */
      if (lastCharOf(str) == ' ') {
	str[strlen(str)-1] = '\0'; /* Truncate space character */
	justify(str);		/* Output part before '$' */
	needSpace = TRUE;
      } else {
	justify(str);		/* Output part before '$' */
	needSpace = FALSE;
      }
    }
    *symptr = ch;		/* restore '$' */
    str = printSymbol(symptr);	/* Print the symbolic reference and advance */
  }

  if (str[0] != 0) {
    justify(str);			/* Output trailing part */
    skipSpace = FALSE;
    if (lastCharOf(str) != ' ')
      needSpace = TRUE;
  }
  if (needSpace)
    capitalize = strchr("!?.", str[strlen(str)-1]) != 0;
  anyOutput = TRUE;
  free(copy);
}


/*======================================================================*/
void printMessage(MsgKind msg)		/* IN - message number */
{
  interpret(msgs[msg].stms);
}

/*======================================================================*/
void printMessageWithParameters(MsgKind msg, Parameter *messageParameters)
{
	Parameter *savedParameters = allocateParameterArray(NULL);
	copyParameterList(savedParameters, parameters);
	if (messageParameters != NULL)
		copyParameterList(parameters, messageParameters);

	interpret(msgs[msg].stms);

	copyParameterList(parameters, savedParameters);
	free(savedParameters);
}


/*----------------------------------------------------------------------*\

  Various check functions

  endOfTable()
  isObj, isLoc, isAct, IsCnt & isNum

\*----------------------------------------------------------------------*/

/* Instance query methods */
// TODO Move to Instance.c

Bool isObject(int instance)
{
  return isA(instance, OBJECT);
}

Bool isContainer(int instance)
{
  return instance != 0 && instances[instance].container != 0;
}

Bool isActor(int instance)
{
  return isA(instance, ACTOR);
}

Bool isLocation(int instance)
{
  return isA(instance, LOCATION);
}


Bool isLiteral(int instance)
{
  return instance > header->instanceMax;
}

Bool isNumeric(int instance)
{
  return isLiteral(instance) && literal[literalFromInstance(instance)].type == NUMERIC_LITERAL;
}

Bool isString(int instance)
{
  return isLiteral(instance) && literal[literalFromInstance(instance)].type == STRING_LITERAL;
}



/* Word class query methods, move to Word.c */
/* Word classes are numbers but in the dictionary they are generated as bits */
static Bool isVerb(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&VERB_BIT)!=0;
}

Bool isVerbWord(int wordIndex) {
	return isVerb(playerWords[wordIndex].code);
}

static Bool isConjunction(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&CONJUNCTION_BIT)!=0;
}

Bool isConjunctionWord(int wordIndex) {
	return isConjunction(playerWords[wordIndex].code);
}

static Bool isBut(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&EXCEPT_BIT)!=0;
}

Bool isButWord(int wordIndex) {
	return isBut(playerWords[wordIndex].code);
}

static Bool isThem(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&THEM_BIT)!=0;
}

Bool isThemWord(int wordIndex) {
	return isThem(playerWords[wordIndex].code);
}

static Bool isIt(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&IT_BIT)!=0;
}

Bool isItWord(int wordIndex) {
	return isIt(playerWords[wordIndex].code);
}

static Bool isNoun(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&NOUN_BIT)!=0;
}

Bool isNounWord(int wordIndex) {
	return isNoun(playerWords[wordIndex].code);
}

static Bool isAdjective(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&ADJECTIVE_BIT)!=0;
}

Bool isAdjectiveWord(int wordIndex) {
	return isAdjective(playerWords[wordIndex].code);
}

static Bool isPreposition(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&PREPOSITION_BIT)!=0;
}

Bool isPrepositionWord(int wordIndex) {
	return isPreposition(playerWords[wordIndex].code);
}

Bool isAll(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&ALL_BIT)!=0;
}

Bool isAllWord(int wordIndex) {
	return isAll(playerWords[wordIndex].code);
}

static Bool isDir(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&DIRECTION_BIT)!=0;
}

Bool isDirectionWord(int wordIndex) {
	return isDir(playerWords[wordIndex].code);
}

Bool isNoise(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&NOISE_BIT)!=0;
}

Bool isPronoun(int wordCode) {
  return wordCode < dictionarySize && (dictionary[wordCode].classBits&PRONOUN_BIT)!=0;
}

Bool isPronounWord(int wordIndex) {
	return isPronoun(playerWords[wordIndex].code);
}

Bool isLiteralWord(int wordIndex) {
  return playerWords[wordIndex].code >= dictionarySize;
}



#ifdef CHECKOBJ
/*======================================================================

  checkobj()

  Check that the object given is valid, else print an error message
  or find out what he wanted.

  This routine is not used any longer, kept for sentimental reasons ;-)

  */
void checkobj(obj)
     Aword *obj;
{
  Aword oldobj;

  if (*obj != EOF)
    return;

  oldobj = EOF;
  for (cur.obj = OBJMIN; cur.obj <= OBJMAX; cur.obj++) {
    /* If an object is present and it is possible to perform his action */
    if (isHere(cur.obj) && possible())
      if (oldobj == EOF)
	oldobj = cur.obj;
      else
	error(WANT);          /* And we didn't find multiple objects */
    }

  if (oldobj == EOF)
    error(WANT);              /* But we found ONE */

  *obj = cur.obj = oldobj;
  output("($o)");             /* Then he surely meant this object */
}
#endif




/*----------------------------------------------------------------------*\

  Event Handling

  eventchk()

\*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
static void runPendingEvents(void)
{
  int i;

  while (eventQueueTop != 0 && eventQueue[eventQueueTop-1].after == 0) {
    eventQueueTop--;
    if (isLocation(eventQueue[eventQueueTop].where))
      current.location = eventQueue[eventQueueTop].where;
    else
      current.location = where(eventQueue[eventQueueTop].where, FALSE);
    if (sectionTraceOption) {
      printf("\n<EVENT %d (at ", eventQueue[eventQueueTop].event);
      traceSay(current.location);
      printf("):>\n");
    }
    interpret(events[eventQueue[eventQueueTop].event].code);
  }

  for (i = 0; i<eventQueueTop; i++)
    eventQueue[i].after--;
}




/*----------------------------------------------------------------------*\

  Main program and initialisation

\*----------------------------------------------------------------------*/


static FILE *codfil;
static char codfnm[256] = "";
static char txtfnm[256] = "";
static char logfnm[256] = "";


/*----------------------------------------------------------------------*/
static char *decodeState(int c) {
  static char state[2] = "\0\0";
  switch (c) {
  case 0: return ".";
  case 'd': return "dev";
  case 'a': return "alpha";
  case 'b': return "beta";
  default:
    state[0] = header->version[3];
    return state;
  }
}


/*----------------------------------------------------------------------*/
static void incompatibleDevelopmentVersion(ACodeHeader *header) {
  char str[80];
  sprintf(str, "Incompatible version of ACODE program. Development versions always require exact match. Game is %ld.%ld%s%ld, interpreter %ld.%ld%s%ld!",
	  (long)(header->version[0]),
	  (long)(header->version[1]),
	  decodeState(header->version[3]),
	  (long)(header->version[2]),
	  (long)alan.version.version,
	  (long)alan.version.revision,
	  alan.version.state,
	  (long)alan.version.correction);
  apperr(str);
}


/*----------------------------------------------------------------------*/
static void incompatibleVersion(ACodeHeader *header) {
  char str[80];
  sprintf(str, "Incompatible version of ACODE program. Game is %ld.%ld, interpreter %ld.%ld.",
	  (long)(header->version[0]),
	  (long)(header->version[1]),
	  (long)alan.version.version,
	  (long)alan.version.revision);
  apperr(str);
}


/*----------------------------------------------------------------------*/
static void alphaRunningLaterGame(char gameState) {
  output("<WARNING! You are running an alpha interpreter, but the game is generated by a");
  if (gameState == 'b')
    output("beta");
  else
    output("release");
  output("state compiler which was released later. This might cause the game to not work fully as intended. Look for an upgraded game file.>\n");
}

/*----------------------------------------------------------------------*/
static void nonDevelopmentRunningDevelopmentStateGame(char version[]) {
  char errorMessage[100];
  char versionString[100];

  strcpy(errorMessage, "Games generated by a development state compiler");
  sprintf(versionString, "(this game is v%d.%d.%d%s)", version[0], version[1],
	  version[2], decodeState(version[3]));
  strcat(errorMessage, versionString);
  strcat(errorMessage, "can only be run with a matching interpreter. Look for a game file generated with an alpha, beta or release state compiler.>\n");
  apperr(errorMessage);
}


/*----------------------------------------------------------------------*/
static void checkVersion(ACodeHeader *header)
{
  /* Strategy for version matching is:
     1) Development interpreters/games require exact match
     2) Alpha, Beta and Release interpreters will not run development games
     3) Alpha interpreters must warn if they run beta or release games
     4) Beta interpreters may introduce changes which are not alpha compatible,
        if the change is a strict addition (i.e. if not used will not affect
        alpha interpreters, example is introduction of a new opcode if it is
        done at the end of the list)
     5) Release interpreters should run alpha and beta games without problems
    */

  char interpreterVersion[4];
  Abool developmentVersion;
  Abool alphaVersion;
  int compareLength;
  char gameState = header->version[3];

  /* Construct our own version */
  interpreterVersion[0] = alan.version.version;
  interpreterVersion[1] = alan.version.revision;
  interpreterVersion[2] = alan.version.correction;
  interpreterVersion[3] = alan.version.state[0];

  /* Check version of .ACD file */
  if (debugOption && !regressionTestOption) {
    printf("<Version of '%s' is %d.%d%s%d>",
	   adventureFileName,
	   (int)(header->version[0]),
	   (int)(header->version[1]),
	   decodeState(gameState),
	   (int)(header->version[2]));
    newline();
  }

  /* Development version require exact match, else only 2 digit match */
  developmentVersion = (strcmp(alan.version.state, "dev") == 0);
  alphaVersion = (strcmp(alan.version.state, "alpha") == 0);
  compareLength = (developmentVersion? 3 : 2);

  if (gameState == 'd' && !developmentVersion)
    /* Development state game requires development state interpreter... */
    nonDevelopmentRunningDevelopmentStateGame(header->version);
  else {
    /* Compatible if version, revision (and correction if dev state) match... */
    if (memcmp(header->version, interpreterVersion, compareLength) != 0) {
      /* Mismatch! */
      if (!ignoreErrorOption) {
	if (developmentVersion)
	  incompatibleDevelopmentVersion(header);
	else
	  incompatibleVersion(header);
      } else
	output("<WARNING! Incompatible version of ACODE program.>\n");
    } else if (developmentVersion && gameState != 'd')
      /* ... unless interpreter is development and game not */
      incompatibleDevelopmentVersion(header);
    else if (alphaVersion && gameState != 'a') {
      /* If interpreter is alpha version and the game is later, warn! */
      alphaRunningLaterGame(gameState);
    }
  }
}

/*----------------------------------------------------------------------
  Calculate where the actual memory starts. Might be different for
  different versions.
*/
static int memoryStart(char version[4]) {
  /* Pre 3.0alpha5 had a shorter header */
  if (version[3] == 3 && version[2] == 0 && version[0] == 'a' && version[1] <5)
    return sizeof(Pre3_0alpha5Header)/sizeof(Aword);
  else
    return sizeof(ACodeHeader)/sizeof(Aword);
}



/*----------------------------------------------------------------------*/
static void load(void)
{
  ACodeHeader tmphdr;
  Aword crc = 0;
  int i;
  char err[100];

  rewind(codfil);
  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  rewind(codfil);
  if (strncmp((char *)&tmphdr, "ALAN", 4) != 0)
    apperr("Not an Alan game file, does not start with \"ALAN\"");

  checkVersion(&tmphdr);

  /* Allocate and load memory */

  if (littleEndian())
    reverseHdr(&tmphdr);

  if (tmphdr.size <= sizeof(ACodeHeader)/sizeof(Aword))
    syserr("Malformed game file. Too small.");

  /* No memory allocated yet? */
  if (memory == NULL) {
    memory = allocate(tmphdr.size*sizeof(Aword));
  }
  header = (ACodeHeader *) pointerTo(0);

  memTop = fread(pointerTo(0), sizeof(Aword), tmphdr.size, codfil);
  if (memTop != tmphdr.size)
    syserr("Could not read all ACD code.");

  /* Calculate checksum */
  for (i = memoryStart(tmphdr.version); i < memTop; i++) {
    crc += memory[i]&0xff;
    crc += (memory[i]>>8)&0xff;
    crc += (memory[i]>>16)&0xff;
    crc += (memory[i]>>24)&0xff;
#ifdef CRCLOG
    printf("%6x\t%6lx\t%6lx\n", i, crc, memory[i]);
#endif
  }
  if (crc != tmphdr.acdcrc) {
    sprintf(err, "Checksum error in Acode (.a3c) file (0x%lx instead of 0x%lx).",
	    crc, tmphdr.acdcrc);
    if (!ignoreErrorOption)
      syserr(err);
    else {
      output("<WARNING! $$");
      output(err);
      output("$$ Ignored, proceed at your own risk.>$n");
    }
  }

  if (littleEndian()) {
    if (debugOption||sectionTraceOption||singleStepOption)
      output("<Hmm, this is a little-endian machine, fixing byte ordering....");
    reverseACD();			/* Reverse content of the ACD file */
    if (debugOption||sectionTraceOption||singleStepOption)
      output("OK.>$n");
  }
}


/*----------------------------------------------------------------------*/
static void checkDebug(void)
{
  /* Make sure he can't debug if not allowed! */
  if (!header->debug) {
    if (debugOption|sectionTraceOption|singleStepOption) {
      printf("<Sorry, '%s' is not compiled for debug!>\n", adventureFileName);
      terminate(0);
    }
    para();
    debugOption = FALSE;
    sectionTraceOption = FALSE;
    singleStepOption = FALSE;
    tracePushOption = FALSE;
  }

  if (debugOption)		/* If debugging... */
    srand(0);			/* ... use no randomization */
  else
    srand(time(0));		/* Else seed random generator */
}


/*----------------------------------------------------------------------*/
static void initStaticData(void)
{
  /* Dictionary */
  dictionary = (DictionaryEntry *) pointerTo(header->dictionary);
  /* Find out number of entries in dictionary */
  for (dictionarySize = 0; !isEndOfList(&dictionary[dictionarySize]); dictionarySize++);

  /* Scores */


  /* All addresses to tables indexed by ids are converted to pointers,
     then adjusted to point to the (imaginary) element before the
     actual table so that [0] does not exist. Instead indices goes
     from 1 and we can use [1]. */

  if (header->instanceTableAddress == 0)
    syserr("Instance table pointer == 0");
  instances = (InstanceEntry *) pointerTo(header->instanceTableAddress);
  instances--;			/* Back up one so that first is no. 1 */


  if (header->classTableAddress == 0)
    syserr("Class table pointer == 0");
  classes = (ClassEntry *) pointerTo(header->classTableAddress);
  classes--;			/* Back up one so that first is no. 1 */

  if (header->containerTableAddress != 0) {
    container = (ContainerEntry *) pointerTo(header->containerTableAddress);
    container--;
  }

  if (header->eventTableAddress != 0) {
    events = (EventEntry *) pointerTo(header->eventTableAddress);
    events--;
  }

  /* Scores, if already allocated, copy initial data */
  if (scores == NULL)
    scores = duplicate((Aword *) pointerTo(header->scores), header->scoreCount*sizeof(Aword));
  else
    memcpy(scores, pointerTo(header->scores),
	   header->scoreCount*sizeof(Aword));


  stxs = (SyntaxEntry *) pointerTo(header->syntaxTableAddress);
  vrbs = (VerbEntry *) pointerTo(header->verbTableAddress);
  ruls = (RulEntry *) pointerTo(header->ruleTableAddress);
  msgs = (MessageEntry *) pointerTo(header->messageTableAddress);

  if (header->pack)
    freq = (Aword *) pointerTo(header->freq);
}


/*----------------------------------------------------------------------*/
static void initStrings(void)
{
  StringInitEntry *init;

  for (init = (StringInitEntry *) pointerTo(header->stringInitTable); !isEndOfList(init); init++)
    setValue(init->instanceCode, init->attributeCode, (Aword)getStringFromFile(init->fpos, init->len));
}

/*----------------------------------------------------------------------*/
static Aint sizeOfAttributeData(void)
{
  int i;
  int size = 0;

  for (i=1; i<=header->instanceMax; i++) {
    AttributeEntry *attribute = pointerTo(instances[i].initialAttributes);
    while (!isEndOfList(attribute)) {
      size += AwordSizeOf(AttributeEntry);
      attribute++;
    }
    size += 1;			/* For EOF */
  }

  if (size != header->attributesAreaSize)
    syserr("Attribute area size calculated wrong.");
  return size;
}


/*----------------------------------------------------------------------*/
static AttributeEntry *initializeAttributes(int awordSize)
{
  Aword *attributeArea = allocate(awordSize*sizeof(Aword));
  Aword *currentAttributeArea = attributeArea;
  int i;

  for (i=1; i<=header->instanceMax; i++) {
    AttributeEntry *originalAttribute = pointerTo(instances[i].initialAttributes);
    admin[i].attributes = (AttributeEntry *)currentAttributeArea;
    while (!isEndOfList(originalAttribute)) {
      *((AttributeEntry *)currentAttributeArea) = *originalAttribute;
      currentAttributeArea += AwordSizeOf(AttributeEntry);
      originalAttribute++;
    }
    *((Aword*)currentAttributeArea) = EOF;
    currentAttributeArea += 1;
  }

  return (AttributeEntry *)attributeArea;
}




/*----------------------------------------------------------------------*/
static void initDynamicData(void)
{
  int instanceId;

  /* Allocate for administrative table */
  admin = (AdminEntry *)allocate((header->instanceMax+1)*sizeof(AdminEntry));

  /* Create game state copy of attributes */
  attributes = initializeAttributes(sizeOfAttributeData());

  /* Initialise string & set attributes */
  initStrings();
  initSets((SetInitEntry*)pointerTo(header->setInitTable));

  /* Set initial locations */
  for (instanceId = 1; instanceId <= header->instanceMax; instanceId++)
    admin[instanceId].location = instances[instanceId].initialLocation;
}


/*----------------------------------------------------------------------*/
static void runInheritedInitialize(Aint theClass) {
  if (theClass == 0) return;
  runInheritedInitialize(classes[theClass].parent);
  if (classes[theClass].initialize)
    interpret(classes[theClass].initialize);
}


/*----------------------------------------------------------------------*/
static void runInitialize(Aint theInstance) {
  runInheritedInitialize(instances[theInstance].parent);
  if (instances[theInstance].initialize != 0)
    interpret(instances[theInstance].initialize);
}


/*----------------------------------------------------------------------*/
static void initializeInstances() {
  int instanceId;

  /* Set initial locations */
  for (instanceId = 1; instanceId <= header->instanceMax; instanceId++) {
    current.instance = instanceId;
    runInitialize(instanceId);
  }
}


/*----------------------------------------------------------------------*/
static void start(void)
{
  int startloc;

  current.tick = -1;
  current.location = startloc = where(HERO, FALSE);
  current.actor = HERO;
  current.score = 0;

  initializeInstances();

  if (sectionTraceOption)
    printf("\n<START:>\n");
  interpret(header->start);
  para();

  locate(HERO, startloc);
  rules();
}



/*----------------------------------------------------------------------*/
static void openFiles(void)
{
  char str[256];
  char *usr = "";
  time_t tick;

  /* Open Acode file */
  strcpy(codfnm, adventureFileName);
  if ((codfil = fopen(codfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure code file '");
    strcat(str, codfnm);
    strcat(str, "'.");
    apperr(str);
  }

  /* Open Text file */
  strcpy(txtfnm, adventureFileName);
  if ((textFile = fopen(txtfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure text data file '");
    strcat(str, txtfnm);
    strcat(str, "'.");
    apperr(str);
  }

  /* If logging open log file */
  if (transcriptOption || logOption) {
    time(&tick);
    sprintf(logfnm, "%s%d%s.log", adventureName, (int)tick, usr);
#ifdef HAVE_GLK
    glui32 fileUsage = transcriptOption?fileusage_Transcript:fileusage_InputRecord;
    frefid_t logFileRef = glk_fileref_create_by_name(fileUsage, logfnm, 0);
    logFile = glk_stream_open_file(logFileRef, filemode_Write, 0);
#else
    logFile = fopen(logfnm, "w");
#endif
    if (logFile == NULL) {
      transcriptOption = FALSE;
      logOption = FALSE;
    }
  }
}


/*----------------------------------------------------------------------*/
static void init(void)
{
  int i;

  if (!regressionTestOption && (debugOption||sectionTraceOption||singleStepOption)) {
    char str[80];
    output("<Hi! This is Alan interactive fiction interpreter Arun,");
    sprintf(str, "version %ld.%ld%s%ld!>$n",
	    (long)alan.version.version,
	    (long)alan.version.revision,
	    alan.version.state[0]=='\0'?".":alan.version.state,
	    (long)alan.version.correction);
    output(str);
  }

  /* Initialise some status */
  eventQueueTop = 0;			/* No pending events */
  initStaticData();
  initDynamicData();
  initParse();
  checkDebug();

  getPageSize();

  /* Find first conjunction and use that for ',' handling */
  for (i = 0; i < dictionarySize; i++)
    if (isConjunction(i)) {
      conjWord = i;
      break;
    }

  /* Start the adventure */
  if (debugOption)
    debug(FALSE, 0, 0);
  else
    clear();

  start();
}



static Bool traceActor(int theActor)
{
  if (sectionTraceOption) {
    printf("\n<ACTOR %d, ", theActor);
    traceSay(theActor);
    printf(" (at ");
    traceSay(current.location);
  }
  return sectionTraceOption;
}


/*----------------------------------------------------------------------*/
static char *scriptName(int theActor, int theScript)
{
  ScriptEntry *scriptEntry = pointerTo(header->scriptTableAddress);

  while (theScript > 1) {
    scriptEntry++;
    theScript--;
  }
  return pointerTo(scriptEntry->stringAddress);
}


/*----------------------------------------------------------------------*/
static void moveActor(int theActor)
{
  ScriptEntry *scr;
  StepEntry *step;
  Aword previousInstance = current.instance;

  current.actor = theActor;
  current.instance = theActor;
  current.location = where(theActor, FALSE);
  if (theActor == HERO) {
    /* Ask him! */
    if (setjmp(forfeitLabel) == 0) {
      parse(parameters);
      capitalize = TRUE;
      fail = FALSE;			/* fail only aborts one actor */
    }
  } else if (admin[theActor].script != 0) {
    for (scr = (ScriptEntry *) pointerTo(header->scriptTableAddress); !isEndOfList(scr); scr++) {
      if (scr->code == admin[theActor].script) {
	/* Find correct step in the list by indexing */
	step = (StepEntry *) pointerTo(scr->steps);
	step = (StepEntry *) &step[admin[theActor].step];
	/* Now execute it, maybe. First check wait count */
	if (admin[theActor].waitCount > 0) { /* Wait some more ? */
	  if (traceActor(theActor))
	    printf("), SCRIPT %s(%ld), STEP %ld, Waiting another %ld turns>\n",
		   scriptName(theActor, admin[theActor].script),
		   admin[theActor].script, admin[theActor].step+1,
		   admin[theActor].waitCount);
	  admin[theActor].waitCount--;
	  break;
	}
	/* Then check possible expression to wait for */
	if (step->exp != 0) {
	  if (traceActor(theActor))
	    printf("), SCRIPT %s(%ld), STEP %ld, Evaluating:>\n",
		   scriptName(theActor, admin[theActor].script),
		   admin[theActor].script, admin[theActor].step+1);
	  if (!evaluate(step->exp))
	    break;		/* Break loop, don't execute step*/
	}
	/* OK, so finally let him do his thing */
	admin[theActor].step++;		/* Increment step number before executing... */
	if (!isEndOfList(step+1) && (step+1)->after != 0) {
	  admin[theActor].waitCount = evaluate((step+1)->after);
	}
	if (traceActor(theActor))
	  printf("), SCRIPT %ld(%s), STEP %ld, Executing:>\n",
		 admin[theActor].script,
		 scriptName(theActor, admin[theActor].script),
		 admin[theActor].step);
	interpret(step->stms);
	step++;
	/* ... so that we can see if he is USEing another script now */
	if (admin[theActor].step != 0 && isEndOfList(step))
	  /* No more steps in this script, so stop him */
	  admin[theActor].script = 0;
	fail = FALSE;			/* fail only aborts one actor */
	break;			/* We have executed a script so leave loop */
      }
    }
    if (isEndOfList(scr))
      syserr("Unknown actor script.");
  } else {
    if (sectionTraceOption) {
      printf("\n<ACTOR %d, ", theActor);
      traceSay(theActor);
      printf(" (at ");
      traceSay(current.location);
      printf("), Idle>\n");
    }
  }

  current.instance = previousInstance;
}

/*======================================================================*/
void run(void)
{
	int i;
	Bool playerChangedState;
	Stack theStack;

	openFiles();
	load();			/* Load program */

	if (setjmp(restartLabel) != NO_JUMP_RETURN) {	/* Return here if he wanted to restart */
		/* So, a RESTART! */
		deleteStack(theStack);
	}

	theStack = createStack(STACKSIZE);
	setInterpreterStack(theStack);

	initStateStack();

	if (setjmp(returnLabel) == NO_JUMP_RETURN)
		init();			/* Initialise and start the adventure */

	while (TRUE) {
		if (debugOption)
			debug(FALSE, 0, 0);

		runPendingEvents();
		current.tick++;

		/* Return here if error during execution */
		switch (setjmp(returnLabel)) {
		case NO_JUMP_RETURN:
			break;
		case ERROR_RETURN:
			forgetGameState();
			forceNewPlayerInput();
			break;
		case UNDO_RETURN:
			forceNewPlayerInput();
			break;
		default:
			syserr("Unexpected longjmp() return value");
		}

#ifdef DMALLOC
		dmalloc_verify(0);
#endif
		depth = 0;

		/* Move all characters, hero first */
		rememberGameState();

		/* TODO: Why 'playerChangedState' since gameStateChanged is sufficient
		 * Actually it isn't since it might have been one of the events or other actors
		 * that changed the state. Why is this important?
		 * Yes, because for UNDO we want to undo the last command the player did that
		 * changed the state, not any of the others. */
		playerChangedState = FALSE;
		moveActor(header->theHero);
		playerChangedState = gameStateChanged;

		if (gameStateChanged)
			rememberCommands();
		else
			forgetGameState();

		rules();
		for (i = 1; i <= header->instanceMax; i++)
			if (i != header->theHero && isActor(i)) {
				moveActor(i);
				rules();
			}
	}
}

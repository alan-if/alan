/*----------------------------------------------------------------------*\

  MAIN.C

  Main module of interpreter for ALAN Adventure Language

\*----------------------------------------------------------------------*/

#include "sysdep.h"

#include "acode.h"
#include "types.h"
#include "set.h"
#include "main.h"
#include "syserr.h"

#include <time.h>
#ifdef USE_READLINE
#include "readline.h"
#endif

#ifdef HAVE_SHORT_FILENAMES
#include "av.h"
#else
#include "alan.version.h"
#endif

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
static AcdHdr dummyHeader;	/* Dummy to use until memory allocated */
AcdHdr *header = &dummyHeader;

int memTop = 0;			/* Top of load memory */

int conjWord;			/* First conjunction in dictonary, for ',' */


/* Amachine variables */
CurVars current;

/* The event queue */
int eventQueueSize = 0;
EventQueueEntry *eventQueue = NULL; /* Event queue */
Aint eventQueueTop = 0;		/* Event queue top pointer */

/* Amachine structures - Dynamic */
InstanceEntry *instance;	/* Instance table pointer */
AdminEntry *admin;		/* Administrative data about instances */
AttributeEntry *attributes;	/* Dynamic attribute values */
Aword *scores;			/* Score table pointer */

/* Amachine structures - Static */
ContainerEntry *container;	/* Container table pointer */
ClassEntry *class;		/* Class table pointer */
DictionaryEntry *dictionary;	/* Dictionary pointer */
VerbEntry *vrbs;		/* Verb table pointer */
ParseEntry *stxs;		/* Syntax table pointer */
RulEntry *ruls;			/* Rule table pointer */
EventEntry *events;		/* Event table pointer */
MsgEntry *msgs;			/* Message table pointer */
Aword *freq;			/* Cumulative character frequencies */

int dictsize;

Bool verbose = FALSE;
Bool ignoreErrorOption = TRUE;
Bool debugOption = FALSE;
Bool sectionTraceOption = FALSE;
Bool tracePushOption = FALSE;
Bool traceStackOption = FALSE;
Bool singleStepOption = FALSE;
Bool transcriptOption = FALSE;
Bool logOption = FALSE;
Bool statusLineOption = TRUE;
Bool regressionTestOption = FALSE;
Bool fail = FALSE;
Bool anyOutput = FALSE;


/* The files and filenames */
char *adventureName;
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
Bool skipsp = FALSE;

/* Restart jump buffer */
jmp_buf restartLabel;		/* Restart long jump return point */
jmp_buf errorLabel;		/* Error (or undo) long jump return point */
jmp_buf forfeitLabel;		/* Player forfeit by an empty command */


/* PRIVATE DATA */
static Bool onStatusLine = FALSE; /* Don't log when printing status */


/*======================================================================

  terminate()

  Terminate the execution of the adventure, e.g. close windows,
  return buffers...

 */
void terminate(int code)
{
#ifdef __amiga__
#ifdef AZTEC_C
#include <fcntl.h>
  extern struct _dev *_devtab;
  char buf[85];
  
  if (con) { /* Running from WB, created a console so kill it */
    /* Running from WB, so we created a console and
       hacked the Aztec C device table to use it for all I/O
       so now we need to make it close it (once!) */
    _devtab[1].fd = _devtab[2].fd = 0;
  } else
#else
  /* Geek Gadgets GCC */
#include <workbench/startup.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

  if (_WBenchMsg != NULL) {
    Close(window);
    if (_WBenchMsg->sm_ArgList != NULL)
      UnLock(CurrentDir(cd));
  } else
#endif
#endif
    newline();
  free(memory);
  if (transcriptOption)
#ifdef HAVE_GLK
    glk_stream_close(logFile, NULL);
#else
    fclose(logFile);
#endif

#ifdef __MWERKS__
  printf("Command-Q to close window.");
#endif

#ifdef HAVE_GLK
  glk_exit();
#else
  exit(code);
#endif
}

/*======================================================================*/
void usage(void)
{
  printf("\nArun, Adventure Interpreter version %s (%s %s)\n\n",
	 alan.version.string, alan.date, alan.time);
  printf("Usage:\n\n");
  printf("    %s [<switches>] <adventure>\n\n", PROGNAME);
  printf("where the possible optional switches are:\n");
#ifdef HAVE_GLK
  glk_set_style(style_Preformatted);
#endif
  printf("    -v       verbose mode\n");
  printf("    -l       log transcript to a file\n");
  printf("    -c       log player commands to a file\n");
  printf("    -n       no Status Line\n");
  printf("    -d       enter debug mode\n");
  printf("    -t[<n>]  trace game execution, higher <n> gives more trace\n");
  printf("    -i       ignore version and checksum errors\n");
  printf("    -r       refrain from printing timestamps and paging (making regression testing easier)\n");
#ifdef HAVE_GLK
  glk_set_style(style_Normal);
#endif
}


/*======================================================================*/
void error(MsgKind msgno)	/* IN - The error message number */
{
  /* Print an error message, force new player input and abort. */
  if (msgno != MSGMAX)
    prmsg(msgno);
  playerWords[wordIndex] = EOF;		/* Force new player input */
  dscrstkp = 0;			/* Reset describe stack */
  longjmp(errorLabel,TRUE);
}


/*======================================================================*/
void statusline(void)
{
#ifdef HAVE_GLK
  glui32 glkWidth;
  char line[100];
  int pcol = col;

  if (glkStatusWin == NULL)
    return;

  glk_set_window(glkStatusWin);
  glk_window_clear(glkStatusWin);
  glk_window_get_size(glkStatusWin, &glkWidth, NULL);

  onStatusLine = TRUE;
  col = 1;
  glk_window_move_cursor(glkStatusWin, 1, 0);
  interpret(instance[where(HERO, TRUE)].mentioned);

  if (header->maxscore > 0)
    sprintf(line, "Score %d(%d)/%d moves", current.score, (int)header->maxscore, current.tick);
  else
    sprintf(line, "%d moves", current.tick);
  glk_window_move_cursor(glkStatusWin, glkWidth - col - strlen(line), 0);
  glk_put_string(line);
  needSpace = FALSE;

  col = pcol;
  onStatusLine = FALSE;

  glk_set_window(glkMainWin); 
#else
#ifdef HAVE_ANSI
  char line[100];
  int i;
  int pcol = col;

  if (!statusLineOption) return;
  /* ansi_position(1,1); ansi_bold_on(); */
  printf("\x1b[1;1H");
  printf("\x1b[7m");

  onStatusLine = TRUE;
  col = 1;
  interpret(instance[where(HERO, FALSE)].mentioned);

  if (header->maxscore > 0)
    sprintf(line, "Score %d(%ld)/%d moves", current.score, header->maxscore, current.tick);
  else
    sprintf(line, "%ld moves", (long)current.tick);
  for (i=0; i < pageWidth - col - strlen(line); i++) putchar(' ');
  printf(line);
  printf("\x1b[m");
  printf("\x1b[%d;1H", pageLength);

  needSpace = FALSE;
  capitalize = TRUE;

  onStatusLine = FALSE;
  col = pcol;
#endif
#endif
}



/*----------------------------------------------------------------------*/
static int updateColumn(int currentColumn, char *string) {
  char *newlinePosition = strrchr(string, '\n');
  if (newlinePosition != NULL)
    return &string[strlen(string)] - newlinePosition;
  else
    return currentColumn + strlen(string);
}



/*======================================================================

  logPrint()

  Print some text and log it if logging is on.

 */
void logPrint(char string[])
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

  if (!regressionTestOption && lin >= pageLength - 1) {
    logPrint("\n");
    needSpace = FALSE;
    prmsg(M_MORE);
    statusline();
    fflush(stdout);
    fgets(buf, 256, stdin);
    getPageSize();
    lin = 0;
  } else
    logPrint("\n");
  
  lin++;
#else
  logPrint("\n");
#endif
  col = 1;
  needSpace = FALSE;
}


/*======================================================================*/
void para(void)
{
  /* Make a new paragraph, i.e one empty line (one or two newlines). */

#ifdef HAVE_GLK
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


/*======================================================================*/
void *allocate(unsigned long len)
{
  void *p = (void *)calloc((size_t)len, 1);

  if (p == NULL)
    syserr("Out of memory.");

  return p;
}


/*======================================================================*/
void *duplicate(void *original, unsigned long len)
{
  void *p = allocate(len);

  memcpy(p, original, len);
  return p;
}


/*----------------------------------------------------------------------*/
static void justify(char str[])
{
#ifdef HAVE_GLK
  logPrint(str);
#else
  int i;
  char ch;
  
  if (col >= pageWidth && !skipsp)
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
      logPrint(str);		/* and print it */
      skipsp = FALSE;		/* If skipping, now we're done */
      str[i] = ch;		/* Restore character */
      /* Skip white after printed portion */
      for (str = &str[i]; isSpace(str[0]) && str[0] != '\0'; str++);
    }
    newline();			/* Then start a new line */
  }
  logPrint(str);			/* Print tail */
#endif
  col = col + strlen(str);	/* Update column */
}


/*----------------------------------------------------------------------

  space()

  Output a space if needed.

 */
static void space(void)
{
  if (skipsp)
    skipsp = FALSE;
  else {
    if (needSpace) {
      logPrint(" ");
      col++;
    }
  }
  needSpace = FALSE;
}


/*----------------------------------------------------------------------*/
static void sayPlayerWordsForParameter(int p) {
  int i;

  for (i = parameters[p].firstWord; i <= parameters[p].lastWord; i++) {
    justify((char *)pointerTo(dictionary[playerWords[i]].wrd));
    if (i < parameters[p].lastWord)
      justify(" ");
  }
}


/*----------------------------------------------------------------------

  sayParameter()

  A parameter needs to be said, find out which instance it is and say it.

*/
static void sayParameter(int p, int form)
{
  int i;

  for (i = 0; i <= p; i++)
    if (parameters[i].code == EOF)
      syserr("Nonexistent parameter referenced.");

#ifdef ALWAYS_SAY_PARAMETERS_USING_PLAYER_WORDS
  if (params[p].firstWord != EOF) /* Any words he used? */
    /* Yes, so use them... */
    sayPlayerWordsForParameter(p);
  else
    sayForm(params[p].code, form);
#else
  if (parameters[p].code == 0) {
    /* Ambiguous instance referenced, so use the words he used */
    sayPlayerWordsForParameter(p);
  } else
    sayForm(parameters[p].code, form);
#endif
}


/*----------------------------------------------------------------------

  prsym()

  Print an expanded symbolic reference.

  N = newline
  I = indent on a new line
  P = new paragraph
  L = current location name
  O = current object -> first parameter!
  <n> = n:th parameter
  +<n> = definite form of n:th parameter
  -<n> = definite form of n:th parameter
  V = current verb
  A = current actor
  T = tabulation
  $ = no space needed after this, and don't capitalize
 */
static char *printSymbol(char *str)	/* IN - The string starting with '$' */
{
  int advance = 2;

  switch (toLower(str[1])) {
  case 'n':
    newline();
    needSpace = FALSE;
    break;
  case 'i':
    newline();
    logPrint("    ");
    col = 5;
    needSpace = FALSE;
    break;
  case 'o':
    sayParameter(0, 0);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case '+':
  case '0':
    if (isdigit(str[2])) {
      sayParameter(str[2]-'1', str[1]=='+'?SAY_DEFINITE:SAY_INDEFINITE);
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
    sayParameter(str[1]-'1', SAY_SIMPLE);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'l':
    say(current.location);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'a':
    say(current.actor);
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'v':
    justify((char *)pointerTo(dictionary[verbWord].wrd));
    needSpace = TRUE;		/* We did print something non-white */
    break;
  case 'p':
    para();
    needSpace = FALSE;
    break;
  case 't': {
    int i;
    int spaces = 4-(col-1)%4;
    
    for (i = 0; i<spaces; i++) logPrint(" ");
    col = col + spaces;
    needSpace = FALSE;
    break;
  }
  case '$':
    skipsp = TRUE;
    capitalize = FALSE;
    break;
  default:
    logPrint("$");
    break;
  }

  return &str[advance];
}


/*----------------------------------------------------------------------*/
static Bool inhibitSpace(char *str) {
  return str[0] == '$' && str[1] == '$';
}


/*----------------------------------------------------------------------*/
static Bool fullStopOrCommaNext(char *str) {
  return (str[0] == '.' || str[0] == ',') && (str[1] == '\0' || str[1] == ' ');
}


/*----------------------------------------------------------------------*/
static void capitalizeFirst(char *str) {
  int i = 0;

#ifdef CAPITALIZEFIRSTLETTER
  /* Skip over non-letters... */
  while (!isLetter(str[i]) && i < strlen(str)) i++;
#endif
  str[i] = toUpper(str[i]);
  capitalize = FALSE;
}


/*======================================================================

  output()

  Output a string to suit the screen. Any symbolic inserts ('$') are
  recogniced and performed.

 */
void output(char original[])
{
  char ch;
  char *str, *copy;
  char *symptr;

  copy = strdup(original);
  str = copy;

  if (!(inhibitSpace(str) || fullStopOrCommaNext(str)))
    space();			/* Output space if needed (& not inhibited) */

  /* Output string up to symbol and handle the symbol */
  while ((symptr = strchr(str, '$')) != (char *) NULL) {
    ch = *symptr;		/* Terminate before symbol */
    *symptr = '\0';
    if (strlen(str) > 0) {
      if (capitalize)
	capitalizeFirst(str);
      justify(str);		/* Output part before '$' */
      if (str[strlen(str)-1] == ' ')
	needSpace = FALSE;
    }
    *symptr = ch;		/* restore '$' */
    str = printSymbol(symptr);	/* Print the symbolic reference and advance */
  }

  if (capitalize)
    capitalizeFirst(str);

  if (str[0] != 0) {
    justify(str);			/* Output trailing part */
    skipsp = FALSE;
    if (str[strlen(str)-1] != ' ')
      needSpace = TRUE;
  }
  anyOutput = TRUE;
  capitalize = str[strlen(str)-1] == '.';
  free(copy);
}


/*======================================================================

  prmsg()

  Print a message from the message table.
  
  */
void prmsg(MsgKind msg)		/* IN - message number */
{
  interpret(msgs[msg].stms);
}


/*----------------------------------------------------------------------*\

  Various check functions

  endOfTable()
  isObj, isLoc, isAct, IsCnt & isNum

\*----------------------------------------------------------------------*/

/* How to know we are at end of a table */
Bool eot(Aword *adr)
{
  return *adr == EOF;
}


Bool isObj(Aword x)
{
  return isA((x), OBJECT);
}

Bool isContainer(Aword x)
{
  return (x) != 0 && instance[x].container != 0;
}

Bool isAct(Aword x)
{
  return isA((x), ACTOR);
}

Bool isLoc(Aword x)
{
  return isA((x), LOCATION);
}


Bool isLiteral(Aword x)
{
  return (x) > header->instanceMax;
}

Bool isNum(Aword x)
{
  return isLiteral(x) && literal[(x)-header->instanceMax].type == NUMERIC_LITERAL;
}

Bool isStr(Aword x)
{
  return isLiteral(x) && literal[(x)-header->instanceMax].type == STRING_LITERAL;
}


/* Word classes are numbers but in the dictonary they are generated as bits */
Bool isVerb(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<VERB_WORD))!=0;
}

Bool isConj(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<CONJUNCTION_WORD))!=0;
}

Bool isBut(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<BUT_WORD))!=0;
}

Bool isThem(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<THEM_WORD))!=0;
}

Bool isIt(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<IT_WORD))!=0;
}

Bool isNoun(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<NOUN_WORD))!=0;
}

Bool isAdjective(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<ADJECTIVE_WORD))!=0;
}

Bool isPreposition(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<PREPOSITION_WORD))!=0;
}

Bool isAll(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<ALL_WORD))!=0;
}

Bool isDir(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<DIRECTION_WORD))!=0;
}

Bool isNoise(int word) {
  return word < dictsize && (dictionary[word].classBits&((Aword)1L<<NOISE_WORD))!=0;
}

Bool isLiteralWord(int word) {
  return word >= dictsize;
}


/*======================================================================

  exitto()

  Is there an exit from one location to another?

  */
Bool exitto(int to, int from)
{
  ExitEntry *theExit;

  if (instance[from].exits == 0)
    return(FALSE); /* No exits */

  for (theExit = (ExitEntry *) pointerTo(instance[from].exits); !endOfTable(theExit); theExit++)
    if (theExit->target == to)
      return(TRUE);

  return(FALSE);
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


/*----------------------------------------------------------------------

  eventCheck()

  Check if any events are pending. If so execute them.
  */
static void eventCheck(void)
{
  while (eventQueueTop != 0 && eventQueue[eventQueueTop-1].time == current.tick) {
    eventQueueTop--;
    if (isLoc(eventQueue[eventQueueTop].where))
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
}





/*----------------------------------------------------------------------*\

  Main program and initialisation

  codfil
  filenames

  checkvers()
  load()
  checkdebug()
  initheader()
  initstrings()
  start()
  init()

\*----------------------------------------------------------------------*/


static FILE *codfil;
static char codfnm[256] = "";
static char txtfnm[256] = "";
static char logfnm[256] = "";


/*----------------------------------------------------------------------

  checkvers()

 */
static void checkvers(AcdHdr *header)
{
  char vers[4];
  char state[2];
  Abool developmentVersion;
  int compareLength;

  /* Construct our own version */
  vers[0] = alan.version.version;
  vers[1] = alan.version.revision;
  vers[2] = alan.version.correction;
  vers[3] = alan.version.state[0];

  /* Check version of .ACD file */
  if (debugOption) {
    state[0] = header->vers[3];
    state[1] = '\0';
    printf("<Version of '%s' is %d.%d(%d)%s>",
	   adventureName,
	   (int)(header->vers[0]),
	   (int)(header->vers[1]),
	   (int)(header->vers[2]),
	   (header->vers[3])==0? "": state);
    newline();
  }

  /* Development version require exact match, else only 2 digit match */
  developmentVersion = (strcmp(alan.version.state, "development") == 0);
  compareLength = (developmentVersion? 3 : 2);

  /* Compatible if version and revision match... */
  if (memcmp(header->vers, vers, compareLength) != 0) {
      if (ignoreErrorOption) {
	char str[80];
	if (developmentVersion)
	  sprintf(str, "Incompatible version of ACODE program. Development versions always require exact match. Game is %ld.%ld.%ld, interpreter %ld.%ld.%ld!",
		  (long)(header->vers[0]),
		  (long)(header->vers[1]),
		  (long)(header->vers[2]),
		  (long)alan.version.version,
		  (long)alan.version.revision,
		  (long)alan.version.correction);
	else
	  sprintf(str, "Incompatible version of ACODE program. Game is %ld.%ld, interpreter %ld.%ld.",
		  (long)(header->vers[0]),
		  (long)(header->vers[1]),
		  (long)alan.version.version,
		  (long)alan.version.revision);
	syserr(str);
      } else
	output("<WARNING! Incompatible version of ACODE program.>\n");
  }
}


/*----------------------------------------------------------------------*/
static void load(void)
{
  AcdHdr tmphdr;
  Aword crc = 0;
  int i;
  char err[100];

  rewind(codfil);
  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  rewind(codfil);
  if (strncmp((char *)&tmphdr, "ALAN", 4) != 0)
    syserr("Not an Alan game file, does not start with \"ALAN\"");
  checkvers(&tmphdr);

  /* Allocate and load memory */

  if (littleEndian())
    reverseHdr(&tmphdr);

  /* No memory allocated yet? */
  if (memory == NULL) {
    memory = allocate(tmphdr.size*sizeof(Aword));
  }
  header = (AcdHdr *) pointerTo(0);

  memTop = fread(pointerTo(0), sizeof(Aword), tmphdr.size, codfil);
  if (memTop != tmphdr.size)
    syserr("Could not read all ACD code.");

  /* Calculate checksum */
  for (i = sizeof(tmphdr)/sizeof(Aword); i < memTop; i++) {
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
    if (ignoreErrorOption)
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
static void checkdebug(void)
{
  /* Make sure he can't debug if not allowed! */
  if (!header->debug) {
    if (debugOption|sectionTraceOption|singleStepOption) {
      printf("<Sorry, '%s' is not compiled for debug!>\n", adventureName);
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
  for (dictsize = 0; !endOfTable(&dictionary[dictsize]); dictsize++);

  /* Scores */
  

  /* All addresses to tables indexed by ids are converted to pointers,
     then adjusted to point to the (imaginary) element before the
     actual table so that [0] does not exist. Instead indices goes
     from 1 and we can use [1]. */

  if (header->instanceTableAddress == 0)
    syserr("Instance table pointer == 0");
  instance = (InstanceEntry *) pointerTo(header->instanceTableAddress);
  instance--;			/* Back up one so that first is no. 1 */


  if (header->classTableAddress == 0)
    syserr("Class table pointer == 0");
  class = (ClassEntry *) pointerTo(header->classTableAddress);
  class--;			/* Back up one so that first is no. 1 */

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
    scores = duplicate((Aword *) pointerTo(header->scores), header->scoresMax*sizeof(Aword));
  else
    memcpy(scores, pointerTo(header->scores),
	   header->scoresMax*sizeof(Aword));


  stxs = (ParseEntry *) pointerTo(header->parseTableAddress);
  vrbs = (VerbEntry *) pointerTo(header->verbTableAddress);
  ruls = (RulEntry *) pointerTo(header->ruleTableAddress);
  msgs = (MsgEntry *) pointerTo(header->messageTableAddress);

  if (header->pack)
    freq = (Aword *) pointerTo(header->freq);
}


/*----------------------------------------------------------------------*/
static void initStrings(void)
{
  StringInitEntry *init;
  AttributeEntry *attribute;

  for (init = (StringInitEntry *) pointerTo(header->stringInitTable); !endOfTable(init); init++) {
    getStringFromFile(init->fpos, init->len);
    attribute = pointerTo(init->adr);
    attribute->value = pop();
  }
}

/*----------------------------------------------------------------------*/
static Aint sizeOfAttributeData(void)
{
  int i;
  int size = 0;

  for (i=1; i<=header->instanceMax; i++) {
    AttributeEntry *attribute = pointerTo(instance[i].initialAttributes);
    while (*((Aword *)attribute) != EOF) {
      size++;
      attribute++;
    }
    size++;
  }

  if (size != header->attributesAreaSize)
    syserr("Attribute area size calculated wrong.");
  return size;
}


/*----------------------------------------------------------------------*/
static AttributeEntry *copyAttributes(void)
{
  AttributeEntry *attributeArea = allocate(sizeOfAttributeData()*sizeof(AttributeEntry));
  AttributeEntry *currentAttributeArea = attributeArea;  
  int i;

  for (i=1; i<=header->instanceMax; i++) {
    AttributeEntry *originalAttribute = pointerTo(instance[i].initialAttributes);
    admin[i].attributes = currentAttributeArea;
    while (*((Aword *)originalAttribute) != EOF) {
      *currentAttributeArea = *originalAttribute;
      currentAttributeArea++;
      originalAttribute++;
    }
    *currentAttributeArea = *originalAttribute;
  }

  return attributeArea;
}




/*----------------------------------------------------------------------*/
static void initDynamicData(void)
{
  int instanceId;

  /* Allocate for administrative table */
  admin = (AdminEntry *)allocate((header->instanceMax+1)*sizeof(AdminEntry));

  /* Initialise string & set attributes */
  initStrings();
  initSets((SetInitEntry*)pointerTo(header->setInitTable));

  /* Create game state copy of attributes */
  attributes = copyAttributes();

  /* Set initial locations */
  for (instanceId = 1; instanceId <= header->instanceMax; instanceId++)
    admin[instanceId].location = instance[instanceId].initialLocation;
}


/*----------------------------------------------------------------------*/
static void start(void)
{
  int startloc;

  current.tick = -1;
  current.location = startloc = where(HERO, FALSE);
  current.actor = HERO;
  current.score = 0;

  if (sectionTraceOption)
    printf("\n<START:>\n");
  interpret(header->start);
  para();

  locate(HERO, startloc);
}



/*----------------------------------------------------------------------*/
static void openFiles(void)
{
  char str[256];
  char *usr = "";
  time_t tick;

  /* Open Acode file */
  strcpy(codfnm, adventureName);
  strcat(codfnm, ".a3c");
  if ((codfil = fopen(codfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure code file '");
    strcat(str, codfnm);
    strcat(str, "'.");
    syserr(str);
  }

  /* Open Text file */
  strcpy(txtfnm, adventureName);
  strcat(txtfnm, ".a3c");
  if ((textFile = fopen(txtfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure text data file '");
    strcat(str, txtfnm);
    strcat(str, "'.");
    syserr(str);
  }

  /* If logging open log file */
  if (transcriptOption || logOption) {
    char *namstart;

    if((namstart = strrchr(adventureName, ']')) == NULL
       && (namstart = strrchr(adventureName, '>')) == NULL
       && (namstart = strrchr(adventureName, '/')) == NULL
       && (namstart = strrchr(adventureName, '\\')) == NULL
       && (namstart = strrchr(adventureName, ':')) == NULL)
      namstart = &adventureName[0];
    else
      namstart++;

    time(&tick);
    sprintf(logfnm, "%s%d%s.log", namstart, (int)tick, usr);
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

  /* Initialise some status */
  eventQueueTop = 0;			/* No pending events */
  if (eventQueue == NULL)	/* Make sure there is an event queue */
    increaseEventQueue();

  dscrstkp = 0;			/* No describe in progress */

  if (debugOption||sectionTraceOption||singleStepOption) {
    char str[80];
    output("<Hi! This is Alan interactive fiction interpreter Arun,");
    sprintf(str, "%s version %ld.%ld.%ld!>$n", alan.version.state,
	    (long)alan.version.version,
	    (long)alan.version.revision,
	    (long)alan.version.correction);
    output(str);
  }

  initStaticData();
  initDynamicData();
  initParse();
  checkdebug();

  getPageSize();

  /* Find first conjunction and use that for ',' handling */
  for (i = 0; i < dictsize; i++)
    if (isConj(i)) {
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
      parse();
      capitalize = TRUE;
      fail = FALSE;			/* fail only aborts one actor */
    }
  } else if (admin[theActor].script != 0) {
    for (scr = (ScriptEntry *) pointerTo(header->scriptTableAddress); !endOfTable(scr); scr++) {
      if (scr->code == admin[theActor].script) {
	/* Find correct step in the list by indexing */
	step = (StepEntry *) pointerTo(scr->steps);
	step = (StepEntry *) &step[admin[theActor].step];
	/* Now execute it, maybe. First check wait count */
	if (step->after > admin[theActor].waitCount) { /* Wait some more ? */
	  if (traceActor(theActor))
	    printf("), SCRIPT %s(%ld), STEP %ld, Waiting another %ld turns>\n",
		   scriptName(theActor, admin[theActor].script),
		   admin[theActor].script, admin[theActor].step+1,
		   step->after-admin[theActor].waitCount);
	  admin[theActor].waitCount++;
	  break;
	} else
	  admin[theActor].waitCount = 0;
	/* Then check possible expression to wait for */
	if (step->exp != 0) {
	  if (traceActor(theActor))
	    printf("), SCRIPT %s(%ld), STEP %ld, Evaluating:>\n",
		   scriptName(theActor, admin[theActor].script),
		   admin[theActor].script, admin[theActor].step+1);
	  interpret(step->exp);
	  if (!(Abool)pop())
	    break;		/* Break loop, don't execute step*/
	}
	/* OK, so finally let him do his thing */
	admin[theActor].step++;		/* Increment step number before executing... */
	if (traceActor(theActor))
	  printf("), SCRIPT %s(%ld), STEP %ld, Executing:>\n",
		 scriptName(theActor, admin[theActor].script),
		 admin[theActor].script, admin[theActor].step);
	interpret(step->stm);
	step++;
	/* ... so that we can see if he is USEing another script now */
	if (admin[theActor].step != 0 && endOfTable(step))
	  /* No more steps in this script, so stop him */
	  admin[theActor].script = 0;
	fail = FALSE;			/* fail only aborts one actor */
	break;			/* We have executed a script so leave loop */
      }
    }
    if (endOfTable(scr))
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

  openFiles();
  load();			/* Load program */

  setjmp(restartLabel);	/* Return here if he wanted to restart */

  if (setjmp(errorLabel) == 0)
    init();			/* Initialise and start the adventure */

  while (TRUE) {
#ifdef MALLOC
    if (malloc_verify() == 0) syserr("Error in heap.");
#endif
    if (debugOption)
      debug(FALSE, 0, 0);

    pushGameState();

    eventCheck();
    current.tick++;

    (void) setjmp(errorLabel);	/* Return here if any error during execution */
    recursions = 0;

    /* Move all characters, hero first */
    moveActor(header->theHero);
    rules();
    for (i = 1; i <= header->instanceMax; i++)
      if (i != header->theHero && isA(i, ACTOR)) {
	moveActor(i);
	rules();
      }
  }
}

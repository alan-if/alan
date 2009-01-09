#include "output.h"

/* IMPORTS */
#include "options.h"
#include "memory.h"
#include "word.h"
#include "lists.h"
#include "term.h"
#include "syserr.h"
#include "dictionary.h"
#include "current.h"
#include "inter.h"
#include "params.h"
#include "msg.h"
#include "readline.h"
// TODO Remove dependency on exe.h, returnLable
#include "exe.h"


/* PUBLIC DATA */
Bool anyOutput = FALSE;
Bool capitalize = FALSE;
Bool needSpace = FALSE;
Bool skipSpace = FALSE;

/* Screen formatting info */
int col, lin;
int pageLength, pageWidth;

/* Logfile */
#ifdef HAVE_GLK
strid_t logFile;
#else
FILE *logFile;
#endif


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


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


static Bool onStatusLine = FALSE; /* Don't log when printing status */
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
      stringCopy = strdup(string);  /* Make sure we can write NULLs */
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
    if (i == 0 && col == 1) /* If it doesn't fit at all */
      /* Wrap immediately after this word */
      while (!isSpace(str[i]) && str[i] != '\0')
    i++;
    if (i > 0) {        /* If it fits ... */
      ch = str[i];      /* Save space or NULL */
      str[i] = '\0';        /* Terminate string */
      printAndLog(str);     /* and print it */
      skipSpace = FALSE;        /* If skipping, now we're done */
      str[i] = ch;      /* Restore character */
      /* Skip white after printed portion */
      for (str = &str[i]; isSpace(str[0]) && str[0] != '\0'; str++);
    }
    newline();          /* Then start a new line */
    while(isSpace(str[0])) str++; /* Skip any leading space on next part */
  }
  printAndLog(str);     /* Print tail */
#endif
  col = col + strlen(str);  /* Update column */
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
static char *printSymbol(char *str) /* IN - The string starting with '$' */
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
    needSpace = TRUE;       /* We did print something non-white */
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
    needSpace = TRUE;       /* We did print something non-white */
    break;
  case 'l':
    space();
    say(current.location);
    needSpace = TRUE;       /* We did print something non-white */
    break;
  case 'a':
    space();
    say(current.actor);
    needSpace = TRUE;       /* We did print something non-white */
    break;
  case 'v':
    space();
    justify((char *)pointerTo(dictionary[verbWord].string));
    needSpace = TRUE;       /* We did print something non-white */
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
    space();            /* Output space if needed (& not inhibited) */

  /* Output string up to symbol and handle the symbol */
  while ((symptr = strchr(str, '$')) != (char *) NULL) {
    ch = *symptr;       /* Terminate before symbol */
    *symptr = '\0';
    if (strlen(str) > 0) {
      skipSpace = FALSE;    /* Only let skipSpace through if it is
                   last in the string */
      if (lastCharOf(str) == ' ') {
    str[strlen(str)-1] = '\0'; /* Truncate space character */
    justify(str);       /* Output part before '$' */
    needSpace = TRUE;
      } else {
    justify(str);       /* Output part before '$' */
    needSpace = FALSE;
      }
    }
    *symptr = ch;       /* restore '$' */
    str = printSymbol(symptr);  /* Print the symbolic reference and advance */
  }

  if (str[0] != 0) {
    justify(str);           /* Output trailing part */
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
Bool confirm(MsgKind msgno)
{
    char buf[80];

    /* This is a bit of a hack since we really want to compare the input,
     it could be affirmative, but for now any input is NOT! */
    printMessage(msgno);

#ifdef USE_READLINE
    if (!readline(buf)) return TRUE;
#else
    if (gets(buf) == NULL) return TRUE;
#endif
    col = 1;

    return (buf[0] == '\0');
}


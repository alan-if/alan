/*----------------------------------------------------------------------*\

  MAIN.C

  Main module of interpreter for ALAN Adventure Language

\*----------------------------------------------------------------------*/

#include "sysdep.h"

#include "types.h"
#include "main.h"

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
#ifdef REVERSED
#include "reverse.h"
#endif
#include "debug.h"
#include "stack.h"
#include "exe.h"
#include "term.h"

#ifdef GLK
#include "glk.h"
#include "glkio.h"
#endif

/* PUBLIC DATA */

/* The Amachine memory */
Aword *memory;
static AcdHdr dummyHeader;	/* Dummy to use until memory allocated */
AcdHdr *header = &dummyHeader;

int memTop = 0;			/* Top of load memory */

int conjWord;			/* First conjunction in dictonary, for ',' */


/* Amachine variables */
CurVars current;

/* Amachine structures */
InstanceEntry *instance;	/* Instance table pointer */
ClassEntry *class;		/* Class table pointer */
ContainerEntry *container;	/* Container table pointer */

AdminEntry *admin;		/* Administrative data about instances */
WrdEntry *dict;			/* Dictionary pointer */
LocEntry *locs;			/* Location table pointer */
VrbEntry *vrbs;			/* Verb table pointer */
StxEntry *stxs;			/* Syntax table pointer */
RulEntry *ruls;			/* Rule table pointer */
EventEntry *events;		/* Event table pointer */
MsgEntry *msgs;			/* Message table pointer */
Aword *scores;			/* Score table pointer */
Aword *freq;			/* Cumulative character frequencies */

int dictsize;

Boolean verbose = FALSE;
Boolean errflg = TRUE;
Boolean trcflg = FALSE;
Boolean dbgflg = FALSE;
Boolean stepFlag = FALSE;
Boolean logflg = FALSE;
Boolean statusflg = TRUE;
Boolean regressionflg = FALSE;
Boolean fail = FALSE;
Boolean anyOutput = FALSE;


/* The files and filenames */
char *advnam;
FILE *txtfil;
FILE *logfil;


/* Screen formatting info */
int col, lin;
int pageLength, pageWidth;

Boolean needsp = FALSE;
Boolean skipsp = FALSE;

/* Restart jump buffer */
jmp_buf restart_label;


/* PRIVATE DATA */
static jmp_buf jmpbuf;		/* Error return long jump buffer */



/*======================================================================

  terminate()

  Terminate the execution of the adventure, e.g. close windows,
  return buffers...

 */
#ifdef _PROTOTYPES_
void terminate(int code)
#else
void terminate(code)
     int code;
#endif
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
  if (logflg)
    fclose(logfil);

#ifdef __MWERKS__
  printf("Command-Q to close window.");
#endif

#ifdef GLK
  glk_exit();
#else
  exit(code);
#endif
}

/*======================================================================

  usage()

  */
#ifdef _PROTOTYPES_
void usage(void)
#else
void usage()
#endif
{
  printf("\nArun, Adventure Interpreter version %s (%s %s)\n\n",
	 alan.version.string, alan.date, alan.time);
  printf("Usage:\n\n");
  printf("    %s [<switches>] <adventure>\n\n", PROGNAME);
  printf("where the possible optional switches are:\n");
#ifdef GLK
  glk_set_style(style_Preformatted);
#endif
  printf("    -v    verbose mode\n");
  printf("    -l    log player commands and game output to a file\n");
  printf("    -n    no Status Line\n");
  printf("    -d    enter debug mode\n");
  printf("    -t    trace game execution\n");
  printf("    -s    single instruction trace\n");
  printf("    -i    ignore version and checksum errors\n");
  printf("    -r    refrain from printing timestamps (making regression testing easier)\n");
#ifdef GLK
  glk_set_style(style_Normal);
#endif
}


/*======================================================================

  syserr()

  Print a little text blaming the user for the system error.

 */
#ifdef _PROTOTYPES_
void syserr(char *str)
#else
void syserr(str)
     char *str;
#endif
{
  output("$n$nAs you enter the twilight zone of Adventures, you stumble \
and fall to your knees. In front of you, you can vaguely see the outlines \
of an Adventure that never was.$n$nSYSTEM ERROR: ");
  output(str);
  output("$n$n");

  if (logflg)
    fclose(logfil);
  newline();

#ifdef __amiga__
#ifdef AZTEC_C
  {
    char buf[80];

    if (con) { /* Running from WB, wait for user ack. */
      printf("press RETURN to quit");
      gets(buf);
    }
  }
#endif
#endif

  terminate(0);
}


/*======================================================================

  error()

  Print an error message, force new player input and abort.

  */
#ifdef _PROTOTYPES_
void error(MsgKind msgno)	/* IN - The error message number */
#else
void error(msgno)
     MsgKind msgno;		/* IN - The error message number */
#endif
{
  if (msgno != MSGMAX)
    prmsg(msgno);
  wrds[wrdidx] = EOF;		/* Force new player input */
  dscrstkp = 0;			/* Reset describe stack */
  longjmp(jmpbuf,TRUE);
}


/*======================================================================

  statusline()

  Print the the status line on the top of the screen.

  */
void statusline(void)
{
#ifdef GLK
  glui32 glkWidth;
  char line[100];
  int pcol = col;

  if (NULL == glkStatusWin)
    return;

  glk_set_window(glkStatusWin);
  glk_window_clear(glkStatusWin);
  glk_window_get_size(glkStatusWin, &glkWidth, NULL);

  col = 1;
  glk_window_move_cursor(glkStatusWin, 1, 0);
  interpret(instance[where(HERO)].mentioned);

  if (header->maxscore > 0)
    sprintf(line, "Score %d(%d)/%d moves", cur.score, (int)header->maxscore, cur.tick);
  else
    sprintf(line, "%d moves", cur.tick);
  glk_window_move_cursor(glkStatusWin, glkWidth - col - strlen(line), 0);
  printf(line);
  needsp = FALSE;

  col = pcol;

  glk_set_window(glkMainWin); 
#else
#ifdef HAVE_ANSI
  char line[100];
  int i;
  int pcol = col;

  if (!statusflg) return;
  /* ansi_position(1,1); ansi_bold_on(); */
  printf("\x1b[1;1H");
  printf("\x1b[7m");

  col = 1;
  interpret(instance[where(HERO)].mentioned);

  if (header->maxscore > 0)
    sprintf(line, "Score %d(%ld)/%d moves", current.score, header->maxscore, current.tick);
  else
    sprintf(line, "%ld moves", (long)current.tick);
  for (i=0; i < pageWidth - col - strlen(line); i++) putchar(' ');
  printf(line);
  printf("\x1b[m");
  printf("\x1b[%d;1H", pageLength);
  needsp = FALSE;

  col = pcol;
#endif
#endif
}


/*======================================================================

  logprint()

  Print some text and log it if logging is on.

 */
void logprint(char str[])
{
  printf(str);
  if (logflg)
    fprintf(logfil, str);
}



/*======================================================================

  newline()

  Make a newline, but check for screen full.

 */
#ifdef _PROTOTYPES_
void newline(void)
#else
void newline()
#endif
{
#ifdef GLK
  glk_put_char('\n');
  needsp = FALSE;
#else
  char buf[256];
  
  col = 1;
  if (lin >= pageLength - 1) {
    logprint("\n");
    needsp = FALSE;
    prmsg(M_MORE);
#ifdef USE_READLINE
    (void) readline(buf);
#else
    fgets(buf, 256, stdin);
#endif
    getPageSize();
    lin = 0;
  } else
    logprint("\n");
  
  lin++;
  needsp = FALSE;
#endif
}


/*======================================================================

  para()

  Make a new paragraph, i.e one empty line (one or two newlines).

 */
#ifdef _PROTOTYPES_
void para(void)
#else
void para()
#endif
{
  if (col != 1)
    newline();
  newline();
}


/*======================================================================

  clear()

  Clear the screen.

 */
#ifdef _PROTOTYPES_
void clear(void)
#else
void clear()
#endif
{
#ifdef GLK
  glk_window_clear(glkMainWin);
#else
#ifdef HAVE_ANSI
  if (!statusflg) return;
  printf("\x1b[2J");
  printf("\x1b[%d;1H", pageLength);
#endif
#endif
}


/*======================================================================

  allocate()

  Safely allocate new memory.

*/
#ifdef _PROTOTYPES_
void *allocate(unsigned long len)		/* IN - Length to allocate */
#else
void *allocate(len)
     unsigned long len;			/* IN - Length to allocate */
#endif
{
  void *p = (void *)calloc((size_t)len, 1);

  if (p == NULL)
    syserr("Out of memory.");

  return p;
}


/*----------------------------------------------------------------------

  just()

  Justify a string so that it wraps at end of screen.

 */
#ifdef _PROTOTYPES_
static void just(char str[])
#else
static void just(str)
     char str[];
#endif
{
#ifdef GLK
  logprint(str);
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
      logprint(str);		/* and print it */
      skipsp = FALSE;		/* If skipping, now we're done */
      str[i] = ch;		/* Restore character */
      /* Skip white after printed portion */
      for (str = &str[i]; isSpace(str[0]) && str[0] != '\0'; str++);
    }
    newline();			/* Then start a new line */
  }
  logprint(str);			/* Print tail */
  col = col + strlen(str);	/* Update column */
#endif
}


/*----------------------------------------------------------------------

  space()

  Output a space if needed.

 */
#ifdef _PROTOTYPES_
static void space(void)
#else
static void space()
#endif
{
  if (skipsp)
    skipsp = FALSE;
  else {
    if (needsp) {
      logprint(" ");
      col++;
    }
  }
  needsp = FALSE;
}



/*----------------------------------------------------------------------

  sayparam()

  A parameter needs to be said, check for words the player used and use
  them if possible.

*/
#ifdef _PROTOTYPES_
static void sayparam(int p)
#else
static void sayparam(p)
     int p;
#endif
{
  int i;

  for (i = 0; i <= p; i++)
    if (params[i].code == EOF)
      syserr("Nonexistent parameter referenced.");

  if (params[p].firstWord == EOF) /* Any words he used? */
    say(params[p].code);
  else				/* Yes, so use them... */
    for (i = params[p].firstWord; i <= params[p].lastWord; i++) {
      just((char *)addrTo(dict[wrds[i]].wrd));
      if (i < params[p].lastWord)
	just(" ");
    }
}


/*----------------------------------------------------------------------

  prsym()

  Print an expanded symbolic reference.

  N = newline
  I = indent on a new line
  P = new paragraph
  L = current location name
  O = current object -> first parameter!
  V = current verb
  A = current actor
  T = tabulation
  $ = no space needed after this
 */
#ifdef _PROTOTYPES_
static void prsym(
		  char *str	/* IN - The string starting with '$' */
)
#else
static void prsym(str)
     char *str;			/* IN - The string starting with '$' */
#endif
{
  switch (toLower(str[1])) {
  case 'n':
    newline();
    needsp = FALSE;
    break;
  case 'i':
    newline();
    logprint("    ");
    col = 5;
    needsp = FALSE;
    break;
  case 'o':
    sayparam(0);
    needsp = TRUE;		/* We did print something non-white */
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
    sayparam(str[1]-'1');
    needsp = TRUE;		/* We did print something non-white */
    break;
  case 'l':
    say(current.location);
    needsp = TRUE;		/* We did print something non-white */
    break;
  case 'a':
    say(current.actor);
    needsp = TRUE;		/* We did print something non-white */
    break;
  case 'v':
    just((char *)addrTo(dict[vrbwrd].wrd));
    needsp = TRUE;		/* We did print something non-white */
    break;
  case 'p':
    para();
    needsp = FALSE;
    break;
  case 't': {
    int i;
    int spaces = 4-(col-1)%4;
    
    for (i = 0; i<spaces; i++) logprint(" ");
    col = col + spaces;
    needsp = FALSE;
    break;
  }
  case '$':
    skipsp = TRUE;
    break;
  default:
    logprint("$");
    break;
  }
}



/*======================================================================

  output()

  Output a string to suit the screen. Any symbolic inserts ('$') are
  recogniced and performed.

 */
#ifdef _PROTOTYPES_
void output(char original[])
#else
void output(original)
     char original[];
#endif
{
  char ch;
  char *str, *copy;
  char *symptr;

  copy = strdup(original);
  str = copy;

  if (str[0] != '$' || str[1] != '$')
    space();			/* Output space if needed (& not inhibited) */

  while ((symptr = strchr(str, '$')) != (char *) NULL) {
    ch = *symptr;		/* Terminate before symbol */
    *symptr = '\0';
    if (strlen(str) > 0) {
      just(str);		/* Output part before '$' */
      if (str[strlen(str)-1] == ' ')
	needsp = FALSE;
    }
    *symptr = ch;		/* restore '$' */
    prsym(symptr);		/* Print the symbolic reference */
    str = &symptr[2];		/* Advance to after symbol and continue */
  }
  if (str[0] != 0) {
    just(str);			/* Output trailing part */
    skipsp = FALSE;
    if (str[strlen(str)-1] != ' ')
      needsp = TRUE;
  }
  anyOutput = TRUE;
  free(copy);
}


/*======================================================================

  prmsg()

  Print a message from the message table.
  
  */
#ifdef _PROTOTYPES_
void prmsg(MsgKind msg)		/* IN - message number */
#else
void prmsg(msg)
     MsgKind msg;		/* IN - message number */
#endif
{
  interpret(msgs[msg].stms);
}


/*----------------------------------------------------------------------*\

  Various check functions

  endOfTable()
  isObj, isLoc, isAct, IsCnt & isNum

\*----------------------------------------------------------------------*/

/* How to know we are at end of a table */
#ifdef _PROTOTYPES_
Boolean eot(Aword *adr)
#else
Boolean eot(adr)
     Aword *adr;
#endif
{
  return *adr == EOF;
}


#ifdef _PROTOTYPES_
Boolean isObj(Aword x)
#else
Boolean isObj(x)
     Aword x;
#endif
{
  return isA(x, OBJECT);
}

#ifdef _PROTOTYPES_
Boolean isCnt(Aword x)
#else
Boolean isCnt(x)
     Aword x;
#endif
{
  return x != 0 && instance[x].container != 0;
}

#ifdef _PROTOTYPES_
Boolean isAct(Aword x)
#else
Boolean isAct(x)
     Aword x;
#endif
{
  return isA(x, ACTOR);
}

#ifdef _PROTOTYPES_
Boolean isLoc(Aword x)
#else
Boolean isLoc(x)
     Aword x;
#endif
{
  return isA(x, LOCATION);
}


#ifdef _PROTOTYPES_
Boolean isLit(Aword x)
#else
Boolean isLit(x)
     Aword x;
#endif
{
  return x > header->instanceMax;
}

#ifdef _PROTOTYPES_
Boolean isNum(Aword x)
#else
Boolean isNum(x)
     Aword x;
#endif
{
  return isLit(x) && literal[x].type == NUMERIC_LITERAL;
}

#ifdef _PROTOTYPES_
Boolean isStr(Aword x)
#else
Boolean isStr(x)
     Aword x;
#endif
{
  return isLit(x) && literal[x].type == STRING_LITERAL;
}



/*======================================================================

  exitto()

  Is there an exit from one location to another?

  */
#ifdef _PROTOTYPES_
Boolean exitto(int to, int from)
#else
Boolean exitto(to, from)
     int to, from;
#endif
{
  ExitEntry *theExit;

  if (instance[from].exits == 0)
    return(FALSE); /* No exits */

  for (theExit = (ExitEntry *) addrTo(instance[from].exits); !endOfTable(theExit); theExit++)
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




/*----------------------------------------------------------------------
  count()

  Count the number of items in a container.

  */
#ifdef _PROTOTYPES_
static int count(int cnt)	/* IN - the container to count */
#else
static int count(cnt)
     int cnt;			/* IN - the container to count */
#endif
{
  int i, j = 0;
  
  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt))
      /* Then it's in this container also */
      j++;
  return(j);
}



/*----------------------------------------------------------------------
  sumatr()

  Sum the values of one attribute in a container. Recursively.

  */
#ifdef _PROTOTYPES_
static int sumatr(
     Aword atr,			/* IN - the attribute to sum over */
     Aword cnt			/* IN - the container to sum */
)
#else
static int sumatr(atr, cnt)
     Aword atr;			/* IN - the attribute to sum over */
     Aword cnt;			/* IN - the container to sum */
#endif
{
  int i;
  int sum = 0;

  for (i = 1; i <= header->instanceMax; i++)
    if (isObj(i)) {
      if (instance[i].location == cnt) { /* Then it's in this container */
	if (instance[i].container != 0)	/* This is also a container! */
	  sum = sum + sumatr(atr, i);
	sum = sum + attribute(i, atr);
      }
    }
  return(sum);
}




/*======================================================================
  checklim()

  Checks if a limit for a container is exceeded.

  */
#ifdef _PROTOTYPES_
Boolean checklim(
     Aword cnt,			/* IN - Container code */
     Aword obj			/* IN - The object to add */
)
#else
Boolean checklim(cnt, obj)
     Aword cnt;			/* IN - Container code */
     Aword obj;			/* IN - The object to add */
#endif
{
  LimEntry *lim;
  Aword props;

  fail = TRUE;
  if (!isCnt(cnt))
    syserr("Checking limits for a non-container.");

  /* Find the container properties */
  props = instance[cnt].container;

  if (container[props].limits != 0) { /* Any limits at all? */
    for (lim = (LimEntry *) addrTo(container[props].limits); !endOfTable(lim); lim++)
      if (lim->atr == I_COUNT) {
	if (count(cnt) >= lim->val) {
	  interpret(lim->stms);
	  return(TRUE);		/* Limit check failed */
	}
      } else {
	if (sumatr(lim->atr, cnt) + attribute(obj, lim->atr) > lim->val) {
	  interpret(lim->stms);
	  return(TRUE);
	}
      }
  }
  fail = FALSE;
  return(FALSE);
}





/*----------------------------------------------------------------------*\

  Action routines

\*----------------------------------------------------------------------*/



/*======================================================================
  trycheck()

  Tries a check, returns TRUE if it passed, FALSE else.

  */
#ifdef _PROTOTYPES_
Boolean trycheck(
     Aaddr adr,			/* IN - ACODE address to check table */
     Boolean act		/* IN - Act if it fails ? */
)
#else
Boolean trycheck(adr, act)
     Aaddr adr;			/* IN - ACODE address to check table */
     Boolean act;		/* IN - Act if it fails ? */
#endif
{
  ChkEntry *chk;

  chk = (ChkEntry *) addrTo(adr);
  if (chk->exp == 0) {
    interpret(chk->stms);
    return(FALSE);
  } else {
    while (!endOfTable(chk)) {
      interpret(chk->exp);
      if (!(Abool)pop()) {
	if (act)
	  interpret(chk->stms);
	return(FALSE);
      }
      chk++;
    }
    return(TRUE);
  }
}


/*======================================================================
  go()

  Move hero in a direction.

  */
#ifdef _PROTOTYPES_
void go(int dir)
#else
void go(dir)
     int dir;
#endif
{
  ExitEntry *theExit;
  Boolean ok;
  Aword oldloc;

  theExit = (ExitEntry *) addrTo(instance[current.location].exits);
  if (instance[current.location].exits != 0)
    while (!endOfTable(theExit)) {
      if (theExit->code == dir) {
	ok = TRUE;
	if (theExit->checks != 0) {
	  if (trcflg) {
	    printf("\n<EXIT %d (%s) from %d (", dir,
		   (char *)addrTo(dict[wrds[wrdidx-1]].wrd), current.location);
	    debugsay(current.location);
	    printf("), Checking:>\n");
	  }
	  ok = trycheck(theExit->checks, TRUE);
	}
	if (ok) {
	  oldloc = current.location;
	  if (theExit->action != 0) {
	    if (trcflg) {
	      printf("\n<EXIT %d (%s) from %d (", dir, 
		     (char *)addrTo(dict[wrds[wrdidx-1]].wrd), current.location);
	      debugsay(current.location);
	      printf("), Executing:>\n");
	    }	    
	    interpret(theExit->action);
	  }
	  /* Still at the same place? */
	  if (where(HERO) == oldloc) {
	    if (trcflg) {
	      printf("\n<EXIT %d (%s) from %d (", dir, 
		     (char *)addrTo(dict[wrds[wrdidx-1]].wrd), current.location);
	      debugsay(current.location);
	      printf("), Moving:>\n");
	    }
	    locate(HERO, theExit->target);
	  }
	}
	return;
      }
      theExit++;
    }
  error(M_NO_WAY);
}



/*----------------------------------------------------------------------

  findalt()

  Find the verb alternative wanted in a verb list and return
  the address to it.

 */
#ifdef _PROTOTYPES_
static AltEntry *findalt(
     Aword vrbsadr,		/* IN - Address to start of list */
     Aword param		/* IN - Which parameter to match */
)
#else
static AltEntry *findalt(vrbsadr, param)
     Aword vrbsadr;		/* IN - Address to start of list */
     Aword param;		/* IN - Which parameter to match */
#endif
{
  VrbEntry *vrb;
  AltEntry *alt;

  if (vrbsadr == 0)
    return(NULL);

  for (vrb = (VrbEntry *) addrTo(vrbsadr); !endOfTable(vrb); vrb++)
    if (vrb->code == current.verb) {
      for (alt = (AltEntry *) addrTo(vrb->alts); !endOfTable(alt); alt++)
	if (alt->param == param || alt->param == 0)
	  return alt;
      return NULL;
    }
  return NULL;
}




/*======================================================================

  possible()

  Check if current action is possible according to the CHECKs.

  */
#ifdef _PROTOTYPES_
Boolean possible(void)
#else
Boolean possible()
#endif
{
  AltEntry *alt[MAXPARAMS+2];	/* List of alt-pointers, one for each param */
  int i;			/* Parameter index */
  
  fail = FALSE;
  alt[0] = findalt(header->verbTableAddress, 0);
  /* Perform global checks */
  if (alt[0] != 0 && alt[0]->checks != 0) {
    if (!trycheck(alt[0]->checks, FALSE)) return FALSE;
    if (fail) return FALSE;
  }
  
  /* Now CHECKs in this location */
  alt[1] = findalt(instance[current.location].verbs, 0);
  if (alt[1] != 0 && alt[1]->checks != 0)
    if (!trycheck(alt[1]->checks, FALSE))
      return FALSE;
  
  for (i = 0; params[i].code != EOF; i++) {
    alt[i+2] = findalt(instance[params[i].code].verbs, i+1);
    /* CHECKs in a possible parameter */
    if (alt[i+2] != 0 && alt[i+2]->checks != 0)
      if (!trycheck(alt[i+2]->checks, FALSE))
	return FALSE;
  }

  for (i = 0; i < 2 || params[i-2].code != EOF; i++)
    if (alt[i] != 0 && alt[i]->action != 0)
      break;
  if (i >= 2 && params[i-2].code == EOF)
    /* Didn't find any code for this verb/object combination */
    return FALSE;
  else
    return TRUE;
}



/*----------------------------------------------------------------------

  do_it()

  Execute the action commanded by hero.

  */
#ifdef _PROTOTYPES_
static void do_it(void)
#else
static void do_it()
#endif
{
  AltEntry *alt[MAXPARAMS+2];	/* List of alt-pointers, one for each param */
  Boolean done[MAXPARAMS+2];	/* Is it done */
  int i;			/* Parameter index */
  char trace[80];		/* Trace string buffer */

  fail = FALSE;
  alt[0] = findalt(header->verbTableAddress, 0);
  /* Perform global checks */
  if (alt[0] != 0 && alt[0]->checks != 0) {
    if (trcflg)
      printf("\n<VERB %d, CHECK, GLOBAL:>\n", current.verb);
    if (!trycheck(alt[0]->checks, TRUE)) return;
    if (fail) return;
  }

  /* Now CHECKs in this location */
  current.instance = current.location;
  alt[1] = findalt(instance[current.location].verbs, 0);
  if (alt[1] != 0 && alt[1]->checks != 0) {
    if (trcflg)
      printf("\n<VERB %d, CHECK, in LOCATION:>\n", current.verb);
    if (!trycheck(alt[1]->checks, TRUE)) return;
    if (fail) return;
  }
  
  for (i = 0; params[i].code != EOF; i++) {
    current.instance = params[i].code;
    if (isLit(params[i].code))
      alt[i+2] = 0;
    else {
      alt[i+2] = findalt(instance[params[i].code].verbs, i+1);
      /* CHECKs in the parameters */
      if (alt[i+2] != 0 && alt[i+2]->checks != 0) {
	if (trcflg)
	  printf("\n<VERB %d, CHECK, in Parameter #%d:>\n", current.verb, i);
	if (!trycheck(alt[i+2]->checks, TRUE)) return;
	if (fail) return;
      }
    }
  }

  /* Check for anything to execute... */
  for (i = 0; i < 2 || params[i-2].code != EOF; i++)
    if (alt[i] != 0 && alt[i]->action != 0)
      break;
  if (i >= 2 && params[i-2].code == EOF)
    /* Didn't find any code for this verb/object combination */
    error(M_CANT0);
  
  /* Perform actions! */
  
  /* First try any BEFORE or ONLY from outside in */
  done[0] = FALSE;
  done[1] = FALSE;
  for (i = 2; params[i-2].code != EOF; i++)
    done[i] = FALSE;
  i--;
  while (i >= 0) {
    if (alt[i] != 0)
      if (alt[i]->qual == (Aword)Q_BEFORE || alt[i]->qual == (Aword)Q_ONLY) {
	if (alt[i]->action != 0) {
	  if (trcflg) {
	    if (i == 0)
	      strcpy(trace, "GLOBAL");
	    else if (i == 1)
	      strcpy(trace, "in LOCATION");
	    else
	      sprintf(trace, "in PARAMETER %d", i-1);
	    if (alt[i]->qual == (Aword)Q_BEFORE)
	      printf("\n<VERB %d, %s (BEFORE), Body:>\n", current.verb, trace);
	    else
	      printf("\n<VERB %d, %s (ONLY), Body:>\n", current.verb, trace);
	  }
	  current.instance = params[i-2].code;
	  interpret(alt[i]->action);
	  if (fail) return;
	  if (alt[i]->qual == (Aword)Q_ONLY) return;
	}
	done[i] = TRUE;
      }
    i--;
  }
  
  /* Then execute any not declared as AFTER, i.e. the default */
  for (i = 0; i < 2 || params[i-2].code != EOF; i++) {
    if (alt[i] != 0)
      if (alt[i]->qual != (Aword)Q_AFTER) {
	if (!done[i] && alt[i]->action != 0) {
	  if (trcflg) {
	    if (i == 0)
	      strcpy(trace, "GLOBAL");
	    else if (i == 1)
	      strcpy(trace, "in LOCATION");
	    else
	      sprintf(trace, "in PARAMETER %d", i-1);
	    printf("\n<VERB %d, %s, Body:>\n", current.verb, trace);
	  }
	  current.instance = params[i-2].code;
	  interpret(alt[i]->action);
	  if (fail) return;
	}
	done[i] = TRUE;
      }
  }

  /* Finally, the ones declared as after */
  i--;
  while (i >= 0) {
    if (alt[i] != 0)
      if (!done[i] && alt[i]->action != 0) {
	if (trcflg) {
	  if (i == 0)
	    strcpy(trace, "GLOBAL");
	  else if (i == 1)
	    strcpy(trace, "in LOCATION");
	  else
	    sprintf(trace, "in PARAMETER %d", i-1);
	  printf("\n<VERB %d, %s (AFTER), Body:>\n", current.verb, trace);
	}
	current.instance = params[i-2].code;
	interpret(alt[i]->action);
	if (fail) return;
      }
    i--;
  }
}



/*======================================================================

  action()

  Execute all activities commanded. Handles possible multiple actions
  such as THEM or lists of objects.

  */
#ifdef _PROTOTYPES_
void action(
     ParamEntry plst[]		/* IN - Plural parameter list */
)
#else
void action(plst)
     ParamEntry plst[];
#endif
{
  int i, mpos;
  char marker[10];

  if (plural) {
    /*
       The code == 0 means this is a multiple position. We must loop
       over this position (and replace it by each present in the plst)
     */
    for (mpos = 0; params[mpos].code != 0; mpos++); /* Find multiple position */
    sprintf(marker, "($%d)", mpos+1); /* Prepare a printout with $1/2/3 */
    for (i = 0; plst[i].code != EOF; i++) {
      params[mpos] = plst[i];
      output(marker);
      do_it();
      if (plst[i+1].code != EOF)
        para();
    }
    params[mpos].code = 0;
  } else
    do_it();
}



/*----------------------------------------------------------------------*\

  Event Handling

  eventchk()

\*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------

  eventCheck()

  Check if any events are pending. If so execute them.
  */
#ifdef _PROTOTYPES_
static void eventCheck(void)
#else
static void eventCheck()
#endif
{
  while (etop != 0 && eventQueue[etop-1].time == current.tick) {
    etop--;
    if (isLoc(eventQueue[etop].where))
      current.location = eventQueue[etop].where;
    else
      current.location = where(eventQueue[etop].where);
    if (trcflg) {
      printf("\n<EVENT %d (at ", eventQueue[etop].event);
      debugsay(current.location);
      printf("):>\n");
    }
    interpret(events[eventQueue[etop].event].code);
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
#ifdef _PROTOTYPES_
static void checkvers(AcdHdr *header)
#else
static void checkvers(header)
     AcdHdr *header;
#endif
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
  if (dbgflg) {
    state[0] = header->vers[3];
    state[1] = '\0';
    printf("<Version of '%s' is %d.%d(%d)%s>",
	   advnam,
	   (int)(header->vers[0]),
	   (int)(header->vers[1]),
	   (int)(header->vers[2]),
	   (header->vers[3])==0? "": state);
    newline();
  }

  /* Development version require exact match, else only 2 digit match */
  developmentVersion = (strcmp(alan.version.state, "dev") == 0);
  compareLength = (developmentVersion? 3 : 2);

  /* Compatible if version and revision match... */
  if (memcmp(header->vers, vers, compareLength) != 0) {
      if (errflg) {
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


/*----------------------------------------------------------------------

  load()

 */
#ifdef _PROTOTYPES_
static void load(void)
#else
static void load()
#endif
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

#ifdef REVERSED
  reverseHdr(&tmphdr);
#endif

  /* No memory allocated yet? */
  if (memory == NULL) {
    memory = allocate(tmphdr.size*sizeof(Aword));
  }
  header = (AcdHdr *) addrTo(0);

  memTop = fread(addrTo(0), sizeof(Aword), tmphdr.size, codfil);
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
    if (errflg)
      syserr(err);
    else {
      output("<WARNING! $$");
      output(err);
      output("$$ Ignored, proceed at your own risk.>$n");
    }
  }

#ifdef REVERSED
  if (dbgflg||trcflg||stepFlag)
    output("<Hmm, this is a little-endian machine, fixing byte ordering....");
  reverseACD(tmphdr.vers[0] == 2 && tmphdr.vers[1] == 5); /* Reverse all words in the ACD file */
  if (dbgflg||trcflg||stepFlag)
    output("OK.>$n");
#endif
}


/*----------------------------------------------------------------------

  checkdebug()

 */
#ifdef _PROTOTYPES_
static void checkdebug(void)
#else
static void checkdebug()
#endif
{
  /* Make sure he can't debug if not allowed! */
  if (!header->debug) {
    if (dbgflg|trcflg|stepFlag)
      printf("<Sorry, '%s' is not compiled for debug!>\n", advnam);
    para();
    dbgflg = FALSE;
    trcflg = FALSE;
    stepFlag = FALSE;
  }

  if (dbgflg)			/* If debugging */
    srand(0);			/* use no randomization */
  else
    srand(time(0));		/* seed random generator */
}


/*----------------------------------------------------------------------

  initheader()

 */
#ifdef _PROTOTYPES_
static void initheader(void)
#else
static void initheader()
#endif
{
  /* Allocate for administrative table */
  admin = (AdminEntry *)allocate((header->instanceMax+1)*sizeof(AdminEntry));

  dict = (WrdEntry *) addrTo(header->dictionary);
  /* Find out number of entries in dictionary */
  for (dictsize = 0; !endOfTable(&dict[dictsize]); dictsize++);

  if (header->instanceTableAddress == 0)
    syserr("Instance table pointer == 0");
  instance = (InstanceEntry *) addrTo(header->instanceTableAddress);
  instance--;			/* Back up one so that first is no. 1 */


  if (header->classTableAddress == 0)
    syserr("Class table pointer == 0");
  class = (ClassEntry *) addrTo(header->classTableAddress);
  class--;			/* Back up one so that first is no. 1 */

  if (header->containerTableAddress != 0) {
    container = (ContainerEntry *) addrTo(header->containerTableAddress);
    container--;
  }

  if (header->eventTableAddress != 0) {
    events = (EventEntry *) addrTo(header->eventTableAddress);
    events--;
  }

  stxs = (StxEntry *) addrTo(header->syntaxTableAddress);
  vrbs = (VrbEntry *) addrTo(header->verbTableAddress);
  ruls = (RulEntry *) addrTo(header->ruleTableAddress);
  msgs = (MsgEntry *) addrTo(header->messageTableAddress);
  scores = (Aword *) addrTo(header->scores);

  if (header->pack)
    freq = (Aword *) addrTo(header->freq);
}


/*----------------------------------------------------------------------

  initstrings()

  */
#ifdef _PROTOTYPES_
static void initstrings(void)
#else
static void initstrings()
#endif
{
  IniEntry *init;
  AttributeEntry *attribute;

  for (init = (IniEntry *) addrTo(header->init); !endOfTable(init); init++) {
    getstr(init->fpos, init->len);
    attribute = addrTo(init->adr);
    attribute->value = pop();
  }
}


/*----------------------------------------------------------------------

  start()

 */
#ifdef _PROTOTYPES_
static void start(void)
#else
static void start()
#endif
{
  int startloc;

  current.tick = -1;
  current.location = startloc = where(HERO);
  current.actor = HERO;
  current.score = 0;
  if (trcflg)
    printf("\n<START:>\n");
  interpret(header->start);
  para();

  instance[HERO].location = 0;
  locate(HERO, startloc);
}



/*----------------------------------------------------------------------
  init()

  Initialization, program load etc.

  */
#ifdef _PROTOTYPES_
static void init(void)
#else
static void init()
#endif
{
  int i;

  /* Initialise some status */
  etop = 0;			/* No pending events */
  looking = FALSE;		/* Not looking now */
  dscrstkp = 0;			/* No describe in progress */

  load();

  initheader();
  checkdebug();

  /* Initialise string attributes */
  initstrings();

  getPageSize();

  /* Find first conjunction and use that for ',' handling */
  for (i = 0; i < dictsize; i++)
    if (isConj(i)) {
      conjWord = i;
      break;
    }

  /* Start the adventure */
  clear();
  start();
}



static Boolean traceActor(int theActor)
{
  if (trcflg) {
    printf("\n<ACTOR %d, ", theActor);
    debugsay(theActor);
    printf(" (at ");
    debugsay(current.location);
  }
  return trcflg;
}

/*----------------------------------------------------------------------

  scriptName()

*/
static char *scriptName(int theActor, int theScript)
{
  ScriptEntry *scriptEntry = addrTo(header->scriptTableAddress);

  while (theScript > 1) {
    scriptEntry++;
    theScript--;
  }
  return addrTo(scriptEntry->stringAddress);
}


/*----------------------------------------------------------------------

  movactor()

  Let the current actor move. If player, ask him.

 */
#ifdef _PROTOTYPES_
static void moveActor(int theActor)
#else
static void moveActor(theActor)
     int theActor;
#endif
{
  ScriptEntry *scr;
  StepEntry *step;
  Aword previousInstance = current.instance;


  current.actor = theActor;
  current.instance = theActor;
  current.location = where(theActor);
  if (theActor == HERO) {
    parse();
    fail = FALSE;			/* fail only aborts one actor */
  } else if (admin[theActor].script != 0) {
    for (scr = (ScriptEntry *) addrTo(header->scriptTableAddress); !endOfTable(scr); scr++) {
      if (scr->code == admin[theActor].script) {
	/* Find correct step in the list by indexing */
	step = (StepEntry *) addrTo(scr->steps);
	step = (StepEntry *) &step[admin[theActor].step];
	/* Now execute it, maybe. First check wait count */
	if (step->after > admin[theActor].waitCount) { /* Wait some more ? */
	  if (traceActor(theActor))
	    printf("), SCRIPT %s(%ld), STEP %ld, Waiting another %ld turns>\n",
		   scriptName(theActor, admin[theActor].script),
		   admin[theActor].script, admin[theActor].step+1,
		   step->after-admin[theActor].waitCount);
	  admin[theActor].waitCount++;
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
    if (trcflg) {
      printf("\n<ACTOR %d, ", theActor);
      debugsay(theActor);
      printf(" (at ");
      debugsay(current.location);
      printf("), Idle>\n");
    }
  }

  current.instance = previousInstance;
}

/*----------------------------------------------------------------------

  openFiles()

  Open the necessary files.

  */
#ifdef _PROTOTYPES_
static void openFiles(void)
#else
static void openFiles()
#endif
{
  char str[256];
  char *usr = "";
  time_t tick;

  /* Open Acode file */
  strcpy(codfnm, advnam);
  strcat(codfnm, ".a3c");
  if ((codfil = fopen(codfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure code file '");
    strcat(str, codfnm);
    strcat(str, "'.");
    syserr(str);
  }

  /* Open Text file */
  strcpy(txtfnm, advnam);
  strcat(txtfnm, ".a3c");
  if ((txtfil = fopen(txtfnm, READ_MODE)) == NULL) {
    strcpy(str, "Can't open adventure text data file '");
    strcat(str, txtfnm);
    strcat(str, "'.");
    syserr(str);
  }

  /* If logging open log file */
  if (logflg) {
    char *namstart;

    if((namstart = strrchr(advnam, ']')) == NULL
       && (namstart = strrchr(advnam, '>')) == NULL
       && (namstart = strrchr(advnam, '/')) == NULL
       && (namstart = strrchr(advnam, '\\')) == NULL
       && (namstart = strrchr(advnam, ':')) == NULL)
      namstart = &advnam[0];
    else
      namstart++;

    time(&tick);
    sprintf(logfnm, "%s%d%s.log", namstart, (int)tick, usr);
    if ((logfil = fopen(logfnm, "w")) == NULL)
      logflg = FALSE;
  }
}
    

/*======================================================================

  run()

  Run the adventure

  */
void run(void)
{
  int i;


  openFiles();

  setjmp(restart_label);	/* Return here if he wanted to restart */

  init();			/* Load, initialise and start the adventure */

  while (TRUE) {
#ifdef MALLOC
    if (malloc_verify() == 0) syserr("Error in heap.");
#endif
    if (dbgflg)
      debug();

    eventCheck();
    current.tick++;
    (void) setjmp(jmpbuf);


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

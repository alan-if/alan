/*----------------------------------------------------------------------*\

  util.c

  Alan compiler utilities unit

\*----------------------------------------------------------------------*/

#include "util.h"


/* IMPORTS */
#include "alan.h"
#include "sysdep.h"
#include "lmList.h"
#include "smScan.h"
#include "options.h"


/* PUBLIC DATA */

Srcp nulsrcp			/* NULL position for list */
  = {0,0,0};

Bool verbose;		/* Verbose output */
long counter;		/* And counter for verbose mode */
long allocated;		/* Calculated memory usage */


/* PRIVATE DATA */


/* FUNCTIONS */



/*======================================================================

  showProgress()

*/
void showProgress() {
  if (verbose) {
    printf("%8ld\b\b\b\b\b\b\b\b", counter++);
    fflush(stdout);
  }
}



/*======================================================================*/
void *allocate(int len)		/* IN - Length to allocate */
{
  void *p = calloc(1, (size_t)len);

  if (p == NULL)
    panic("Out of memory");

  allocated += len;

  return p;
}


/*======================================================================

  unimpl()

  An unimplemented constrution was encountered.

 */
void unimpl(Srcp srcp,		/* IN  - Where? */
	    char *phase)	/* IN  - What phase? */
{
  lmLog(&srcp, 998, sevWAR, phase);
}



/*----------------------------------------------------------------------

	Find out whether error message is of one of the severities
	being printed.

*/
static int test_severity(char *err, lmSev sevs)
{
  /* Check if the severity was among the wanted ones */
  char c;
  lmSev sev = sevSYS;

  sscanf(err, "%*d %c", &c);
  switch (c) {
  case 'O': sev = sevOK;  break;
  case 'I': sev = sevINF; break;
  case 'W': sev = sevWAR; break;
  case 'E': sev = sevERR; break;
  case 'F': sev = sevFAT; break;
  case 'S': sev = sevSYS; break;
  default: SYSERR("Unexpected severity marker");
  }
  return sev & sevs;
}


/*----------------------------------------------------------------------*/
static void specialListing(lmSev sevs)
{
  int i,j;
  char err[1024], line[1024];
  Srcp srcp;
  List *fnm;
  List nofile;
	
  nofile.element.str = "<no file>";
  for (i = 1; lmMsg(i, &srcp, err); i++) {
    if (test_severity(err, sevs)) {
      /* Advance to the correct file name */
      if (srcp.file == -1) 
	fnm = &nofile;
      else
	for (fnm = fileNames, j = 0; j < srcp.file; j++)
	  if (fnm != NULL)
	    fnm = fnm->next;
      if (fnm == NULL)
	fnm = &nofile;
      if (ccFlag)
	sprintf(line, "\"%s\", line %d(%d): %s\n",
		fnm->element.str, srcp.line, srcp.col, err);
      else if (ideFlag)
	sprintf(line, "\"%s\", line %d %d-%d: %s\n",
		fnm->element.str, srcp.line, srcp.startpos, srcp.endpos, err);
      else
	sprintf(line, "\"%s\", line %d:%d: ALAN-%s (column %d)\n",
		fnm->element.str, srcp.line, srcp.col, err, srcp.col);

#ifdef __mac__
      lmLiPrint(line);
#else
      printf(line);
#endif
    }
  }
}


#ifdef __mac__
/*----------------------------------------------------------------------

  Write listing and/or error messages to screen or file

*/
static void listing(lmSev sevs)
{
  char *fnm;

  if (lstflg)
    fnm = lstfnm;
  else
    fnm = "";
#ifdef __MWERKS__
  _fcreator = 'ttxt';
  _ftype = 'TEXT';
#endif

  listing(fnm, lcount, ccount, fulflg?liFULL:liTINY, sevs);

  if (dmpflg) {
    lmSkipLines(0);
    duadv(dmpflg);
  }

  if (sumflg) {
    if (lmSeverity() < sevERR)
      summary();
    endtotal();			/* Stop timer */
    prtimes();
    stats();
  }
  lmLiTerminate();
}
#else

/*======================================================================*/
void listing(char *listFileName, int lines, int columns,
	     lmTyp listingType, lmSev severities) {
  if (ccFlag || ideFlag) {
    lmList(listFileName, lines, columns, 0, 0);	/* Sort and prepare for retrieval */
    specialListing(severities);
  } else
    lmList(listFileName, lines, columns, listingType, severities);
}

#endif


/*======================================================================*/
void syserr(char *errorMessage, char *function, char *file, int line)
{
  int messageLength;
  char *position;
  char *messageString;

  messageLength = strlen(errorMessage) + strlen(function) + strlen(file) + strlen(" in '()', :00000");

  messageString = allocate(messageLength+1);
  sprintf(messageString, "%s in '%s()', %s:%d", errorMessage, function, file, line);

  lmLog(&nulsrcp, 997, sevSYS, messageString);
  listing("", 0, 79, liTINY, sevALL);
  terminate(EXIT_FAILURE);
}


/*======================================================================

  panic()

  A catastrophe has happened. Print message but do as little as possible.

  */
void panic(char *str)
{
  printf("PANIC!! %s\n", str);
  terminate(EXIT_FAILURE);
}


/*======================================================================

  terminate()

  Terminate the program with an error code.

 */
#ifdef WINGUI
#include <windows.h>
void terminate(int ecode)
{
  if (guiMode) {
    char *message = "Finished with strange error status!";
    switch (lmSeverity()) {
    case sevOK: 
      message = "Finished OK!"; break;
    case sevINF:
      message = "Finished OK with some informational messages."; break;
    case sevWAR:
      message = "Finished OK with warning messages."; break;
    case sevERR:
    case sevFAT:
      message = "Finished with errors."; break;
    }
    MessageBox(NULL, message, "Alan V3 (Development) Compilation Result", MB_OK);
  }
  exit(ecode);
}
#else
void terminate(int ecode)
{
#ifdef __MWERKS__
  printf("Command-Q to quit.");
#endif
  exit(ecode);
}
#endif

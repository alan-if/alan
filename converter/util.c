/*----------------------------------------------------------------------*\

  util.c

  Alan compiler utilities unit

\*----------------------------------------------------------------------*/

#include "util.h"


/* IMPORTS */

#include "sysdep.h"
#include "lmList.h"


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
char *strlow(char str[])        /* INOUT - Native string to convert */
{
  char *s;

  for (s = str; *s; s++)
    *s = tolower(*s);
  return(str);
}


/*======================================================================

  allocate()

  Safely allocate new memory.

*/
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
void unimpl(Srcp *srcp,		/* IN  - Where? */
	    char *phase)	/* IN  - What phase? */
{
  lmLog(srcp, 998, sevWAR, phase);
}


/*======================================================================*/
void syserr(char *errorMessage, char insertString[])
{
  char *messageString;
  int len = 0;

  len = strlen(errorMessage);
  len += insertString?strlen(insertString)+1:0;
  messageString = allocate(len);

  if (insertString) {
    messageString = allocate(strlen(errorMessage)+strlen(insertString)+1);
    sprintf(messageString, errorMessage, insertString);
  } else {
    messageString = allocate(strlen(errorMessage)+1);
    sprintf(messageString, errorMessage);
  }
  lmLog(&nulsrcp, 997, sevSYS, messageString);
  lmList("", 0, 79, liTINY, sevALL);
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
void terminate(int ecode)
{
#ifdef __MWERKS__
	printf("Command-Q to quit.");
#endif
	exit(ecode);
}


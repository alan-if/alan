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


/*======================================================================

  syserr()

  Some kind of internal system error was detected. Log it.

 */
void syserr(char *str)
{
  lmLog(&nulsrcp, 997, sevSYS, str);
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


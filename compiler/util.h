#ifndef _UTIL_H_
#define _UTIL_H_
/*----------------------------------------------------------------------*\

  util.h

  Alan compiler utilities unit

\*----------------------------------------------------------------------*/

/* IMPORTS */

#include "types.h"
#include "srcp.h"


/* PUBLIC DATA */

extern Srcp nulsrcp;		/* NULL position for list */
extern long allocated;		/* Calculated memory usage */
extern Bool verbose;		/* Verbose output */
extern long counter;		/* And counter for verbose mode */


/* FUNCTIONS */

#define progressMeter() {\
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }\
\
}

extern void *allocate(int len);
extern void unimpl(Srcp *srcp, char *phase);
extern void syserr(char *str);
extern void panic(char *str);
extern void terminate(int ecode);

#endif

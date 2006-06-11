#ifndef _UTIL_H_
#define _UTIL_H_
/*----------------------------------------------------------------------*\

  util.h

  Alan compiler utilities unit

\*----------------------------------------------------------------------*/

/* IMPORTS */

#include "types.h"
#include "srcp.h"
#include "lmList.h"


/* PUBLIC DATA */

extern Srcp nulsrcp;		/* NULL position for list */
extern long allocated;		/* Calculated memory usage */
extern Bool verbose;		/* Verbose output */
extern long counter;		/* And counter for verbose mode */


/* FUNCTIONS */
#define SYSERR(m) syserr(m, __FUNCTION__, __FILE__, __LINE__)
extern void syserr(char *errorMessage, const char *function, char *file, int line);
extern void listing(char *listFileName, int lines, int columns,
		    lmTyp listingType, lmSev severities);
extern void progressCounter(void);
extern void *allocate(int len);
extern void unimpl(Srcp srcp, char *phase);
extern void panic(char *str);
extern void terminate(int ecode);

#endif

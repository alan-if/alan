/*----------------------------------------------------------------------*\

  options.h

  Alan compiler command line options

\*----------------------------------------------------------------------*/

#include "lst.h"

extern char *srcptr;		/* Pointer to adventure name */
extern Bool verbose;			/* Verbose mode */
extern Bool warnings;		/* Show warnings */
extern Bool infos;		/* Show informational messages */
extern Bool fulflg;		/* Full source listing */
extern Bool lstflg;		/* Create listing file */
extern Bool ccflg;		/* Show messages as old 'cc' */
extern int lcount;		/* Number of lines per page */
extern int ccount;		/* -"-    columns */
extern DmpKind dmpflg;	/* Dump internal form flags */
extern Bool dbgflg;		/* Debug option flags */
extern Bool packflg;	/* Pack option flags */
extern Bool sumflg;		/* Print a summary */
extern List *includePaths;	/* List of additional include paths */

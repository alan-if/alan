/*----------------------------------------------------------------------*\

  options.c

  Alan compiler command line options

\*----------------------------------------------------------------------*/

#include "types.h"
#include "dump.h"
#include "options.h"
#include "lst.h"

char *srcptr;			/* Pointer to adventure name */
Bool verbose;			/* Verbose mode */
int charset;			/* Which charset to read source in */
Bool warnings;			/* Show warnings */
Bool infos;			/* Show informational messages */
Bool fulflg;			/* Full source listing */
Bool lstflg;			/* Create listing file */
Bool ccflg;			/* Show messages as old 'cc' */
int lcount;			/* Number of lines per page */
int ccount;			/* -"-    columns */
DmpKind dmpflg = DMPNON;	/* Dump internal form flags */
Bool dbgflg = 0;		/* Debug option flags */
Bool packflg = 0;		/* Pack option flags */
Bool sumflg;			/* Print a summary */
List *includePaths = NULL;	/* List of additional include paths */

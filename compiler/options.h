#ifndef OPTIONS_H
#define OPTIONS_H
/*----------------------------------------------------------------------* \

  options.h

  Alan compiler command line option flags
  NOTE that options in the generated Acode is handled by opt.h/.c

\*----------------------------------------------------------------------*/

#include "lst.h"
#include "dump.h"

extern char *srcptr;            /* Pointer to adventure name */
extern bool verboseFlag;        /* Verbose mode */
extern bool warningFlag;        /* Show warnings */
extern bool infoFlag;           /* Show informational messages */
extern bool fullFlag;           /* Full source listing */
extern bool listingFlag;        /* Create listing file */
extern bool ideFlag;            /* Format messages for AlanIDE */
extern bool ccFlag;             /* Show messages as old 'cc' */
extern int lcount;              /* Number of lines per page */
extern int ccount;              /* -"-    columns */
extern DmpKind dumpFlags;       /* Dump internal form flags */
extern bool xmlFlag;            /* Output XML option flag */
extern bool debugFlag;          /* Debug option flag */
extern bool packFlag;           /* Pack option flag */
extern bool summaryFlag;        /* Print a summary */
extern List *importPaths;       /* List of additional include paths */

#endif

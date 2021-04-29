/*----------------------------------------------------------------------*\

  options.c

  Alan compiler command line options

\*----------------------------------------------------------------------*/

#include "types.h"
#include "dump.h"
#include "options.h"
#include "lst.h"

char *srcptr;                   /* Pointer to adventure name */
Bool verboseFlag;               /* Verbose mode */
Bool warningFlag;               /* Show warnings */
Bool infoFlag;                  /* Show informational messages */
Bool fullFlag;                  /* Full source listing */
Bool listingFlag;               /* Create listing file */
Bool ccFlag;                    /* Show messages as old 'cc' */
Bool ideFlag;                   /* Format messages for AlanIDE */
int lcount;                     /* Number of lines per page */
int ccount;                     /* -"-    columns */
DmpKind dumpFlags = DUMP_NOTHING; /* Dump internal form flags */
Bool xmlFlag = 0;               /* XML output option flag */
Bool debugFlag = 0;             /* Debug option flag, only valid until after parsing */
Bool packFlag = 0;              /* Pack option flag, d:o */
Bool summaryFlag;               /* Print a summary flag */
List *importPaths = NULL;      /* List of additional import directories */

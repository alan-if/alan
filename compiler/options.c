/*----------------------------------------------------------------------*\

  options.c

  Alan compiler command line options

\*----------------------------------------------------------------------*/

#include "types.h"
#include "dump.h"
#include "options.h"
#include "lst.h"

char *srcptr;                   /* Pointer to adventure name */
bool verboseFlag;               /* Verbose mode */
bool warningFlag;               /* Show warnings */
bool infoFlag;                  /* Show informational messages */
bool fullFlag;                  /* Full source listing */
bool listingFlag;               /* Create listing file */
bool ideFlag;                   /* Format messages for AlanIDE */
bool ccFlag;                    /* Show messages as old 'cc' */
int lcount;                     /* Number of lines per page */
int ccount;                     /* -"-    columns */
DmpKind dumpFlags = DUMP_NOTHING; /* Dump internal form flags */
bool xmlFlag = 0;               /* XML output option flag */
bool debugFlag = 0;             /* Debug option flag, only valid until after parsing */
bool packFlag = 0;              /* Pack option flag, d:o */
bool summaryFlag;               /* Print a summary flag */
List *importPaths = NULL;      /* List of additional import directories */

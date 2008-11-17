#ifndef _CHECKENTRYARRAY_H_
#define _CHECKENTRYARRAY_H_
/*----------------------------------------------------------------------*\

  Array of CheckEntries

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"


/* CONSTANTS */


/* TYPES */

typedef CheckEntry CheckEntryArray[];


/* DATA */


/* FUNCTIONS */
extern Bool tryChecks(Aaddr adr, Bool act);

#endif

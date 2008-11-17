#ifndef _CHECKARRAY_H_
#define _CHECKARRAY_H_
/*----------------------------------------------------------------------*\

  Array of CheckEntries

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"


/* CONSTANTS */


/* TYPES */

typedef CheckEntry CheckArray[];


/* DATA */


/* FUNCTIONS */
extern Bool tryChecks(Aaddr adr, Bool act);

#endif

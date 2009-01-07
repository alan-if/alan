#ifndef _CHECKENTRYARRAY_H_
#define _CHECKENTRYARRAY_H_
/*----------------------------------------------------------------------*\

  Array of CheckEntries

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"


/* CONSTANTS */
#ifndef EXECUTE_CHECK_BODY_ON_FAIL
#define EXECUTE_CHECK_BODY_ON_FAIL TRUE
#define DONT_EXECUTE_CHECK_BODY_ON_FAIL FALSE
#endif


/* TYPES */

typedef CheckEntry CheckEntryArray[];


/* DATA */


/* FUNCTIONS */
extern Bool checksFailed(Aaddr adr, Bool execute);

#endif

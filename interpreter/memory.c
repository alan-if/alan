/*----------------------------------------------------------------------*\

    The Amachine memory

\*----------------------------------------------------------------------*/
#include "memory.h"

/* Imports */

#include "types.h"


/* Public Data */

Aword *memory = NULL;
static ACodeHeader dummyHeader; /* Dummy to use until memory allocated */
ACodeHeader *header = &dummyHeader;
int memTop = 0;         /* Top of load memory */


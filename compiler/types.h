/*----------------------------------------------------------------------*\

    types.h

    Common types for the Alan compiler

\*----------------------------------------------------------------------*/
#ifndef _TYPES_
#define _TYPES_

#include "srcp.h"


#define NEW(x) ((x *) allocate(sizeof(x)))

typedef int Bool;

#ifndef TRUE
#define TRUE (0==0)
#define FALSE (!TRUE)
#endif

#endif

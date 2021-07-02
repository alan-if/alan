/*----------------------------------------------------------------------*\

    types.h

    Common types for the Alan compiler

\*----------------------------------------------------------------------*/
#ifndef _TYPES_
#define _TYPES_

#include <stdbool.h>

/* TODO: use stdbool instead of bool */
typedef int Bool;
typedef unsigned int uint;
typedef unsigned char uchar;

#define NEW(x) ((x *) allocate(sizeof(x)))

/* Size of something in Awords */
#define ASIZE(x) (sizeof(x)/sizeof(Aword))

#endif

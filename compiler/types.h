/*----------------------------------------------------------------------*\

    types.h

    Common types for the Alan compiler

\*----------------------------------------------------------------------*/
#ifndef _TYPES_
#define _TYPES_

#include <stdbool.h>

/* TODO: use stdbool instead of Bool */
typedef int Bool;
typedef unsigned int uint;
typedef unsigned char uchar;

#ifndef TRUE
#define TRUE (0==0)
#define FALSE (!TRUE)
#endif

#define NEW(x) ((x *) allocate(sizeof(x)))

/* Size of something in Awords */
#define ASIZE(x) (sizeof(x)/sizeof(Aword))

#endif

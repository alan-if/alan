/*
    types.h

    Common types for the Alan compiler

 */

#ifndef _TYPES_
#define _TYPES_


#define NEW(x) ((x *) allocate(sizeof(x)))

typedef int Boolean;
#ifndef TRUE
#define TRUE (0==0)
#define FALSE (!TRUE)
#endif

#endif

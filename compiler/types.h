/*----------------------------------------------------------------------*\

			       types.h

		  Common types for the Alan compiler

\*----------------------------------------------------------------------*/

#ifndef _TYPES_H_
#define _TYPES_H_

#include "sysdep.h"

#ifdef _PROTOTYPES_
extern void *allocate(int size);
#else
extern void *allocate();
#endif

#define NEW(x) ((x *) allocate(sizeof(x)))

typedef int Bool;

#ifndef TRUE
#define TRUE (0==0)
#define FALSE (!TRUE)
#endif

#endif

#ifndef _SET_H_
#define _SET_H_
/*----------------------------------------------------------------------*\

  SET.H

  Header file for set types of ARUN Alan System interpreter
  
  A Set is implemented as a small datastucture holding a current size,
  allocated size and a pointer to the member array which is dynamically
  allocated.

\*----------------------------------------------------------------------*/

#include "acode.h"

typedef struct Set {
	int size;
	int allocated;
	Aword *members;
} Set;

#endif

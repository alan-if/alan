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
#include "types.h"


typedef struct Set {
	int size;
	int allocated;
	Aword *members;
} Set;


extern void initSets(SetInitEntry *initTable);
extern int sizeOfSet(Set *theSet);
extern void clearSet(Set *theSet);
extern Set *copySet(Set *theSet);
extern Aword getMember(Set *theSet, Aint member);
extern Bool inSet(Set *theSet, Aword member);
extern void addToSet(Set *theSet, Aword newMember);
extern void removeFromSet(Set *theSet, Aword member);

#endif

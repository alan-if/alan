#ifndef _NAME_H_
#define _NAME_H_
/*----------------------------------------------------------------------*\

				Name.h

		      Names, (i.e. Lists of Ids)

\*----------------------------------------------------------------------*/

/* USE: */
#include "List.h"
#include "acode.h"


/* Types: */


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Analyse a Name by putting the words into the dictionary */
extern void analyseName(List *name);

/* Generate the dictionary */
extern Aaddr generateName(List *name);

#else
extern void analyseName();
extern Aaddr generateName();
#endif

#endif

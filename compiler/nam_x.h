#ifndef _NAM_X_H_
#define _NAM_X_H_
/*----------------------------------------------------------------------*\

			       NAM_X.H
			    Name Handling

\*----------------------------------------------------------------------*/

/* OWN TYPES: */
#include "nam.h"


/* USE: */
#include "id.h"


/* DATA: */


/* METHODS: */

/* Analyse names, returns length of name text */
extern int analyzeNames(List *names,
			IdNode *id);

#endif

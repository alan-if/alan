#ifndef SCO_H
#define SCO_H
/*----------------------------------------------------------------------*\

				SCO.H
				Scores
                                                                     
\*----------------------------------------------------------------------*/

#include "acode.h"

/* Types: */

/* Data: */

extern int scocnt;
extern int scotot;



/* Functions: */

/* Prepare a score table */
extern void prepscos(void);

/* Generate the score table */
extern Aaddr gesco(void);


#endif

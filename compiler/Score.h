#ifndef SCORE_H
#define SCORE_H
/*----------------------------------------------------------------------*\

			       Score.h

			    Score Handling
                                                                     
\*----------------------------------------------------------------------*/


#include "acode.h"


/* Types: */

/* Data: */

extern int scoreCount;
extern int scoreTotal;



/* Functions: */
#ifdef _PROTOTYPES_

/* Prepare a score table */
extern void prepareScores(void);

/* Generate the score table */
extern Aaddr generateScores(void);

#else
extern void prepareScores();
extern Aaddr generateScores();
#endif

#endif

/*----------------------------------------------------------------------*\

				Score.c

				Scores

\*----------------------------------------------------------------------*/

#include "Score.h"

#include "Srcp.h"
#include "lmList.h"

#include "Adventure.h"

#include "emit.h"
#include "acode.h"




/* PUBLIC: */

int scoreCount = 0;
int scoreTotal = 0;



/*======================================================================

  prepareScores()

  Prepare score handling by allocating space to collect them in

  */
#ifdef _PROTOTYPES_
void prepareScores(void)
#else
void prepareScores()
#endif
{
  adventure.scores = (int *)allocate((scoreCount+1)*sizeof(int));
}


/*======================================================================

  generateScores()

  Generate information on the scores.

  */
#ifdef _PROTOTYPES_
Aaddr generateScores(void)
#else
Aaddr generateScores()
#endif
{
  int i;
  Aaddr adr;

  adr = emitAddress();		/* Save address of score table */
  if (scoreCount != 0) {
    for (i = 1; i <= scoreCount; i++)
      emit(adventure.scores[i]);
  }
  emit(EOF);
  return(adr);
}

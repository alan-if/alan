/*----------------------------------------------------------------------*\

				SCO.C
				Scores

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "adv_x.h"

#include "lmList.h"

#include "sco.h"
#include "emit.h"
#include "../interpreter/acode.h"




/* PUBLIC: */

int scocnt = 0;
int scotot = 0;



/*======================================================================

  prepscos()

  Prepare score handling by allocating space to collect them in

  */
void prepscos(void)
{
  adv.scores = (int *) allocate((scocnt+1)*sizeof(int));
}


/*======================================================================

  gesco()

  Generate information on the scores.

  */
Aaddr gesco(void)
{
  int i;
  Aaddr adr;

  adr = nextEmitAddress();		/* Save address of score table */
  if (scocnt != 0) {
    for (i = 1; i <= scocnt; i++)
      emit(adv.scores[i]);
  }
  emit(EOF);
  return(adr);
}

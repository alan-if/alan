/*----------------------------------------------------------------------*\

				SCO.C
				Scores

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"
#include "sco.h"
#include "emit.h"
#include "acode.h"




/* PUBLIC: */

int scocnt = 0;
int scotot = 0;



/*======================================================================

  prepscos()

  Prepare score handling by allocating space to collect them in

  */
#ifdef _PROTOTYPES_
void prepscos(void)
#else
void prepscos()
#endif
{
  adv.scores = (int *) allocate((scocnt+1)*sizeof(int));
}


/*======================================================================

  gesco()

  Generate information on the scores.

  */
#ifdef _PROTOTYPES_
Aaddr gesco(void)
#else
Aaddr gesco()
#endif
{
  int i;
  Aaddr adr;

  adr = emadr();		/* Save address of score table */
  if (scocnt != 0) {
    for (i = 1; i <= scocnt; i++)
      emit(adv.scores[i]);
  }
  emit(EOF);
  return(adr);
}

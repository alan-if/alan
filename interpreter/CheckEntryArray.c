#include "CheckEntryArray.h"

#include "main.h"
#include "inter.h"
#include "stack.h"


/*======================================================================*/
Bool tryChecks(Aaddr adr,	/* IN - ACODE address to check array */
	       Bool execute	/* IN - Execute the ELSE clause if it fails ? */
)
{
  CheckEntry *chk;

  chk = (CheckEntry *) pointerTo(adr);
  if (chk->exp == 0) {
    if (execute)
      interpret(chk->stms);
    return(FALSE);
  } else {
    while (!endOfTable(chk)) {
      interpret(chk->exp);
      if (!(Abool)pop()) {
	if (execute)
	  interpret(chk->stms);
	return(FALSE);
      }
      chk++;
    }
    return(TRUE);
  }
}

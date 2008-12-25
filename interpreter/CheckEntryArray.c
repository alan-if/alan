#include "CheckEntryArray.h"

#include "main.h"
#include "inter.h"
#include "stack.h"


/*======================================================================*/
Bool checksFailed(Aaddr adr,	/* IN - ACODE address to check array */
		Bool execute		/* IN - Execute the ELSE clause if it fails ? */
)
{
	CheckEntry *chk = (CheckEntry *) pointerTo(adr);
	if (chk->exp == 0) {
		if (execute == EXECUTE_CHECK_BODY_ON_FAIL)
			interpret(chk->stms);
		return TRUE;
	} else {
		while (!endOfTable(chk)) {
			if (!evaluate(chk->exp)) {
				if (execute == EXECUTE_CHECK_BODY_ON_FAIL)
					interpret(chk->stms);
				return TRUE;
			}
			chk++;
		}
		return FALSE;
	}
}

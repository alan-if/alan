/*----------------------------------------------------------------------*\

  act.c

  Action routines

\*----------------------------------------------------------------------*/

#include "act.h"

/* Import */

#ifdef HAVE_GLK
#define MAP_STDIO_TO_GLK
#include "glkio.h"
#endif

#include "main.h"
#include "parse.h"
#include "AltInfo.h"
#include "AltInfoArray.h"
#include "CheckEntryArray.h"



/*----------------------------------------------------------------------*/
static void executeCommand(void)
{
	AltInfo *altInfos;
	int altIndex;

	fail = FALSE;

	altInfos = findAllAlternatives();

	if (anyCheckFailed(altInfos, EXECUTE_CHECK_BODY_ON_FAIL))
		return;

	/* Check for anything to execute... */
	if (!anythingToExecute(altInfos))
		error(M_CANT0);

	/* Now perform actions! First try any BEFORE or ONLY from inside out */
	for (altIndex = lastAltInfoIndex(altInfos); altIndex >= 0; altIndex--) {
		if (altInfos[altIndex].alt != NULL)
			if (altInfos[altIndex].alt->qual == (Aword)Q_BEFORE || altInfos[altIndex].alt->qual == (Aword)Q_ONLY) {
				if (!executedOk(&altInfos[altIndex]))
					return;
				if (altInfos[altIndex].alt->qual == (Aword)Q_ONLY)
					return;
			}
	}

	/* Then execute any not declared as AFTER, i.e. the default */
	for (altIndex = 0; !altInfos[altIndex].end; altIndex++) {
		if (altInfos[altIndex].alt != 0)
			if (altInfos[altIndex].alt->qual != (Aword)Q_AFTER)
				if (!executedOk(&altInfos[altIndex]))
					return;
	}

	/* Finally, the ones declared as AFTER */
	for (altIndex = lastAltInfoIndex(altInfos); altIndex >= 0; altIndex--) {
		if (altInfos[altIndex].alt != 0)
			if (!executedOk(&altInfos[altIndex]))
				return;
	}
}


/* A parameter position with code == 0 means this is a multiple position. We must loop
 * over this position (and replace it by each present in the matched list)
 */
static int findMultiplePosition(ParamEntry *parameters) {
	int multiplePosition;
	for (multiplePosition = 0; parameters[multiplePosition].instance != -1; multiplePosition++)
		if (parameters[multiplePosition].instance == 0)
			return multiplePosition;
	return -1;
}

/*======================================================================

  action()

  Execute all activities commanded. Handles possible multiple actions
  such as THEM or lists of objects.

 */
void action(ParamEntry *parameters, ParamEntry *matched)
{
	int i, multiplePosition;
	char marker[10];

	multiplePosition = findMultiplePosition(parameters);
	if (multiplePosition != -1) {
		sprintf(marker, "($%d)", multiplePosition+1); /* Prepare a printout with $1/2/3 */
		for (i = 0; matched[i].instance != EOF; i++) {
			parameters[multiplePosition] = matched[i];
			output(marker);
			executeCommand();
			if (matched[i+1].instance != EOF)
				para();
		}
		parameters[multiplePosition].instance = 0;
	} else
		executeCommand();
}

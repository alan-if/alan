/*----------------------------------------------------------------------*\

    act.c

    Action routines

\*----------------------------------------------------------------------*/

#include "act.h"

/* Import */

#include "AltInfo.h"
#include "output.h"
#include "msg.h"



/*----------------------------------------------------------------------*/
static void executeCommand(void)
{
    AltInfo *altInfos;
    int altIndex;

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


/*======================================================================

action()

Execute all activities commanded. Handles possible multiple actions
such as THEM or lists of objects.

*/
void action(Parameter parameters[], Parameter multipleMatches[])
{
    int i, multiplePosition;
    char marker[10];

    multiplePosition = findMultiplePosition(parameters);
    if (multiplePosition != -1) {
	sprintf(marker, "($%d)", multiplePosition+1); /* Prepare a printout with $1/2/3 */
	for (i = 0; multipleMatches[i].instance != EOF; i++) {
	    parameters[multiplePosition] = multipleMatches[i];
	    output(marker);
	    executeCommand();
	    if (multipleMatches[i+1].instance != EOF)
		para();
	}
	parameters[multiplePosition].instance = 0;
    } else
	executeCommand();
}

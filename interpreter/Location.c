/*----------------------------------------------------------------------*\

location.c

\*----------------------------------------------------------------------*/
#include "Location.h"

#include "instance.h"
#include "options.h"
#include "word.h"
#include "inter.h"
#include "lists.h"
#include "checkentry.h"
#include "debug.h"
#include "memory.h"
#include "dictionary.h"
#include "output.h"
#include "msg.h"
#include "current.h"


/*======================================================================*/
void go(int location, int dir)
{
    ExitEntry *theExit;
    Bool ok;
    Aword oldloc;

    theExit = (ExitEntry *) pointerTo(instances[location].exits);
    if (instances[location].exits != 0)
	while (!isEndOfArray(theExit)) {
	    if (theExit->code == dir) {
		ok = TRUE;
		if (theExit->checks != 0) {
		    if (sectionTraceOption) {
			printf("\n<EXIT %d(%s) from ", dir,
			       (char *)pointerTo(dictionary[playerWords[currentWordIndex-1].code].string));
			traceSay(location);
			printf("(%d), Checking:>\n", location);
		    }
		    ok = !checksFailed(theExit->checks, EXECUTE_CHECK_BODY_ON_FAIL);
		}
		if (ok) {
		    oldloc = location;
		    if (theExit->action != 0) {
			if (sectionTraceOption) {
			    printf("\n<EXIT %s(%d) from ",
				   (char *)pointerTo(dictionary[playerWords[currentWordIndex-1].code].string), dir);
			    traceSay(location);
			    printf("(%d), Executing:>\n", location);
			}
			interpret(theExit->action);
		    }
		    /* Still at the same place? */
		    if (where(HERO, FALSE) == oldloc) {
			if (sectionTraceOption) {
			    printf("\n<EXIT %s(%d) from ",
				   (char *)pointerTo(dictionary[playerWords[currentWordIndex-1].code].string), dir);
			    traceSay(location);
			    printf("(%d), Moving:>\n", location);
			}
			locate(HERO, theExit->target);
		    }
		}
		return;
	    }
	    theExit++;
	}
    error(M_NO_WAY);
}


/*======================================================================*/
Bool exitto(int to, int from)
{
    ExitEntry *theExit;

    if (instances[from].exits == 0)
	return FALSE; /* No exits */

    for (theExit = (ExitEntry *) pointerTo(instances[from].exits); !isEndOfArray(theExit); theExit++)
	if (theExit->target == to)
	    return TRUE;

    return FALSE;
}


/*======================================================================*/
void look(void)
{
    int i;

    /* Set describe flag for all objects and actors */
    for (i = 1; i <= header->instanceMax; i++)
        admin[i].alreadyDescribed = FALSE;

    if (anyOutput)
        para();

    setSubHeaderStyle();
    sayInstance(current.location);
    setNormalStyle();

    newline();
    capitalize = TRUE;
    if (describe(current.location))
        describeInstances();
}




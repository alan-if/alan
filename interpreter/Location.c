#include "Location.h"

#include "acode.h"
#include "types.h"
#include "main.h"
#include "options.h"
#include "parse.h"
#include "inter.h"
#include "exe.h"
#include "act.h"
#include "CheckEntryArray.h"
#include "debug.h"


/*======================================================================*/
void go(int location, int dir)
{
	ExitEntry *theExit;
	Bool ok;
	Aword oldloc;

	theExit = (ExitEntry *) pointerTo(instances[location].exits);
	if (instances[location].exits != 0)
		while (!endOfTable(theExit)) {
			if (theExit->code == dir) {
				ok = TRUE;
				if (theExit->checks != 0) {
					if (sectionTraceOption) {
						printf("\n<EXIT %d(%s) from ", dir,
								(char *)pointerTo(dictionary[playerWords[wordIndex-1].code].string));
						traceSay(location);
						printf("(%d), Checking:>\n", location);
					}
					ok = tryChecks(theExit->checks, EXECUTE);
				}
				if (ok) {
					oldloc = location;
					if (theExit->action != 0) {
						if (sectionTraceOption) {
							printf("\n<EXIT %s(%d) from ",
									(char *)pointerTo(dictionary[playerWords[wordIndex-1].code].string), dir);
							traceSay(location);
							printf("(%d), Executing:>\n", location);
						}
						interpret(theExit->action);
					}
					/* Still at the same place? */
					if (where(HERO, FALSE) == oldloc) {
						if (sectionTraceOption) {
							printf("\n<EXIT %s(%d) from ",
									(char *)pointerTo(dictionary[playerWords[wordIndex-1].code].string), dir);
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




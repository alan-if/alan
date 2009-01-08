#include "Location.h"

#include "instance.h"
#include "options.h"
#include "parse.h"
#include "inter.h"
#include "exe.h"
#include "lists.h"
#include "checkentry.h"
#include "debug.h"
#include "memory.h"
#include "dictionary.h"

// TODO Remove dependency on main.h
// TODO Move error() to where?
#include "main.h"


/*======================================================================*/
void go(int location, int dir)
{
	ExitEntry *theExit;
	Bool ok;
	Aword oldloc;

	theExit = (ExitEntry *) pointerTo(instances[location].exits);
	if (instances[location].exits != 0)
		while (!isEndOfList(theExit)) {
			if (theExit->code == dir) {
				ok = TRUE;
				if (theExit->checks != 0) {
					if (sectionTraceOption) {
						printf("\n<EXIT %d(%s) from ", dir,
								(char *)pointerTo(dictionary[playerWords[wordIndex-1].code].string));
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


/*======================================================================*/
Bool exitto(int to, int from)
{
  ExitEntry *theExit;

  if (instances[from].exits == 0)
    return FALSE; /* No exits */

  for (theExit = (ExitEntry *) pointerTo(instances[from].exits); !isEndOfList(theExit); theExit++)
    if (theExit->target == to)
      return TRUE;

  return FALSE;
}




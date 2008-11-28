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
static AltEntry *findAlternative(Aaddr verbTableAddress, int verbCode,
					    Aint parameter)
{
  AltEntry *alt;
  VerbEntry *verbEntry;

  if (verbTableAddress == 0) return NULL;

  for (verbEntry = (VerbEntry *) pointerTo(verbTableAddress); !endOfTable(verbEntry); verbEntry++)
    if (verbEntry->code == verbCode) {
      for (alt = (AltEntry *) pointerTo(verbEntry->alts); !endOfTable(alt); alt++) {
	if (alt->param == parameter || alt->param == 0)
	  return alt;
      }
      return NULL;
    }
  return NULL;
}



/*----------------------------------------------------------------------*/
static AltEntry *alternativeFinder(
    Aint parameter,		/* IN - Which parameter to match */
    Aint theInstance,		/* IN - Which instance to check */
    Aint theClass		/* IN - Which class to check */
)
{
  if (theClass != NO_CLASS)
    return findAlternative(class[theClass].verbs, current.verb, parameter);
  else if (theInstance != NO_INSTANCE)
    return findAlternative(instances[theInstance].verbs, current.verb, parameter);
  else
    return findAlternative(header->verbTableAddress, current.verb, parameter);
}


/*----------------------------------------------------------------------*/
static AltInfo *findAllAlternatives(void) {
  int parameter;
  AltInfo altInfos[1000];
  altInfos[0].end = TRUE;

  addGlobalAlternatives(altInfos, &alternativeFinder);

  addAlternativesFromLocation(altInfos, current.location, &alternativeFinder);

  for (parameter = 1; parameters[parameter-1].instance != EOF; parameter++) {
    addAlternativesFromParameter(altInfos, parameter, &alternativeFinder);
  }
  return duplicateAltInfoArray(altInfos);
}


/*======================================================================*/
Bool possible(void)
{
  AltInfo *altInfos;

  altInfos = findAllAlternatives();

  if (!checksPerformedOk(altInfos, DONT_EXECUTE))
    return FALSE;

  return anythingToExecute(altInfos);
}


/*----------------------------------------------------------------------*/
static void executeCommand(void)
{
  AltInfo *altInfos;
  int altIndex;

  fail = FALSE;

  altInfos = findAllAlternatives();

  if (!checksPerformedOk(altInfos, EXECUTE)) return;

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
void action(ParameterList plst)
{
  int i, mpos;
  char marker[10];

  if (plural) {
    /*
       A parameter position with code == 0 means this is a multiple position. We must loop
       over this position (and replace it by each present in the plst)
     */
    for (mpos = 0; parameters[mpos].instance != 0; mpos++); /* Find multiple position */
    sprintf(marker, "($%d)", mpos+1); /* Prepare a printout with $1/2/3 */
    for (i = 0; plst[i].instance != EOF; i++) {
      parameters[mpos] = plst[i];
      output(marker);
      executeCommand();
      if (plst[i+1].instance != EOF)
        para();
    }
    parameters[mpos].instance = 0;
  } else
    executeCommand();
}

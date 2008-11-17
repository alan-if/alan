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

#include "types.h"
#include "main.h"
#include "inter.h"
#include "exe.h"
#include "stack.h"
#include "parse.h"
#include "debug.h"
#include "syserr.h"
#include "options.h"
#include "AltInfo.h"
#include "AltInfoArray.h"
#include "CheckArray.h"


/*----------------------------------------------------------------------*/
static int countInContainer(int cnt)	/* IN - the container to count in */
{
  int i, j = 0;
  
  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt, TRUE))
      /* Then it's in this container also */
      j++;
  return(j);
}



/*----------------------------------------------------------------------*/
static int sumAttributesInContainer(
     Aword atr,			/* IN - the attribute to sum over */
     Aword cnt			/* IN - the container to sum */
) {
  int i;
  int sum = 0;

  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt, TRUE)) {		/* Then it's directly in this cont */
      if (instance[i].container != 0)	/* This is also a container! */
	sum = sum + sumAttributesInContainer(atr, i);
      sum = sum + attributeOf(i, atr);
    }
  return(sum);
}


/*======================================================================*/
Bool checkContainerLimits(
     Aword cnt,			/* IN - Container code */
     Aword obj			/* IN - The object to add */
) {
  LimEntry *lim;
  Aword props;

  if (!isContainer(cnt))
    syserr("Checking limits for a non-container.");

  /* Find the container properties */
  props = instance[cnt].container;

  if (container[props].limits != 0) { /* Any limits at all? */
    for (lim = (LimEntry *) pointerTo(container[props].limits); !endOfTable(lim); lim++)
      if (lim->atr == 1-I_COUNT) {
	if (countInContainer(cnt) >= lim->val) {
	  interpret(lim->stms);
	  return(TRUE);		/* Limit check failed */
	}
      } else {
	if (sumAttributesInContainer(lim->atr, cnt) + attributeOf(obj, lim->atr) > lim->val) {
	  interpret(lim->stms);
	  return(TRUE);
	}
      }
  }
  return(FALSE);
}



/*======================================================================*/
void go(int location, int dir)
{
  ExitEntry *theExit;
  Bool ok;
  Aword oldloc;

  theExit = (ExitEntry *) pointerTo(instance[location].exits);
  if (instance[location].exits != 0)
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
    return findAlternative(instance[theInstance].verbs, current.verb, parameter);
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


/*----------------------------------------------------------------------*/
static Bool anythingToExecute(AltInfo altInfo[])
{
  int altIndex;

  /* Check for anything to execute... */
  for (altIndex = 0; !altInfo[altIndex].end; altIndex++)
    if (executable(&altInfo[altIndex]))
      return TRUE;
  return FALSE;
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
void action(
     ParameterList plst		/* IN - Plural parameter list */
)
{
  int i, mpos;
  char marker[10];

  if (plural) {
    /*
       The code == 0 means this is a multiple position. We must loop
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

/*----------------------------------------------------------------------*\

  act.c

  Action routines

\*----------------------------------------------------------------------*/
#include "types.h"
#include "main.h"
#include "inter.h"
#include "exe.h"
#include "stack.h"
#include "parse.h"
#include "debug.h"


/* Private data */
#define EXECUTE TRUE
#define DONT_EXECUTE FALSE


/*----------------------------------------------------------------------
  count()

  Count the number of items in a container.

  */
#ifdef _PROTOTYPES_
static int count(int cnt)	/* IN - the container to count */
#else
static int count(cnt)
     int cnt;			/* IN - the container to count */
#endif
{
  int i, j = 0;
  
  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt))
      /* Then it's in this container also */
      j++;
  return(j);
}



/*----------------------------------------------------------------------
  sumatr()

  Sum the values of one attribute in a container. Recursively.

  */
#ifdef _PROTOTYPES_
static int sumatr(
     Aword atr,			/* IN - the attribute to sum over */
     Aword cnt			/* IN - the container to sum */
)
#else
static int sumatr(atr, cnt)
     Aword atr;			/* IN - the attribute to sum over */
     Aword cnt;			/* IN - the container to sum */
#endif
{
  int i;
  int sum = 0;

  for (i = 1; i <= header->instanceMax; i++)
    if (isObj(i)) {
      if (instance[i].location == cnt) { /* Then it's in this container */
	if (instance[i].container != 0)	/* This is also a container! */
	  sum = sum + sumatr(atr, i);
	sum = sum + attribute(i, atr);
      }
    }
  return(sum);
}


/*======================================================================
  checklim()

  Checks if a limit for a container is exceeded.

  */
#ifdef _PROTOTYPES_
Boolean checklim(
     Aword cnt,			/* IN - Container code */
     Aword obj			/* IN - The object to add */
)
#else
Boolean checklim(cnt, obj)
     Aword cnt;			/* IN - Container code */
     Aword obj;			/* IN - The object to add */
#endif
{
  LimEntry *lim;
  Aword props;

  fail = TRUE;
  if (!isCnt(cnt))
    syserr("Checking limits for a non-container.");

  /* Find the container properties */
  props = instance[cnt].container;

  if (container[props].limits != 0) { /* Any limits at all? */
    for (lim = (LimEntry *) pointerTo(container[props].limits); !endOfTable(lim); lim++)
      if (lim->atr == I_COUNT) {
	if (count(cnt) >= lim->val) {
	  interpret(lim->stms);
	  return(TRUE);		/* Limit check failed */
	}
      } else {
	if (sumatr(lim->atr, cnt) + attribute(obj, lim->atr) > lim->val) {
	  interpret(lim->stms);
	  return(TRUE);
	}
      }
  }
  fail = FALSE;
  return(FALSE);
}



/*======================================================================
  trycheck()

  Tries a check, returns TRUE if it passed, FALSE else.

  */
Boolean trycheck(
     Aaddr adr,			/* IN - ACODE address to check table */
     Boolean execute		/* IN - Act if it fails ? */
)
{
  ChkEntry *chk;

  chk = (ChkEntry *) pointerTo(adr);
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


/*======================================================================
  go()

  Move hero in a direction.

  */
#ifdef _PROTOTYPES_
void go(int dir)
#else
void go(dir)
     int dir;
#endif
{
  ExitEntry *theExit;
  Boolean ok;
  Aword oldloc;

  theExit = (ExitEntry *) pointerTo(instance[current.location].exits);
  if (instance[current.location].exits != 0)
    while (!endOfTable(theExit)) {
      if (theExit->code == dir) {
	ok = TRUE;
	if (theExit->checks != 0) {
	  if (trcflg) {
	    printf("\n<EXIT %d (%s) from %d (", dir,
		   (char *)pointerTo(dict[wrds[wrdidx-1]].wrd), current.location);
	    debugsay(current.location);
	    printf("), Checking:>\n");
	  }
	  ok = trycheck(theExit->checks, EXECUTE);
	}
	if (ok) {
	  oldloc = current.location;
	  if (theExit->action != 0) {
	    if (trcflg) {
	      printf("\n<EXIT %d (%s) from %d (", dir, 
		     (char *)pointerTo(dict[wrds[wrdidx-1]].wrd), current.location);
	      debugsay(current.location);
	      printf("), Executing:>\n");
	    }	    
	    interpret(theExit->action);
	  }
	  /* Still at the same place? */
	  if (where(HERO) == oldloc) {
	    if (trcflg) {
	      printf("\n<EXIT %d (%s) from %d (", dir, 
		     (char *)pointerTo(dict[wrds[wrdidx-1]].wrd), current.location);
	      debugsay(current.location);
	      printf("), Moving:>\n");
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
static AltEntry *findAlternativeInVerbList(Aaddr verbListAddress,
					   Aint parameter)
{
  AltEntry *alt;
  VerbEntry *verb;

  if (verbListAddress == 0) return NULL;

  for (verb = (VerbEntry *) pointerTo(verbListAddress); !endOfTable(verb); verb++)
    if (verb->code == current.verb) {
      for (alt = (AltEntry *) pointerTo(verb->alts); !endOfTable(alt); alt++)
	if (alt->param == parameter || alt->param == 0)
	  return alt;
      return NULL;
    }
  return NULL;
}



/*----------------------------------------------------------------------

  findalt()

  Find the verb alternative wanted for an instance and return
  the address to it. If the instance == 0 then look globally.

 */
static AltEntry *findalt(
     Aint inInstance,		/* IN - Which instance to check (0=global) */
     Aint parameter		/* IN - Which parameter to match */
)
{
  Aaddr verbListAddress;
  AltEntry *foundAlt;
  Aint parent;

  if (inInstance == 0)
    return findAlternativeInVerbList(header->verbTableAddress, parameter);

  foundAlt = findAlternativeInVerbList(instance[inInstance].verbs, parameter);
  parent = instance[inInstance].parent;
  while (!foundAlt && parent) {
    foundAlt = findAlternativeInVerbList(class[parent].verbs, parameter);
    parent = class[parent].parent;
  }
  return foundAlt;
}


/*----------------------------------------------------------------------*/
static AltEntry *findAlternativeInInstance(
     Aint inInstance,		/* IN - Which instance to check (0=global) */
     Aint parameter		/* IN - Which parameter to match */
)
{
  if (inInstance == 0)
    return findAlternativeInVerbList(header->verbTableAddress, parameter);
  else
    return findAlternativeInVerbList(instance[inInstance].verbs, parameter);
}


/*----------------------------------------------------------------------*/
static AltEntry *findAlternativeInClass(
     Aint inClass,		/* IN - Which class to check */
     Aint parameter		/* IN - Which parameter to match */
)
{
  return findAlternativeInVerbList(class[inClass].verbs, parameter);
}


/*----------------------------------------------------------------------
  An info node about the Alternatives found and possibly executed
  ----------------------------------------------------------------------*/
typedef struct AltInfo {
  Boolean end;
  AltEntry *alt;
  Boolean done;
  int level;			/* 0 - Global, 1 - location, 2.. - parameter */
  Aint instance;
  int parameter;
  int class;
} AltInfo;




/*----------------------------------------------------------------------*/
static void findAllAlternatives(AltInfo alt[]) {
  int paramIndex;
  int altIndex = 0;
  int parent;

  alt[altIndex].alt = findAlternativeInInstance(0, 0);
  if (alt[altIndex].alt != NULL) {
    alt[altIndex].level = 0;
    alt[altIndex].parameter = 0; /* Globals are executed for first param */
    alt[altIndex].instance = params[0].code;
    alt[altIndex].class = 0;
    alt[altIndex].done = FALSE;
    alt[altIndex].end = FALSE;
    altIndex++;
  }

  alt[altIndex].alt = findAlternativeInInstance(current.location, 0);
  if (alt[altIndex].alt != NULL) {
    alt[altIndex].level = 1;
    alt[altIndex].parameter = -1;		/* in location */
    alt[altIndex].instance = current.location;
    alt[altIndex].class = 0;
    alt[altIndex].done = FALSE;
    alt[altIndex].end = FALSE;
    altIndex++;
  }
  parent = instance[current.location].parent;
  while (parent) {
    alt[altIndex].alt = findAlternativeInClass(parent, 0);
    if (alt[altIndex].alt != NULL) {
      alt[altIndex].level = 1;
      alt[altIndex].parameter = -1;		/* in location */
      alt[altIndex].instance = current.location;
      alt[altIndex].class = parent;
      alt[altIndex].done = FALSE;
      alt[altIndex].end = FALSE;
      altIndex++;
    }
    parent = class[parent].parent;
  }

  for (paramIndex = 0; params[paramIndex].code != EOF; paramIndex++) {
    if (!isLit(params[paramIndex].code)) {
      alt[altIndex].alt = findAlternativeInInstance(params[paramIndex].code, paramIndex+1);
      if (alt[altIndex].alt != NULL) {
	alt[altIndex].level = 2;
	alt[altIndex].parameter = paramIndex;
	alt[altIndex].instance = params[paramIndex].code;
	alt[altIndex].class = 0;
	alt[altIndex].done = FALSE;
	alt[altIndex].end = FALSE;
	altIndex++;
      }
    }
    if (!isLit(params[paramIndex].code)) {
      parent = instance[params[paramIndex].code].parent;
      while (parent) {
	alt[altIndex].alt = findAlternativeInClass(parent, paramIndex+1);
	if (alt[altIndex].alt != NULL) {
	  alt[altIndex].level = 2;
	  alt[altIndex].parameter = paramIndex;
	  alt[altIndex].instance = params[paramIndex].code;
	  alt[altIndex].class = parent;
	  alt[altIndex].done = FALSE;
	  alt[altIndex].end = FALSE;
	  altIndex++;
	}
	parent = class[parent].parent;
      }
    }
  }
  alt[altIndex].end = TRUE;
}


/*----------------------------------------------------------------------*/
static void traceCheck(AltInfo alt)
{
  printf("\n<VERB %d, ", current.verb);
  switch (alt.level) {
  case 0: printf("GLOBAL"); break;
  case 1: printf("in LOCATION"); break;
  default:
    printf("in parameter #%d", alt.parameter+1);
    if (alt.class != 0)
      printf(", inherited from class %d", alt.class);
    break;
  }
  printf(", CHECK:>\n");
}


/*----------------------------------------------------------------------*/
static Boolean executeCheckOK(AltInfo alt, Boolean execute)
{
  if (alt.alt != NULL && alt.alt->checks != 0) {
    if (trcflg && execute)
      traceCheck(alt);
    if (!trycheck(alt.alt->checks, execute)) return FALSE;
    if (fail) return FALSE;
  }
  return TRUE;
}


/*----------------------------------------------------------------------*/
static Boolean performChecksOK(AltInfo altInfo[], Boolean execute)
{
  int altIndex;

  for (altIndex = 0; !altInfo[altIndex].end; altIndex++) {
    current.instance = altInfo[altIndex].instance;
    if (!executeCheckOK(altInfo[altIndex], execute))
      return FALSE;
  }
  return TRUE;
}


/*----------------------------------------------------------------------*/
static Boolean anythingToExecute(AltInfo altInfo[])
{
  int altIndex;

  /* Check for anything to execute... */
  for (altIndex = 0; !altInfo[altIndex].end; altIndex++)
    if (altInfo[altIndex].alt != NULL && altInfo[altIndex].alt->action != 0)
      return TRUE;
  return FALSE;
}



/*======================================================================*/
Boolean possible(void)
{
  AltInfo alt[MAXPARAMS+2];

  findAllAlternatives(alt);

  if (!performChecksOK(alt, DONT_EXECUTE))
    return FALSE;

  return anythingToExecute(alt);
}


/*----------------------------------------------------------------------*/
static void traceExecution(AltInfo *alt)
{
  if (trcflg) {
    printf("\n<VERB %d", current.verb);
    if (alt->level == 0)
      printf(", GLOBAL");
    else if (alt->level == 1)
      printf(", in LOCATION");
    else
      printf(", in parameter #%d", alt->parameter+1);
    if (alt->class != 0)
      printf(", inherited from class %d", alt->class);
    printf(", DOES");
    switch (alt->alt->qual) {
    case Q_BEFORE: printf(" (BEFORE)"); break;
    case Q_ONLY: printf(" (ONLY)"); break;
    case Q_AFTER: printf(" (AFTER)"); break;
    case Q_DEFAULT: break;
    }
    printf(":>\n");  }
}


/*----------------------------------------------------------------------*/
static Boolean executeOK(AltInfo *altInfo)
{
  fail = FALSE;
  if (!altInfo->done && altInfo->alt->action != 0) {
    traceExecution(altInfo);
    current.instance = altInfo->instance;
    interpret(altInfo->alt->action);
  }
  altInfo->done = TRUE;
  return !fail;
}

/*----------------------------------------------------------------------*/
static int lastAltInfo(AltInfo altInfo[])
{
  int altIndex;

  for (altIndex = -1; !altInfo[altIndex+1].end; altIndex++)
    /* Step to last alternative */;
  return(altIndex);
}



/*----------------------------------------------------------------------*/
static void executeCommand(void)
{
  AltInfo altInfo[MAXPARAMS+2];
  int altIndex;

  fail = FALSE;

  findAllAlternatives(altInfo);

  if (!performChecksOK(altInfo, EXECUTE)) return;

  /* Check for anything to execute... */
  if (!anythingToExecute(altInfo))
    error(M_CANT0);

  /* Now perform actions! First try any BEFORE or ONLY from inside out */
  for (altIndex = lastAltInfo(altInfo); altIndex >= 0; altIndex--) {
    if (altInfo[altIndex].alt != NULL)
      if (altInfo[altIndex].alt->qual == (Aword)Q_BEFORE || altInfo[altIndex].alt->qual == (Aword)Q_ONLY) {
	if (!executeOK(&altInfo[altIndex]))
	  return;
	if (altInfo[altIndex].alt->qual == (Aword)Q_ONLY)
	  return;
      }
  }
  
  /* Then execute any not declared as AFTER, i.e. the default */
  for (altIndex = 0; !altInfo[altIndex].end; altIndex++) {
    if (altInfo[altIndex].alt != 0)
      if (altInfo[altIndex].alt->qual != (Aword)Q_AFTER)
	if (!executeOK(&altInfo[altIndex]))
	  return;
  }

  /* Finally, the ones declared as AFTER */
  for (altIndex = lastAltInfo(altInfo); altIndex >= 0; altIndex--) {
    if (altInfo[altIndex].alt != 0)
      if (!executeOK(&altInfo[altIndex]))
	return;
  }
}



/*======================================================================

  action()

  Execute all activities commanded. Handles possible multiple actions
  such as THEM or lists of objects.

  */
#ifdef _PROTOTYPES_
void action(
     ParamEntry plst[]		/* IN - Plural parameter list */
)
#else
void action(plst)
     ParamEntry plst[];
#endif
{
  int i, mpos;
  char marker[10];

  if (plural) {
    /*
       The code == 0 means this is a multiple position. We must loop
       over this position (and replace it by each present in the plst)
     */
    for (mpos = 0; params[mpos].code != 0; mpos++); /* Find multiple position */
    sprintf(marker, "($%d)", mpos+1); /* Prepare a printout with $1/2/3 */
    for (i = 0; plst[i].code != EOF; i++) {
      params[mpos] = plst[i];
      output(marker);
      executeCommand();
      if (plst[i+1].code != EOF)
        para();
    }
    params[mpos].code = 0;
  } else
    executeCommand();
}

#include "AltInfo.h"

/*----------------------------------------------------------------------*\

  An info node about the Verb Alternatives found and possibly executed

\*----------------------------------------------------------------------*/

#include "checkentry.h"
#include "debug.h"
#include "inter.h"
#include "lists.h"
#include "instance.h"
#include "options.h"
#include "parse.h"
#include "types.h"
#include "memory.h"

// TODO Remove dependency on main.h
#include "main.h"


/*======================================================================*/
void primeAltInfo(AltInfo *altInfo, int level, int parameter, int instance, int class)
{
  altInfo->level = level;
  altInfo->parameter = parameter;
  altInfo->instance = instance;
  altInfo->class = class;
  altInfo->done = FALSE;
  altInfo->end = FALSE;
}


/*----------------------------------------------------------------------*/
static char *idOfClass(int theClass) {
	return (char *)pointerTo(classes[theClass].id);
}


/*----------------------------------------------------------------------*/
static void traceInstanceAndItsClass(AltInfo *alt)
{
    traceSay(parameters[alt->parameter-1].instance);
    printf(")");
    if (alt->class != NO_CLASS)
      printf(", inherited from class #%d (%s)", alt->class, idOfClass(alt->class));
}


/*----------------------------------------------------------------------*/
static void traceAltInfo(AltInfo *alt) {
  switch (alt->level) {
  case GLOBAL_LEVEL:
    printf("GLOBAL");
    break;
  case LOCATION_LEVEL:
    printf("in location #%d (", alt->instance);
    traceInstanceAndItsClass(alt);
    break;
  case PARAMETER_LEVEL:
    printf("in parameter #%d (", alt->parameter);
    traceInstanceAndItsClass(alt);
    break;
  }
}


/*----------------------------------------------------------------------*/
static void traceVerbCheck(AltInfo *alt, Bool execute)
{
	if (sectionTraceOption && execute) {
		printf("\n<VERB %d, ", current.verb);
		traceAltInfo(alt);
		printf(", CHECK:>\n");
	}
}


/*======================================================================*/
Bool checkFailed(AltInfo *altInfo, Bool execute)
{
	if (altInfo->alt != NULL && altInfo->alt->checks != 0) {
		traceVerbCheck(altInfo, execute);
		// TODO Why does this not generate a regression error with !
		// Need a new regression case?
		if (checksFailed(altInfo->alt->checks, execute)) return TRUE;
		if (fail) return TRUE;
	}
	return FALSE;
}


/*----------------------------------------------------------------------*/
static void traceVerbExecution(AltInfo *alt)
{
  if (sectionTraceOption) {
    printf("\n<VERB %d, ", current.verb);
    traceAltInfo(alt);
    printf(", DOES");
    switch (alt->alt->qual) {
    case Q_BEFORE: printf(" (BEFORE)"); break;
    case Q_ONLY: printf(" (ONLY)"); break;
    case Q_AFTER: printf(" (AFTER)"); break;
    case Q_DEFAULT: break;
    }
    printf(":>\n");  }
}


/*======================================================================*/
Bool executedOk(AltInfo *altInfo)
{
  fail = FALSE;
  if (!altInfo->done && altInfo->alt->action != 0) {
    traceVerbExecution(altInfo);
    current.instance = altInfo->instance;
    interpret(altInfo->alt->action);
  }
  altInfo->done = TRUE;
  return !fail;
}


/*======================================================================*/
Bool executable(AltInfo *altInfo) {
  return altInfo->alt != NULL && altInfo->alt->action != 0;
}


/*======================================================================*/
AltInfo *duplicateAltInfoArray(AltInfo original[]) {
	int size;
	AltInfo *duplicate;

	for (size = 0; original[size].end != TRUE; size++);
	size++;
	duplicate = allocate(size*sizeof(AltInfo));
	memcpy(duplicate, original, size*sizeof(AltInfo));
	return duplicate;
}


/*======================================================================*/
int lastAltInfoIndex(AltInfo altInfo[])
{
	int altIndex;

	for (altIndex = -1; !altInfo[altIndex+1].end; altIndex++)
		/* Step to last alternative */;
	return(altIndex);
}


/*----------------------------------------------------------------------*/
static AltInfo *nextFreeAltInfo(AltInfoArray altInfos) {
	return &altInfos[lastAltInfoIndex(altInfos)+1];
}


/*----------------------------------------------------------------------*/
static void addAlternative(
		AltInfoArray altInfos,
		int level,
		Aint parameterNumber,
		Aint theClass,
		Aint theInstance,
		AltEntryFinder finder
) {
	AltInfo *altInfoP = nextFreeAltInfo(altInfos);

	altInfoP->alt = (*finder)(parameterNumber, theInstance, theClass);
	if (altInfoP->alt != NULL) {
		primeAltInfo(altInfoP, level, parameterNumber, theInstance, theClass);
		altInfoP[1].end = TRUE;
	}
}


/*----------------------------------------------------------------------*/
static void addGlobalAlternatives(
		AltInfoArray altInfos,
		AltEntryFinder finder
) {
	addAlternative(altInfos, GLOBAL_LEVEL, NO_PARAMETER, NO_CLASS, NO_INSTANCE, finder);
}


/*----------------------------------------------------------------------*/
static void addAlternativesFromParents(
		AltInfoArray altInfos,
		int level,
		Aint parameterNumber,
		Aint theClass,
		Aint theInstance,
		AltEntryFinder finder
){
	if (classes[theClass].parent != 0)
		addAlternativesFromParents(altInfos, level,
				parameterNumber,
				classes[theClass].parent,
				theInstance,
				finder);

	addAlternative(altInfos, level, parameterNumber, theClass, theInstance, finder);
}


/*----------------------------------------------------------------------*/
static void addAlternativesFromLocation(
		AltInfoArray altInfos,
		Aint location,
		AltEntryFinder finder
) {
	if (admin[location].location != 0)
		addAlternativesFromLocation(altInfos, admin[location].location, finder);

	addAlternativesFromParents(altInfos,
			LOCATION_LEVEL,
			NO_PARAMETER,
			instances[location].parent,
			location,
			finder);

	addAlternative(altInfos, LOCATION_LEVEL, NO_PARAMETER, NO_CLASS, location, finder);
}


/*----------------------------------------------------------------------*/
static void addAlternativesFromParameter(
		AltInfoArray altInfos,
		int parameterNumber,
		AltEntryFinder finder
) {
	Aint parent;
	Aint theInstance = parameters[parameterNumber-1].instance;

	if (isLiteral(theInstance))
		parent = literal[parameterNumber].class;
	else
		parent = instances[theInstance].parent;
	addAlternativesFromParents(altInfos, PARAMETER_LEVEL, parameterNumber, parent, theInstance, finder);

	if (!isLiteral(theInstance))
		addAlternative(altInfos, PARAMETER_LEVEL, parameterNumber, NO_CLASS, theInstance, finder);
}


/*======================================================================*/
Bool anyCheckFailed(AltInfoArray altInfo, Bool execute)
{
	int altIndex;

	for (altIndex = 0; !altInfo[altIndex].end; altIndex++) {
		current.instance = altInfo[altIndex].instance;
		if (checkFailed(&altInfo[altIndex], execute))
			return TRUE;
	}
	return FALSE;
}


/*======================================================================*/
Bool anythingToExecute(AltInfo altInfo[])
{
	int altIndex;

	/* Check for anything to execute... */
	for (altIndex = 0; !altInfo[altIndex].end; altIndex++)
		if (executable(&altInfo[altIndex]))
			return TRUE;
	return FALSE;
}


/*----------------------------------------------------------------------*/
static AltEntry *findAlternative(Aaddr verbTableAddress, int verbCode, int parameter)
{
	AltEntry *alt;
	VerbEntry *verbEntry;

	if (verbTableAddress == 0) return NULL;

	for (verbEntry = (VerbEntry *) pointerTo(verbTableAddress); !isEndOfList(verbEntry); verbEntry++)
		if (verbEntry->code == verbCode) {
			for (alt = (AltEntry *) pointerTo(verbEntry->alts); !isEndOfList(alt); alt++) {
				if (alt->param == parameter || alt->param == 0)
					return alt;
			}
			return NULL;
		}
	return NULL;
}


/*----------------------------------------------------------------------*/
static AltEntry *alternativeFinder(int parameter, int theInstance, int theClass)
{
	if (theClass != NO_CLASS)
		return findAlternative(classes[theClass].verbs, current.verb, parameter);
	else if (theInstance != NO_INSTANCE)
		return findAlternative(instances[theInstance].verbs, current.verb, parameter);
	else
		return findAlternative(header->verbTableAddress, current.verb, parameter);
}


/*======================================================================*/
AltInfo *findAllAlternatives(void) {
	int parameterNumber;
	AltInfo altInfos[1000];
	altInfos[0].end = TRUE;

	addGlobalAlternatives(altInfos, &alternativeFinder);

	addAlternativesFromLocation(altInfos, current.location, &alternativeFinder);

	for (parameterNumber = 1; !isEndOfList(&parameters[parameterNumber-1]); parameterNumber++) {
		addAlternativesFromParameter(altInfos, parameterNumber, &alternativeFinder);
	}
	return duplicateAltInfoArray(altInfos);
}


/*======================================================================*/
Bool possible(void)
{
	Bool anything;

	AltInfo *allAlternatives = findAllAlternatives();

	if (anyCheckFailed(allAlternatives, DONT_EXECUTE_CHECK_BODY_ON_FAIL))
		return FALSE;

	anything = anythingToExecute(allAlternatives);
	free(allAlternatives);
	return anything;
}

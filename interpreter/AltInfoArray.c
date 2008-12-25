#include "AltInfoArray.h"

/* Imports: */

#include "AltInfo.h"
#include "main.h"
#include "parse.h"


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
  AltInfo *altInfoP;
  altInfoP = nextFreeAltInfo(altInfos);

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
  addAlternativesFromParents(altInfos,
			     PARAMETER_LEVEL,
			     parameterNumber,
			     parent,
			     theInstance,
			     finder);

  if (!isLiteral(theInstance)) {
    addAlternative(altInfos, PARAMETER_LEVEL, parameterNumber, NO_CLASS, theInstance, finder);
  }
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
    return findAlternative(classes[theClass].verbs, current.verb, parameter);
  else if (theInstance != NO_INSTANCE)
    return findAlternative(instances[theInstance].verbs, current.verb, parameter);
  else
    return findAlternative(header->verbTableAddress, current.verb, parameter);
}


/*======================================================================*/
AltInfo *findAllAlternatives(void) {
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



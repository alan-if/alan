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
  }
}


/*======================================================================*/
void addGlobalAlternatives(
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
  if (class[theClass].parent != 0)
    addAlternativesFromParents(altInfos, level,
			       parameterNumber,
			       class[theClass].parent,
			       theInstance,
			       finder);

  addAlternative(altInfos, level, parameterNumber, theClass, theInstance, finder);
}


/*======================================================================*/
void addAlternativesFromLocation(
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


/*======================================================================*/
void addAlternativesFromParameter(
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
Bool checksPerformedOk(AltInfoArray altInfo, Bool execute)
{
  int altIndex;

  for (altIndex = 0; !altInfo[altIndex].end; altIndex++) {
    current.instance = altInfo[altIndex].instance;
    if (!checkPerformedOk(&altInfo[altIndex], execute))
      return FALSE;
  }
  return TRUE;
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



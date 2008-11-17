#include "AltInfoArray.h"

/* Imports: */

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
static void addAlternativeFromThis(
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
  AltInfo *altInfoP = nextFreeAltInfo(altInfos);

  altInfoP->alt = (*finder)(NO_PARAMETER, NO_INSTANCE, NO_CLASS);
  if (altInfoP->alt != NULL) {
    primeAltInfo(altInfoP, GLOBAL_LEVEL, NO_PARAMETER, NO_INSTANCE, NO_CLASS);
  }
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

  addAlternativeFromThis(altInfos, level, parameterNumber, theClass, theInstance, finder);
}


/*======================================================================*/
void addAlternativesFromLocation(
    AltInfoArray altInfos,
    Aint location,
    AltEntryFinder finder
) {
  AltInfo *altInfoP;

  if (admin[location].location != 0)
    addAlternativesFromLocation(altInfos, admin[location].location, finder);

  addAlternativesFromParents(altInfos,
			     LOCATION_LEVEL,
			     NO_PARAMETER,
			     instance[location].parent,
			     location,
			     finder);

  altInfoP = nextFreeAltInfo(altInfos);
  altInfoP->alt = (*finder)(NO_PARAMETER, location, NO_CLASS);
  if (altInfoP->alt != NULL) {
    primeAltInfo(altInfoP, LOCATION_LEVEL, NO_PARAMETER, location, NO_CLASS);
  }
}


/*======================================================================*/
void addAlternativesFromParameter(
    AltInfoArray altInfos,
    int parameterNumber,
    AltEntryFinder finder
) {
  Aint parent;
  AltInfo *altInfoP;
  Aint theInstance = parameters[parameterNumber-1].instance;

  if (isLiteral(theInstance))
    parent = literal[parameterNumber].class;
  else
    parent = instance[theInstance].parent;
  addAlternativesFromParents(altInfos,
			     PARAMETER_LEVEL,
			     parameterNumber,
			     parent,
			     theInstance,
			     finder);

  if (!isLiteral(theInstance)) {
    altInfoP = nextFreeAltInfo(altInfos);
    altInfoP->alt = (*finder)(parameterNumber, theInstance, NO_CLASS);
    if (altInfoP->alt != NULL) {
      primeAltInfo(altInfoP, PARAMETER_LEVEL, parameterNumber, theInstance, NO_CLASS);
    }
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



#ifndef _ALTINFOARRAY_H_
#define _ALTINFOARRAY_H_
/*----------------------------------------------------------------------*\

  An array of AltInfo nodes

\*----------------------------------------------------------------------*/

#include "AltInfo.h"


/* TYPES */
typedef AltEntry *(*AltEntryFinder)(Aint parameterNumber, Aint theInstance, Aint theClass);

typedef AltInfo AltInfoArray[];


/* DATA */


/* FUNCTIONS */
extern AltInfo *duplicateAltInfoArray(AltInfoArray altInfos);
extern int lastAltInfoIndex(AltInfoArray altInfos);
extern void addGlobalAlternatives(AltInfoArray altInfos, AltEntryFinder finder);
extern void addAlternativesFromLocation(AltInfoArray altInfos, Aint location, AltEntryFinder finder);
extern void addAlternativesFromParameter(AltInfoArray altInfos, int paramIndex, AltEntryFinder);
extern Bool checksPerformedOk(AltInfoArray altInfos, Bool execute);
extern Bool anythingToExecute(AltInfoArray altInfos);

#endif

#ifndef _ALTINFOARRAY_H_
#define _ALTINFOARRAY_H_
/*----------------------------------------------------------------------*\

  An array of AltInfo nodes

\*----------------------------------------------------------------------*/

#include "AltInfo.h"

/* CONSTANTS */


/* TYPES */
typedef AltEntry *(*AltEntryFinder)(Aint parameterNumber, Aint theInstance, Aint theClass);

typedef AltInfo AltInfoArray[];


/* DATA */


/* FUNCTIONS */
extern AltInfo *duplicateAltInfoArray(AltInfoArray altInfos);
extern int lastAltInfoIndex(AltInfoArray altInfos);
extern Bool anyCheckFailed(AltInfoArray altInfos, Bool execute);
extern Bool anythingToExecute(AltInfoArray altInfos);
extern Bool possible(void);
extern AltInfo *findAllAlternatives(void);

#endif

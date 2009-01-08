#ifndef EVENT_H_
#define EVENT_H_
/*----------------------------------------------------------------------*\

	event

\*----------------------------------------------------------------------*/

/* IMPORTS */
#include "acode.h"
#include "types.h"

/* CONSTANTS */


/* TYPES */


/* DATA */
/* Event queue */
extern int eventQueueSize;
extern EventQueueEntry *eventQueue;
extern int eventQueueTop;       /* Event queue top pointer */
extern EventEntry *events;  /* Event table pointer */


/* FUNCTIONS */

#endif /* EVENT_H_ */

#ifndef _EVENT_H_
#define _EVENT_H_
/*----------------------------------------------------------------------*\

			       Event.h

			     Event Nodes

\*----------------------------------------------------------------------*/


/* Use other definitions */
#include "Srcp.h"
#include "Id.h"
#include "List.h"

#include "acode.h"


/* Types: */

typedef struct Event {		/* EVENT */
  Srcp srcp;			/* Source position */
  Id *id;			/* Name of this event */
  Aaddr namadr;			/* Address to name string (debug) */
  List *statements;		/* List of statements to execute */
  Aaddr stmadr;			/* ACODE address of event statements */
} Event;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Event node */
extern Event *newEvent(Srcp *srcp,
		       Id *id,
		       List *stms);

/* Analyze all Events */
extern void analyseEvents(void);

/* Generate code for all Events */
extern Aaddr generateEvents(void);

/* Dump an Event node */
extern void dumpEvent(Event *event);

#else
extern Event *newEvent();
extern void analyseEvents();
extern Aaddr generateEvents();
extern void dumpEvent();
#endif

#endif

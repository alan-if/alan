#ifndef _EVT_H_
#define _EVT_H_
/*----------------------------------------------------------------------*\

				EVT.H
			     Event Nodes

\*----------------------------------------------------------------------*/


/* Use other definitions */
#include "srcp.h"
#include "id.h"
#include "lst.h"

#include "acode.h"


/* Types: */

typedef struct EvtNod {		/* EVENT */
  Srcp srcp;			/* Source position */
  IdNode *id;			/* Name of this event */
  Aaddr namadr;			/* Address to name string (debug) */
  List *stms;			/* List of statements to execute */
  Aaddr stmadr;			/* ACODE address of event statements */
} EvtNod;



/* Data: */

extern int evtmin, evtmax;
extern int evtcount;



/* Functions: */

/* Create a new Event node */
extern EvtNod *newevt(Srcp *srcp,
		      IdNode *id,
		      List *stms);

/* Analyze all Events */
extern void anevts(void);

/* Generate code for all Events */
extern Aaddr geevts(void);

/* Dump an Event node */
extern void duevt(EvtNod *evt);


#endif

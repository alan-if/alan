#ifndef _EVT_H_
#define _EVT_H_
/*----------------------------------------------------------------------*\

				EVT.H
			     Event Nodes

\*----------------------------------------------------------------------*/


/* Use other definitions */
#include "srcp.h"
#include "nam.h"
#include "lst.h"


/* Types: */

typedef struct EvtNod {		/* EVENT */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name of this event */
  Aaddr namadr;			/* Address to name string (debug) */
  List *stms;			/* List of statements to execute */
  Aaddr stmadr;			/* ACODE address of event statements */
} EvtNod;



/* Data: */

extern int evtmin, evtmax;
extern int evtcount;



/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Event node */
extern EvtNod *newevt(Srcp *srcp,
		      NamNod *nam,
		      List *stms);

/* Analyze all Events */
extern void anevts(void);

/* Generate code for all Events */
extern Aaddr geevts(void);

/* Dump an Event node */
extern void duevt(EvtNod *evt);

#else
extern EvtNod *newevt();
extern void anevts();
extern Aaddr geevts();
extern void duevt();
#endif

#endif

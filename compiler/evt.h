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

#include "../interpreter/acode.h"


/* Types: */

typedef struct EvtNod {		/* EVENT */
  Srcp srcp;			/* Source position */
  IdNode *id;			/* Name of this event */
  Aaddr namadr;			/* Address to name string (debug) */
  List *stms;			/* List of statements to execute */
  Aaddr stmadr;			/* ACODE address of event statements */
} EvtNod;



/* Data: */


/* Functions: */

extern EvtNod *newevt(Srcp *srcp, IdNode *id, List *stms);
extern void anevts(void);
extern Aaddr generateEvents(AcdHdr *header);
extern void duevt(EvtNod *evt);


#endif

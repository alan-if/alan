#ifndef _INSTANCE_H_
#define _INSTANCE_H_
/*----------------------------------------------------------------------*\

			       Instance.h

			     Instance Nodes

\*----------------------------------------------------------------------*/


  /* USE: */
#include "Srcp.h"
#include "List.h"
#include "Id.h"
#include "Slot.h"

/* Types: */

typedef struct Instance {		/* Instance Node */
  Srcp srcp;
  Id *id;
  int code;
  Slot *slot;
} Instance;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Instance node */
extern Instance *newInstance(Srcp *srcp,
			     Id *id,
			     Slot *slot);

/* Create predefined instances */
extern void initInstances(void);

/* Analyse an Instance */
extern void analyseInstance(Instance *instance);

/* Generate Acode for all instances */
extern Aaddr generateInstances(void);

/* Dump an Instance */
extern void dumpInstance(Instance *instance);

#else
extern void initInstances();
extern Instance *newInstance();
extern generateInstances();
extern void dumpInstance();
#endif

#endif

#ifndef _INS_H_
#define _INS_H_
/*----------------------------------------------------------------------*\

				INS.H
			     Instance Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "id.h"
#include "lst.h"
#include "whr.h"
#include "slt.h"


/* Types */

typedef struct InsNod {		/* INSTANCE */
  Srcp srcp;			/* Source position */
  struct IdNod *id;		/* Name of the instance */
  List *heritage;		/* The instances heritage */
  Slots *slt;			/* Slots */
} InsNod;


/* Data: */

/* Methods: */


/* Create a new Instance node */
extern InsNod *newins(Srcp *srcp,
		      struct IdNod *id,
		      List *heritage,
		      Slots *slt);

/* Analyze all Instances */
extern void aninss(void);

/* Prepare all Instances before analysis */
extern void prepinss(void);

/* Generate code for all Instances */
extern Aaddr geinss(void);

/* Dump an Instance node */
extern void dumpInstance(InsNod *ins);

#endif

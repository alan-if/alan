#ifndef _INS_H_
#define _INS_H_
/*----------------------------------------------------------------------*\

				INS.H
			     Instance Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "lst.h"
#include "whr.h"
#include "slt.h"


/* Types */

typedef struct InsNod {		/* INSTANCE */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name of the instance */
  Slots *slt;			/* Slots */
} InsNod;


/* Data: */

/* Methods: */


/* Create a new Instance node */
extern InsNod *newins(Srcp *srcp,
		      NamNod *nam,
		      List *nams,
		      Slots *slt);

/* Analyze all Instances */
extern void aninss(void);

/* Prepare all Instances before analysis */
extern void prepinss(void);

/* Generate code for all Instances */
extern Aaddr geinss(void);

/* Dump an Instance node */
extern void duins(InsNod *ins);

#endif

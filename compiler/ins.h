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
  struct IdNode *id;		/* Name of the instance */
  struct SymNod *symbol;	/* Pointer to symbol table node */
  struct IdNode *heritage;	/* The instances heritage */
  struct Slots *slt;		/* Slots */
} InsNod;


/* Data: */
extern int instanceCount;


/* Methods: */


/* Create a new Instance node */
extern InsNod *newins(Srcp *srcp,
		      IdNode *id,
		      IdNode *heritage,
		      Slots *slt);

/* Analyze all Instances */
extern void analyzeInstances(void);

/* Prepare all Instances before analysis */
extern void prepinss(void);

/* Generate code for all Instances */
extern Aaddr generateInstances(void);

/* Dump an Instance node */
extern void dumpInstance(InsNod *ins);

#endif

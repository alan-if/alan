#ifndef _INS_X_H_
#define _INS_X_H_
/*----------------------------------------------------------------------*\

				INS_X.H
			     Instance Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "ins.h"

/* USE: */


/* DATA: */
extern int instanceCount;
extern InsNod *theHero;


/* METHODS: */

extern void initInstances();

/* Create a new Instance node */
extern InsNod *newInstance(Srcp *srcp,
			   IdNode *id,
			   IdNode *heritage,
			   SlotsNode *slt);

/* Symbolize all Instances */
extern void symbolizeInstances(void);

/* Analyze all Instances */
extern void analyzeInstances(void);

/* Prepare all Instances before analysis */
extern void prepinss(void);

/* Generate code for all Instances */
extern Aaddr generateInstances(void);

/* Dump an Instance node */
extern void dumpInstance(InsNod *ins);

#endif

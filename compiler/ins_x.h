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
extern InsNod *newInstance(Srcp *srcp,
			   IdNode *id,
			   IdNode *heritage,
			   SlotsNode *slt);
extern void symbolizeInstances(void);
extern void analyzeInstances(void);
extern void generateInstances(AcdHdr *header);
extern void dumpInstance(InsNod *ins);

#endif

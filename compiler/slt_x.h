#ifndef _SLT_X_H_
#define _SLT_X_H_
/*----------------------------------------------------------------------*\

				SLT.H
			     Slots Nodes
                                                                     
\*----------------------------------------------------------------------*/

#include "slt.h"

/* USE: */


/* DATA: */

/* METHODS: */

extern SlotsNode *newEmptySlots(void);

extern SlotsNode *newSlots(List *nams,
			   WhrNod *whr,
			   List *atrs,
			   CntNod *cnt,
			   List *dscr,
			   List *ment,
			   List *art,
			   List *does,
			   List *exts,
			   List *vrbs,
			   List *scrs);

extern void symbolizeSlots(SlotsNode *slots);
extern void analyzeSlots(SlotsNode *slts);
extern void generateSlotsData(SlotsNode *slts);
extern void generateSlotsEntry(SlotsNode *slts);
extern void dumpSlots(SlotsNode *slots);


#endif

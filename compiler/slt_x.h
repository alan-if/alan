#ifndef _SLT_X_H_
#define _SLT_X_H_
/*----------------------------------------------------------------------*\

				SLT.H
			  Slots Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "slt.h"

/* USE: */
#include "../interpreter/acode.h"
#include "ins.h"
#include "context.h"


/* DATA: */

/* METHODS: */

extern Slots *newEmptySlots(void);

extern Slots *newSlots(List *nams,
			   WhrNod *whr,
			   List *atrs,
			   CntNod *cnt,
			   List *description,
			   Srcp *descriptionSrcp,
			   List *mentioned,
			   Srcp *mentionedSrcp,
			   List *article,
			   Srcp *articleSrcp,
			   List *does,
			   List *exts,
			   List *vrbs,
			   List *scrs);

extern void symbolizeSlots(Slots *slots);
extern void analyzeSlots(Slots *slts, Context *context);
extern void generateSlotsData(Slots *slts);
extern void generateSlotsEntry(InstanceEntry *entry, Slots *slts);
extern void dumpSlots(Slots *slots);


#endif

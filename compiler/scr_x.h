#ifndef _SCR_X_H_
#define _SCR_X_H_
/*----------------------------------------------------------------------*\

				SCR_X.H
			 Script Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "scr.h"


/* USE other definitions */
#include "srcp.h"
#include "lst.h"
#include "ins.h"
#include "context.h"



/* DATA: */


/* FUNCTIONS: */

extern ScrNod *newScript(Srcp *srcp,
			 IdNode *id,
			 int code,
			 List *descr,
			 List *stps);

extern void prepareScripts(List *scrs, InsNod *ins);
extern void analyzeScripts(List *scrs, Context *context);
extern Aword generateScripts(List *scrs, int currentInstance);
extern void dumpScript(ScrNod *scr);


#endif

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
#include "description.h"
#include "context.h"



/* DATA: */


/* FUNCTIONS: */

extern Script *newScript(Srcp *srcp, IdNode *id, Description *descr, List *steps);
extern void prepareScripts(List *scripts, IdNode *id);
extern void analyzeScripts(List *scripts, Context *context);
extern Aword generateScripts(ACodeHeader *header);
extern void dumpScript(Script *script);

#endif

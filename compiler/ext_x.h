#ifndef _EXT_X_H_
#define _EXT_X_H_
/*----------------------------------------------------------------------*\

				EXT.H
			  Exit Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "ext.h"

/* USE: */
#include "context.h"


/* DATA: */

extern int dirmin, dirmax;
extern int dircount;


/* FUNCTIONS: */

/* Create a new Exit node */
extern ExtNod *newext(Srcp *srcp,
		      List *dirs,
		      IdNode *to,
		      List *chks,
		      List *stms);

extern void symbolizeExits(List *exts);
extern void analyzeExits(List *exts, Context *context);
extern Aaddr generateExits(List *exts, int currentInstance);
extern void dumpExit(ExtNod *ext);


#endif

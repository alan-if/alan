#ifndef _CNT_X_H_
#define _CNT_X_H_
/*----------------------------------------------------------------------*\

				CNT_X.H
			   Container Nodes

\*----------------------------------------------------------------------*/

#include "cnt.h"

/* USE: */
#include "wht.h"
#include "context.h"


/* DATA: */


/* FUNCTIONS: */

extern CntNod *newContainer(Srcp *srcp, List *lims, List *hstms, List *estms);
extern CntNod *copyContainer(CntNod *theOriginal);
extern void verifyContainer(WhtNod *wht, Context *context);
extern void analyzeContainer(CntNod *cnt, Context *context);
extern Aaddr generateContainers(AcdHdr *header);
extern void dumpContainer(CntNod *container);

#endif

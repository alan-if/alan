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

extern int cntmin, cntmax;
extern int cntcount;



/* FUNCTIONS: */

extern CntNod *newContainer(Srcp *srcp,
			    List *lims,
			    List *hstms,
			    List *estms);

extern void verifyContainer(WhtNod *wht, Context *context);
extern void analyzeContainer(CntNod *cnt, Context *context);
extern Aaddr generateContainers(void);
extern void dumpContainer(CntNod *container);

#endif

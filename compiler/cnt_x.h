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



/* Functions: */

extern void initContainers(void);
extern CntNod *newContainer(Srcp *srcp,
			    List *lims,
			    List *hstms,
			    List *estms);

extern void containerCheck(WhtNod *wht, Context *context);
extern void analyseContainer(CntNod *cnt, Context *context);
extern Aaddr generateContainers(void);
extern void dumpContainer(CntNod *container);

#endif

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

extern ContainerBody *newContainerBody(Srcp *srcp, List *lims, List *hstms, List *estms);
extern Container *newContainer( ContainerBody *info);
extern void verifyContainer(What *wht, Context *context);
extern void analyzeContainer(Container *cnt, Context *context);
extern void numberContainers(void);
extern Aaddr generateContainers(AcdHdr *header);
extern void dumpContainer(Container *container);

#endif

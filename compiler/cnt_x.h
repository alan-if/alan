#ifndef _CNT_X_H_
#define _CNT_X_H_
/*----------------------------------------------------------------------*\

				CNT_X.H
			   Container Nodes

\*----------------------------------------------------------------------*/

#include "cnt.h"

/* USE: */
#include "wht.h"


/* DATA: */

extern int cntmin, cntmax;
extern int cntcount;



/* Functions: */

/* Initialise container data */
extern void initcnt(void);

/* Create a new Container node */
extern CntNod *newcnt(Srcp *srcp,
		      IdNode *nam,
		      List *lims,
		      List *hstms,
		      List *estms);

/* Check a possible containers code */
extern void cntcheck(WhtNod *wht, List *pars);

/* Analyse one Container */
extern void ancnt(CntNod *cnt);

/* Analyse all Containers */
extern void ancnts(void);

/* Generate code for all Containers */
extern Aaddr gecnts(void);

/* Dump a Container node */
extern void ducnt(CntNod *cnt);

#endif

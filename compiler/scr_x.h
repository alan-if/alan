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



/* DATA: */


/* FUNCTIONS: */

extern ScrNod *newScript(Srcp *srcp,
			 IdNode *id,
			 int code,
			 List *descr,
			 List *stps);

extern Aword generateScripts(InsNod *ins);

/* Prepare the scripts of an Instance for analysis */
extern void prepscrs(List *scrs, InsNod *ins);

/* Analyse a list of Scripts */
extern void anscrs(List *scrs,
		   InsNod *ins);

/* Dump a Script node */
extern void duscr(ScrNod *scr);


#endif

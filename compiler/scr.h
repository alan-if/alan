#ifndef _SCR_H_
#define _SCR_H_
/*----------------------------------------------------------------------*\

				SCR.H
			     Script Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "srcp.h"
#include "lst.h"
#include "ins.h"


/* Types: */

typedef struct ScrNod {		/* SCRIPT */
  Srcp srcp;			/* Source position */
  IdNode *id;			/* Id for this script (if any) */
  int code;			/* and its code */
  List *descr;			/* List of optional description statements */
  Aaddr stmadr;			/* ACODE address description statements */
  List *stps;			/* List of step-blocks */
  Aaddr stpadr;			/* ACODE address to step table */
} ScrNod;



#endif

#ifndef _CLA_H_
#define _CLA_H_
/*----------------------------------------------------------------------*\

				CLASS.H
			     Class Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "id.h"
#include "slt.h"


/* TYPES: */

typedef struct ClaNod {		/* CLASS */
  Srcp srcp;			/* Source position */
  SlotsNode *slots;
  Aaddr adr;			/* Generated at address */
} ClaNod;


#endif

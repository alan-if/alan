#ifndef _ADD_H_
#define _ADD_H_
/*----------------------------------------------------------------------*\

				ADD.H
			     Add To Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "id.h"
#include "slt.h"


/* TYPES: */

typedef struct AddNode {	/* ADD TO */
  Srcp srcp;			/* Source position */
  IdNode *toId;			/* Class to add to */
  Slots *slots;
  Aaddr adr;			/* Generated at address */
} AddNode;


#endif

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
  IdNode *id;			/* Programmer Name of the class */
  SymNod *symbol;		/* Pointer to symbol table node */
  IdNode *parent;		/* The class' heritage */
  SlotsNode *slt;		/* Slots */
  Aaddr adr;			/* Generated at address */
} ClaNod;


#endif
